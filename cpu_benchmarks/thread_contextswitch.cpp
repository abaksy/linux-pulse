#include "../utils.hpp"

#define LOOP_ITER 100
#define ENSEMBLE_SIZE 1000

/*
Calculate overhead of thread context switching
using RDTSC and RDTSCP x86 instructions
*/

uint64_t rdtsc()
{
    uint32_t hi, lo;

    asm volatile("CPUID\n\t"
                 "RDTSC\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 : "=r"(hi), "=r"(lo)::"%rax", "%rbx", "%rcx", "%rdx");

    return getcycles(hi, lo);
}

uint64_t rdtscp()
{
    uint32_t hi, lo;

    asm volatile("RDTSCP\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 "CPUID\n\t" : "=r"(hi), "=r"(lo)::"%rax", "%rbx", "%rcx", "%rdx");

    return getcycles(hi, lo);
}

static int fd[2]; // File descriptor array for pipe creation, 0 = read end, 1 = write end

static inline void *foo(void *)
{
    // Stop the counter
    uint64_t tt = rdtscp();

    // Write the timestamp value to the pipe so that the parent thread can read it 
    write(fd[1], (void *)&tt, sizeof(uint64_t));
    pthread_exit(NULL);
}

uint64_t calculate_kernel_switch()
{
    uint64_t threadSt;
    uint64_t threadEd;

    pthread_t thread;

    // Create a pipe from the fd array 
    pipe(fd);
    pthread_create(&thread, NULL, foo, NULL);

    // Start the counter, which will be ended inside the thread function foo
    threadSt = rdtsc();
    pthread_join(thread, NULL);
    
    // Read the end timestamp value from the pipe (it was written by the spawned thread)
    read(fd[0], (void *)&threadEd, sizeof(uint64_t));
    

    return threadEd - threadSt;
}

double get_kernel_thread_switch_overhead()
{
    uint64_t s[ENSEMBLE_SIZE];
    double sum = 0.0;
    double tmp = 0.0;

    int i = 0;
    while (i < ENSEMBLE_SIZE)
    {
        uint64_t res = calculate_kernel_switch();

        if (res > 0 && res < 20000)
        {
            s[i] = res;
            sum += s[i];
            i++;
        }
    }

    return sum / (double)ENSEMBLE_SIZE;
}

int main()
{
    // cycles_high contains higher-order 32 bits of timestamp counter
    // cycles_low contains lower-order 32 bits of timestamp counter
    uint32_t cycles_high0, cycles_low0;
    uint32_t cycles_high1, cycles_low1;
    uint64_t start_time, end_time;
    double cycles_taken;

    long long int total_sum = 0;

    std::cout << std::fixed << std::setprecision(5);

    // Set nice value -20 for highest possible priority
    set_priority(-20);

    // Warmup iterations
    asm volatile("CPUID\n\t"
                 "RDTSC\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t" : "=r"(cycles_high0), "=r"(cycles_low0)::"%rax", "%rbx", "%rcx", "%rdx");

    asm volatile("RDTSCP\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 "CPUID\n\t" : "=r"(cycles_high1), "=r"(cycles_low1)::"%rax", "%rbx", "%rcx", "%rdx");

    asm volatile("CPUID\n\t"
                 "RDTSC\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t" : "=r"(cycles_high0), "=r"(cycles_low0)::"%rax", "%rbx", "%rcx", "%rdx");

    asm volatile("RDTSCP\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 "CPUID\n\t" : "=r"(cycles_high1), "=r"(cycles_low1)::"%rax", "%rbx", "%rcx", "%rdx");

    double global_avg = 0.0, global_sq_sum = 0.0;
    double* timing_data = new double[LOOP_ITER];
    for (int i = 0; i < LOOP_ITER; i++)
    {
        // get sample mean
        double kernelSwitchAvg = get_kernel_thread_switch_overhead();
        global_avg += kernelSwitchAvg;
        std::cout << "Mean:" << kernelSwitchAvg << '\n';
        timing_data[i] = kernelSwitchAvg;
    }

    global_avg = global_avg/(double)(LOOP_ITER);

    for (int i = 0; i < LOOP_ITER; i++)
    {
        // get population variance
        global_sq_sum += (timing_data[i] - global_avg)*(timing_data[i] - global_avg);
    }

    std::cout << "Overall Mean: " << global_avg << " Std. Dev. " << sqrt(global_sq_sum/(LOOP_ITER)) << '\n';

    delete timing_data;
    return 0;
}
