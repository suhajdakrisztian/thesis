from pandas import *
import numpy
from matplotlib import pyplot
"""
data = read_csv("tests/performance_test_assets/find_if_results.csv")
pstl = data["PSTL"]
std = data["STD"]

data = read_csv("tests//performance_test_assets/find_if_seq_results.csv")
seq = data["STD"]


absmin = min(min(pstl), min(std), min(seq))
absmax = max(max(pstl), max(std), max(seq))

bins = numpy.linspace(absmin*0.9, absmax*1.1, 25)

pyplot.hist(pstl, bins, alpha=0.5, label='pstl runtimes (ms)')
pyplot.hist(std, bins, alpha=0.5, label='std runtimes (ms)')
pyplot.hist(seq, bins, alpha=0.5, label='std sequential runtimes (ms)')
pyplot.legend(loc='upper right')
pyplot.xlabel("Runtime (ms)")
pyplot.ylabel("Frequency")
pyplot.title("Runtime distributions for 100 thousand elements")
pyplot.show()

#pyplot.savefig('find_if_full.png')
"""



x_values = [1, 2, 5, 10, 20, 50, 100]

iota_threads= [2350.93,1305.36,685.69,657.278,725.069,762.729,609.724]
copy_backward_threads = [3527.73,87.7892,76.7908,73.2328,76.5689,74.3477,74.9334]
find_if_threads = [2.0038,1.2013,0.9511,1.5381,1.4193,3.2933,11.4691]
copy_if_threads = [3.8602,1.788,1.4871,3.2404,4.4008,6.2043,9.1791]

print(len(x_values), len(iota_threads))

#pyplot.plot(x_values, iota_threads, marker='x', color='blue')
#pyplot.title("pstl::iota Runtime With Different Number of Threads (ms)")

#pyplot.title("pstl::copy_backward Runtime With Different Number of Threads (ms)")
#pyplot.plot(x_values, copy_backward_threads, marker='x', color='blue')



#pyplot.plot(x_values, find_if_threads, marker='x', color='blue')
#pyplot.title("pstl::find_if Runtime With Different Number of Threads (ms)")


pyplot.plot(x_values, copy_if_threads,marker='x', color='blue', label="pstl::copy_if Runtime With Different Number of Threads (ms)")
pyplot.title("pstl::copy_if Runtime With Different Number of Threads (ms)")

pyplot.xticks([1,  5, 10, 50, 100])
pyplot.savefig("pstl::copy_if threads.png")
pyplot.show()