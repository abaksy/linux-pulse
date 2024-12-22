#include "../utils.hpp"

#define ENSEMBLE_SIZE 1000000

#define KB 1024

//Size of memory region that is accessed in KB
int sizeList[18] = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288};
//The stride length in Bytes
int strideList[7] = {4, 64, 128, 1024, 1048576, 4194304, 16777216};
struct timediff t;

/*
Calculate average latency of main memory access
using RDTSC and RDTSCP x86 instructions
*/

double average_mem_latency(int size, int strideLength)
{
    uint64_t start_time, end_time, total_time;

    
    
    int num, length, index;
    length = strideLength / sizeof(int);   
    num = size * KB / sizeof(int);
    int* A = new int[num];
    A[0] = 0;
    for (int i = 0; i < num; i++) {
        index = i + length;
        if (index >= num) {
            index %= num;
        }
        A[i] = index;
    }

    int x = 0;

    tic(t);
    for (int i = 0; i < ENSEMBLE_SIZE; i++) {
        x = A[x];
    }
    toc(t);

    delete[] A;

    start_time = getcycles(t.cycles_high0, t.cycles_low0);
    end_time = getcycles(t.cycles_high1, t.cycles_low1);
    total_time = end_time - start_time;
    double ans = (double)(total_time)/ENSEMBLE_SIZE;   
    return ans;
}

int main(int argc, char* argv[])
{
    std::cout << std::fixed << std::setprecision(3);
    int stride_length = 4; // Default stride length
    if(argc > 1) {
        stride_length = atoi(argv[1]);
        std::cout << "Using stride length of " << stride_length << '\n';
    } else {
        std::cout << "Using default stride length of " << stride_length << '\n';
    }

    // Evaluate memory accesses for different array size, fixed stride size
    for(int i = 0; i < 18; ++i)
    { 
            double avg_time = average_mem_latency(sizeList[i], stride_length);
            std::cout << sizeList[i] << ',' << avg_time << '\n';
    }
    return 0;
}

