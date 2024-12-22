#include "../utils.hpp"

#define MEASUREMENT_LOOP_ITER 1000000 // 1e6
#define LOOP_ITER 1000
#define ENSEMBLE_SIZE 1000
struct timediff t;

/*
Calculate overhead of procedure call inside user space
using RDTSC and RDTSCP x86 instructions
*/
void procedure0() {}
void procedure1 (int a0) {}
void procedure2 (int a0, int a1) {}
void procedure3 (int a0, int a1, int a2) {}
void procedure4 (int a0, int a1, int a2, int a3) {}
void procedure5 (int a0, int a1, int a2, int a3, int a4) {}
void procedure6 (int a0, int a1, int a2, int a3, int a4, int a5) {}
void procedure7 (int a0, int a1, int a2, int a3, int a4, int a5, int a6) {}

void measure_time(int **times)
{
    std::vector<double> samples;
    
    double global_sum = 0;
    for(int j = 0 ; j < LOOP_ITER; ++j)
    {
        uint64_t sum = 0, var_sum = 0;
        for(int i = 0; i < ENSEMBLE_SIZE; ++i)
        {
            sum += (times[j][i]);
        }
        global_sum += sum;
        double avg = sum/ENSEMBLE_SIZE; // Sample Mean
        // std::cout << "Mean: "<< avg << '\n';
        samples.push_back(avg);
    }

    // Calculate population variance from sample means

    double pop_mean = (std::accumulate(samples.begin(), samples.end(), 0))/(LOOP_ITER);
    double pop_error = 0.0;
    for(int j =0; j < LOOP_ITER; ++j)
    {
        pop_error += (samples[j] - pop_mean)*(samples[j] - pop_mean);
    }
    std::cout << "Overall Mean: "<< global_sum/(LOOP_ITER*ENSEMBLE_SIZE) << " Std Deviation: " << sqrt(pop_error/(LOOP_ITER)) << '\n';
}
   
