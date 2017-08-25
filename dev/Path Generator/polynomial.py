from sys import stdout, argv

from pickle import dumps

from lib import Polynomial

stdout.buffer.write(dumps(Polynomial([float(x) for x in argv[1:]])))
