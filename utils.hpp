#ifndef OS_BENCHMARK_UTILS_H
#define OS_BENCHMARK_UTILS_H

#include <cmath>
#include <numeric>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <stdint.h>
#include <stdfloat>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <random>

extern const std::string timingoverhead = "timingoverhead";
extern const std::string loopoverhead = "loopoverhead";

struct timediff {
    uint32_t cycles_high0, cycles_low0, cycles_high1, cycles_low1;
};


#define tic(timer)              \
    asm volatile ("CPUID\n\t"   \
        "RDTSC\n\t"             \
        "mov %%edx, %0\n\t"     \
        "mov %%eax, %1\n\t"     \
        : "=r" (timer.cycles_high0), "=r" (timer.cycles_low0) \
        :: "%rax", "%rbx", "%rcx", "%rdx"); 

#define toc(timer) \
    asm volatile ("RDTSCP\n\t"  \
        "mov %%edx, %0\n\t"     \
        "mov %%eax, %1\n\t"     \
        "CPUID\n\t": "=r" (timer.cycles_high1), "=r" (timer.cycles_low1):: \
        "%rax", "%rbx", "%rcx", "%rdx");

#define getcycles(hi, lo) (((uint64_t)hi << 32) | lo)

void set_priority(int new_priority)
{

	int prior_priority, later_priority;
	int ret;
	pid_t pid;

	pid = getpid();
	prior_priority = getpriority(PRIO_PROCESS,pid);

	ret = nice(new_priority);

	if (ret != new_priority)
		std::cerr << "[FAILED] set_nice!\n";

	later_priority = getpriority(PRIO_PROCESS,pid);
}

double readTimingOverhead()
{
    // read average timing measurement overhead from file
    std::string filepath = "results/" + timingoverhead;
    std::ifstream inFile(filepath.c_str());
    std::string buffer;
    getline(inFile, buffer);

    double timingOverhead = std::stod(buffer);
    return timingOverhead;
}


double readLoopOverhead() 
{
    // read average loop measurement overhead from file
    std::string filepath = "results/" + loopoverhead;
    std::ifstream inFile(filepath.c_str());
    std::string buffer;
    getline(inFile, buffer);

    double loopOverhead = std::stod(buffer);
    return loopOverhead;
}


#endif