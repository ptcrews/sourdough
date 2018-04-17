import sys

data = [line.strip() for line in sys.stdin]
data = [data[k:k+3] for k in range(0, len(data), 3)]
def process(acc, x):
    return acc + [ (x[0], float(x[1])*1000/float(x[2])) ]
data = reduce(process, data, [])
for item in data: print item

