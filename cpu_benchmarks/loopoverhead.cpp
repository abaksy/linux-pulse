#include "../utils.hpp"

#define MEASUREMENT_LOOP_ITER 1000000 // 1e6
#define LOOP_ITER 100
#define ENSEMBLE_SIZE 100

/*
Calculate overhead of time measurement inside a Loop using RDTSC and RDTSCP x86 instructions
*/
int main()
{
    // cycles_high contains higher-order 32 bits of timestamp counter
    // cycles_low contains lower-order 32 bits of timestamp counter
    uint32_t cycles_high0, cycles_low0;
    uint32_t cycles_high1, cycles_low1;
    uint64_t start_time, end_time, cycles_taken;
    long long int total_sum = 0;

    double **times = new double*[ENSEMBLE_SIZE];
    for(int i = 0; i < LOOP_ITER; ++i) {
        times[i] = new double[LOOP_ITER];
    }


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
    int ii = 0;
    for(int j = 0; j < LOOP_ITER; ++j)
    {
        for(int i = 0; i < ENSEMBLE_SIZE; ++i)
        {
            asm volatile ("CPUID\n\t"
                "RDTSC\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t": "=r" (cycles_high0), "=r" (cycles_low0)::
                "%rax", "%rbx", "%rcx", "%rdx");

            for(ii = 0; ii < MEASUREMENT_LOOP_ITER; ++ii)
                ;

            asm volatile ("RDTSCP\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t"
                "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
                "%rax", "%rbx", "%rcx", "%rdx");
            
            start_time = ((uint64_t)cycles_high0<<32 | cycles_low0);
            end_time = ((uint64_t)cycles_high1<<32 | cycles_low1);

            assert(start_time <= end_time);

            // number of CPU cycles taken
            // cycles_taken = end_time - start_time;

            times[j][i] = (double)(end_time - start_time);
        }
    }

    double loopOverhead = readLoopOverhead();

    std::vector<double> samples;
    
    double global_sum = 0;
    for(int j = 0 ; j < LOOP_ITER; ++j)
    {
        uint64_t sum = 0, var_sum = 0;
        for(int i = 0; i < ENSEMBLE_SIZE; ++i)
        {
            times[j][i] = (times[j][i] - loopOverhead);
            sum += (times[j][i])/MEASUREMENT_LOOP_ITER;
        }
        global_sum += sum;
        double avg = sum/ENSEMBLE_SIZE; // Sample Mean
        std::cout << "Mean: "<< avg << '\n';
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

    for(int i = 0; i < ENSEMBLE_SIZE; ++i) {
        delete times[i];
    }
    delete times;

    return 0;
}