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
print('sArmPath g%s;' % name.title())
print('void %sInit()' % name)
print('{')
print('  static float data[%d][3] =' % count)
print('  {')
print(',\n'.join('    { %f, %f, %f }' % (x, func.map(x), atan(derivative.map(x)) * 180 / pi + (180 if start > end else 0)) for x in
                 chain(frange(start, end, (end - start) / (count - 1)), [end])))
print('  };')
print('  g%s.points = (sArmPath*) data;' % name.title())
print('  g%s.pointsCount = %d;' % (name.title(), count))
print('}')
