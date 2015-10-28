#!/usr/bin/env python

import numpy
import matplotlib.pyplot as plt
import os

# Distance, datafile
datafiles = []
datafiles.append({'dist': 5,  'name': '../data/05cm.dat'})
datafiles.append({'dist': 10, 'name': '../data/10cm.dat'})
datafiles.append({'dist': 15, 'name': '../data/15cm.dat'})
datafiles.append({'dist': 20, 'name': '../data/20cm.dat'})
datafiles.append({'dist': 25, 'name': '../data/25cm.dat'})
datafiles.append({'dist': 30, 'name': '../data/30cm.dat'})
datafiles.append({'dist': 35, 'name': '../data/35cm.dat'})
datafiles.append({'dist': 40, 'name': '../data/40cm.dat'})

sums = []
for datafile in datafiles:
    data = numpy.loadtxt(datafile.get('name'), delimiter=':', usecols=range(1,2))

    running_sum = 0
    for num in data:
        running_sum = running_sum + num

    sums.append((datafile.get('dist'), running_sum / len(data)))

for s in sums:
    plt.plot(s[0], s[1], 'ro')
    print (s[0], s[1])

plt.axis([0, 45, 0, 1.6])
plt.savefig('foo.png')
