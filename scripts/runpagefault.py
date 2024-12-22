import subprocess
import statistics

runtimes = list()


for _ in range(20):
    op = subprocess.run("sudo taskset -c 1 ./bin/pagefault", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    cycles = float(op.stdout.decode().split()[-1])
    print(cycles)
    runtimes.append(cycles)

avg = statistics.mean(runtimes)
stdev = statistics.stdev(runtimes)

avg_time = avg*0.3858
print(f"Average Cycles: {avg} Average Runtime: {avg_time} ns, Standard Deviation: {stdev}")
