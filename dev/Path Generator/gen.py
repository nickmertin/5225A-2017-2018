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
print('  static sArmPathPoint data[%d];' % count)
data = [(x, func.map(x), atan(derivative.map(x)) * 180 / pi + (180 if start > end else 0)) for x in
        chain(frange(start, end, (end - start) / (count - 1)), [end])]
for i in range(count):
    print('  data[%d].pos.x = %f;' % (i, data[i][0]))
    print('  data[%d].pos.y = %f;' % (i, data[i][1]))
    print('  data[%d].direction = %f;' % (i, data[i][2]))
print('  g%s.points = &data;' % name.title())
print('  g%s.pointsCount = %d;' % (name.title(), count))
print('}')
