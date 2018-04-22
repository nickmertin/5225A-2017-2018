from sys import stdin, stderr

timestamp = 0
data = None

for line in stdin:
    cols = line.split(',')
    # print(repr(cols), file=stderr)
    if 'R' in cols[0]:
        continue
    elif cols[2] == ' ':
        timestamp = float(cols[1])
        data = (cols[3], cols[4], cols[6], cols[7])
    else:
        print(','.join(str(c).strip() for c in
                       [(timestamp + float(cols[1])) / 2, cols[2], data[0], data[1], cols[5], data[2], data[3]]), sep='')
