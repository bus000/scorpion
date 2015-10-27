#!/usr/bin/env python

import numpy
import matplotlib.pyplot as plt
import os

datafolder = '../data/'

sums = []
for datafile in sorted(os.listdir(datafolder)):
    print('handling', datafile)

    data = numpy.loadtxt(datafolder+datafile, delimiter=':', usecols=range(1,2))

    running_sum = 0
    for num in data:
        running_sum = running_sum + num

    sums.append((datafile, running_sum / len(data)))


for s in sums:
    plt.plot(s[0][:2], s[1], 'ro')
    print (s[0], s[1])

plt.axis([0, 45, 0, 1.6])
plt.savefig('foo.png')
