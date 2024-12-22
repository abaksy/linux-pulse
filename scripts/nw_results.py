import subprocess
import numpy as np

command = "sudo taskset -c 1 ../bin/tcptest"

setup = list()
rtt = list()
teardown = list()

def getstats(arr):
    # q1 = np.percentile(arr, 25)
    q3 = np.percentile(arr, 90)
    # iqr = q3 - q1
    # # lower_bound = q1 - 1.5 * iqr
    upper_bound = q3

    trimmed_data = [x for x in arr if x <= upper_bound ]
    mean = np.mean(trimmed_data)
    stdev = np.std(trimmed_data)
    return (mean/2.6e6), (stdev/2.6e6)

for i in range(1000):
    out = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    data = out.stdout.decode().split(' ')
    print(data)
    # if i % 100 == 0:
    #     print(i)
    setup.append(int(data[0]))
    rtt.append(int(data[1]))
    teardown.append(int(data[2]))

print("Setup time:", getstats(setup))
print("RTT:", getstats(rtt))
print("Teardown time:", getstats(teardown))

'''
Local:
Setup time: (0.059686651282051285, 0.008635011328924294)
RTT: (1.2501188162393162, 0.14456568476819842)
Teardown time: (0.019121607692307693, 0.0019357747157078228)
'''
'''
Remote:
Setup time: (42.49553708888889, 7.117324879543257)
RTT: (43.53831811794872, 6.616160077529143)
Teardown time: (0.0662251905982906, 0.009452018782718655)
'''