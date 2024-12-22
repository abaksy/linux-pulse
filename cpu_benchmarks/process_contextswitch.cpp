#include "../utils.hpp"

#define LOOP_ITER 100
#define ENSEMBLE_SIZE 1000

/*
Calculate overhead of process context switching
using RDTSC and RDTSCP x86 instructions
*/

uint64_t inline rdtsc()
{
    uint32_t hi, lo;

    asm volatile("CPUID\n\t"
                 "RDTSC\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 : "=r"(hi), "=r"(lo)::"%rax", "%rbx", "%rcx", "%rdx");

    return getcycles(hi, lo);
}

uint64_t inline rdtscp()
{
    uint32_t hi, lo;

    asm volatile("RDTSCP\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 "CPUID\n\t" : "=r"(hi), "=r"(lo)::"%rax", "%rbx", "%rcx", "%rdx");

    return getcycles(hi, lo);
}

static int fd[2]; // File descriptor array for pipe creation, 0 = read end, 1 = write end

uint64_t calculate_ctx_switch_time(int* fd)
{
    uint64_t start;
    uint64_t end;
    pid_t child_pid;
    uint64_t result = 0;

    if ((child_pid = fork()) != 0) {
        // Inside parent process, start timer and wait for child to respond back
        start = rdtsc();

        wait(NULL);

        // Read the end time written by the child from the pipe
        read(fd[0], (void*)&end, sizeof(uint64_t));
    }
    else {
        // Inside child process
        end = rdtsc();
        // Stop timer, write the value to the pipe
        write(fd[1], (void*)&end, sizeof(uint64_t));
        exit(1);
    }
    if(end > start){
        result = end - start;
    }
    return (result);
}


double get_context_switch_overhead()
{
    pipe(fd);
    uint64_t s[ENSEMBLE_SIZE];
    double sum = 0;
    int i = 0;

    while(i < ENSEMBLE_SIZE) {
        uint64_t res = calculate_ctx_switch_time(fd);
        if (res > 0) {
            s[i] = res;
            sum += s[i];
            ++i;
        }
    }

    static double res;
    res = (double)sum / (double)ENSEMBLE_SIZE;

    return res;
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
        double ctx_switch_avg = get_context_switch_overhead();
        global_avg += ctx_switch_avg;
        std::cout << "Mean:" << ctx_switch_avg << '\n';
        timing_data[i] = ctx_switch_avg;
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
