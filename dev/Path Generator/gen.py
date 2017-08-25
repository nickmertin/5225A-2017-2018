from itertools import chain
from pickle import load
from sys import argv, stdin

from math import atan, pi

from lib import frange

count = int(argv[1])
name = argv[2]
start = float(argv[3])
end = float(argv[4])
func = load(stdin.buffer)
derivative = func.derive()
print('float %sData[%d][3] =' % (name, count))
print('{')
print(',\n'.join('  { %f, %f, %f }' % (x, func.map(x), atan(derivative.map(x)) * 180 / pi + (180 if start > end else 0)) for x in
                 chain(frange(start, end, (end - start) / (count - 1)), [end])))
print('};')
print('sArmPath %s;' % name)
print('void %sInit()' % name)
print('{')
print('  %s.points = (sArmPath*) %s_data;' % (name, name))
print('  %s.pointsCount = %d;' % (name, count))
print('}')
