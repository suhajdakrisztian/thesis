from pandas import *
import numpy
from matplotlib import pyplot

data = read_csv("tests/find_if_results.csv")
pstl = data["PSTL"]
std = data["STD"]


absmin = min(min(pstl), min(std))
absmax = max(max(pstl), max(std))

bins = numpy.linspace(absmin*0.9, absmax*1.1, 25)

pyplot.hist(pstl, bins, alpha=0.5, label='pstl runtimes (ms)')
pyplot.hist(std, bins, alpha=0.5, label='std runtimes (ms)')
pyplot.legend(loc='upper right')
pyplot.xlabel("Runtime (ms)")
pyplot.ylabel("Frequency")
pyplot.title("Runtime distributions for 100 thousand elements")
#pyplot.show()

pyplot.savefig('find_if.png')
