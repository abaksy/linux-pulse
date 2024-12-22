# Operating System Measurements for x64 machines running Linux
Measuring some key system parameters for Linux systems running on Intel x64 hardware. We use a low-overhead method of measuring time using the ```rdtsc``` instruction to read the CPU's internal clock that contains a monotonically increasing value, running at the specified **base frequency** of the CPU. The experimental methodology for many of the experiments is heavily inspired by [LMBench](https://www.usenix.org/legacy/publications/library/proceedings/sd96/full_papers/mcvoy.pdf), and we use the methodology decribed in Gabriele Paoloni's 2010 [white paper](https://github.com/tpn/pdfs/blob/master/How%20to%20Benchmark%20Code%20Execution%20Times%20on%20Intel%20IA-32%20and%20IA-64%20Instruction%20Set%20Architectures%20-%20September%2C%202010%20(324264-001).pdf) for our execution time measurements.

## CPU and System Benchmarks
1. Process and thread creation time
2. Process and thread context switching overheads
3. Syscall and procedure call overheads

## Memory Benchmarks
1. RAM Access Time
2. RAM bandwidth
3. Page fault time (hard page fault)

## Network benchmarks
1. Peak network bandwidth
2. Round-trip time
3. TCP Connection setup and teardown time

## File System Benchmarks
1. File cache size measurement
2. File read benchmark (local and remote via NFS)

## Prerequisites 

First ensure that the CPU is running Intel Pstate drivers

```bash
$ cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_driver

intel_pstate
....
```

Then, ensure that dynamic CPU frequency scaling (a.k.a Turbo Boost) is switched off (note that this change **does not persist** across reboots)

```bash
sudo echo "1" | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo
```

## Build Instructions
Build the benchmarks individually using 
```bash
mkdir bin
make -C <benchmark_folder> all
```
```benchmark_folder``` can be either cpu_benchmarks, mem_benchmarks, nw_benchmarks or fs_benchmarks based on the benchmark you want to run 

Use the ```clean``` target to clean up the binary files after finishing the execution

# Benchmarks

## CPU Operations

Run the binary file to get the average and standard deviation of time measurements using 

```bash
sudo taskset -c 1 ./bin/<binary name> 
```

```sudo``` is important as ```nice``` commands to set the priority requires root user privileges

## Memory Operations
**Warning**: You will need atleast 8GB of free disk space to use the temp files that are used for these benchmarks

Run the binary file to get the average and standard deviation of time measurements using 

```bash
sudo taskset -c 1 ./bin/<binary name> 
```

NOTE: For page fault time, you can run the helper script in the ```scripts/``` directory using

```bash
sudo python3 scripts/runpagefault.py
```

For memory access time benchmark run:
```bash
sudo ./scripts/runmemaccess.sh
```

## Network Benchmarks

Change the server IP in [netconfig.h](https://github.com/ucsd-cse221-fa24/cse221-team-4/blob/main/nw_benchmarks/netconfig.h) before building

Run the binary to get the measurements using

```bash
sudo taskset -c 1 ./bin/<binary name> 
```

## File System Benchmarks

**Warning**: You will need atleast 16GB of free disk space to use the temp files that are used for these benchmarks
Also, modify the mount point in [remote_fileread.cpp](https://github.com/ucsd-cse221-fa24/cse221-team-4/blob/main/fs_benchmarks/remote_fileread.cpp) for the remote 
file read tests before building

Run the binary to get the measurements as above
NOTE: For FS contention, you can run the helper script in the ```scripts/``` directory using

```bash
sudo ./scripts/runcontention.sh
```