int main()
{
    // cycles_high contains higher-order 32 bits of timestamp counter
    // cycles_low contains lower-order 32 bits of timestamp counter
    uint32_t cycles_high0, cycles_low0;
    uint32_t cycles_high1, cycles_low1;
    uint64_t start_time, end_time;
    double cycles_taken;

    int **times = new int*[LOOP_ITER];
    for(int i = 0; i < LOOP_ITER; ++i) {
        times[i] = new int[ENSEMBLE_SIZE];
    }

    
    long long int total_sum = 0;

    // Set nice value -20 for highest possible priority
    set_priority(-20);
    
    // Warmup iterations
    asm volatile ("CPUID\n\t"
        "RDTSC\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t": "=r" (cycles_high0), "=r" (cycles_low0)::
        "%rax", "%rbx", "%rcx", "%rdx");

    asm volatile ("RDTSCP\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t"
        "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
        "%rax", "%rbx", "%rcx", "%rdx");
    
    asm volatile ("CPUID\n\t"
        "RDTSC\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t": "=r" (cycles_high0), "=r" (cycles_low0)::
        "%rax", "%rbx", "%rcx", "%rdx");

    asm volatile ("RDTSCP\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t"
        "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
        "%rax", "%rbx", "%rcx", "%rdx");

    // N = 0
    // procedure0();
    // procedure0();
    int ii = 0;
    for(int j = 0; j < LOOP_ITER; ++j)
    {
        for(int i = 0; i < ENSEMBLE_SIZE; ++i)
        {
            tic(t);
            
            procedure0();

            toc(t);
            
            start_time = getcycles(t.cycles_high0, t.cycles_low0);
            end_time = getcycles(t.cycles_high1, t.cycles_low1);

            assert(start_time <= end_time);

            // number of CPU cycles taken
            // cycles_taken = end_time - start_time;

            times[j][i] = (end_time - start_time);
        }
    }

    measure_time(times);
    
    // N = 1
    // procedure1(1);
    // procedure1(1);
    ii = 0;
    for(int j = 0; j < LOOP_ITER; ++j)
    {
        for(int i = 0; i < ENSEMBLE_SIZE; ++i)
        {
            tic(t);
            
            procedure1(1);

            toc(t);
            
            start_time = getcycles(t.cycles_high0, t.cycles_low0);
            end_time = getcycles(t.cycles_high1, t.cycles_low1);

            assert(start_time <= end_time);

            // number of CPU cycles taken
            // cycles_taken = end_time - start_time;tic(t);
            times[j][i] = (end_time - start_time);
        }
    }

    measure_time(times);
    
    // N = 2
    // procedure2(1, 2);
    // procedure2(1, 2);

    ii = 0;
    for(int j = 0; j < LOOP_ITER; ++j)
    {
        for(int i = 0; i < ENSEMBLE_SIZE; ++i)
        {
            tic(t);

                procedure2(1, 2);

            toc(t);

            start_time = getcycles(t.cycles_high0, t.cycles_low0);
            end_time = getcycles(t.cycles_high1, t.cycles_low1);

            assert(start_time <= end_time);

            // number of CPU cycles taken
            // cycles_taken = end_time - start_time;

            times[j][i] = (double)(end_time - start_time);
        }
    }

    measure_time(times);
    
    // N = 3
    // procedure3(1, 2, 3);
    // procedure3(1, 2, 3);
    ii = 0;
    for(int j = 0; j < LOOP_ITER; ++j)
    {
        for(int i = 0; i < ENSEMBLE_SIZE; ++i)
        {
            tic(t);
            
                procedure3(1, 2, 3);

            toc(t);
            
            start_time = getcycles(t.cycles_high0, t.cycles_low0);
            end_time = getcycles(t.cycles_high1, t.cycles_low1);

            assert(start_time <= end_time);

            // number of CPU cycles taken
            // cycles_taken = end_time - start_time;

            times[j][i] = (double)(end_time - start_time);
        }
    }

    measure_time(times);

    // N = 4
    // procedure4(1, 2, 3, 4);
    // procedure4(1, 2, 3, 4);
    ii = 0;
    for(int j = 0; j < LOOP_ITER; ++j)
    {
        for(int i = 0; i < ENSEMBLE_SIZE; ++i)
        {
            tic(t);
            
            
                procedure4(1, 2, 3, 4);

            toc(t);
            
            start_time = getcycles(t.cycles_high0, t.cycles_low0);
            end_time = getcycles(t.cycles_high1, t.cycles_low1);

            assert(start_time <= end_time);

            // number of CPU cycles taken
            // cycles_taken = end_time - start_time;

            times[j][i] = (double)(end_time - start_time);
        }
    }

    measure_time(times);


    // N = 5
    // procedure5(1, 2, 3, 4, 5);
    // procedure5(1, 2, 3, 4, 5);
    ii = 0;
    for(int j = 0; j < LOOP_ITER; ++j)
    {
        for(int i = 0; i < ENSEMBLE_SIZE; ++i)
        {
            tic(t);

            
                procedure5(1, 2, 3, 4, 5);

            toc(t);
            
            start_time = getcycles(t.cycles_high0, t.cycles_low0);
            end_time = getcycles(t.cycles_high1, t.cycles_low1);

            assert(start_time <= end_time);

            // number of CPU cycles taken
            // cycles_taken = end_time - start_time;

            times[j][i] = (double)(end_time - start_time);
        }
    }

    measure_time(times);


    // N = 6
    // procedure6(1, 2, 3, 4, 5, 6);
    // procedure6(1, 2, 3, 4, 5, 6);
    ii = 0;
    for(int j = 0; j < LOOP_ITER; ++j)
    {
        for(int i = 0; i < ENSEMBLE_SIZE; ++i)
        {
            tic(t);
            
            
                procedure6(1, 2, 3, 4, 5, 6);

            toc(t);
            
            start_time = getcycles(t.cycles_high0, t.cycles_low0);
            end_time = getcycles(t.cycles_high1, t.cycles_low1);

            assert(start_time <= end_time);

            // number of CPU cycles taken
            // cycles_taken = end_time - start_time;

            times[j][i] = (double)(end_time - start_time);
        }
    }

    measure_time(times);


    // N = 7   
    ii = 0;
    // procedure7(1, 2, 3, 4, 5, 6, 7);
    // procedure7(1, 2, 3, 4, 5, 6, 7);

    for(int j = 0; j < LOOP_ITER; ++j)
    {
        for(int i = 0; i < ENSEMBLE_SIZE; ++i)
        {
            tic(t);
            
            
                procedure7(1, 2, 3, 4, 5, 6, 7);

            toc(t);
            
            start_time = getcycles(t.cycles_high0, t.cycles_low0);
            end_time = getcycles(t.cycles_high1, t.cycles_low1);

            assert(start_time <= end_time);

            // number of CPU cycles taken
            // cycles_taken = end_time - start_time;

            times[j][i] = (double)(end_time - start_time);
        }
    }

    measure_time(times);
    for(int i = 0; i < ENSEMBLE_SIZE; ++i) {
        delete times[i];
    }
    delete times;

    return 0;
}

