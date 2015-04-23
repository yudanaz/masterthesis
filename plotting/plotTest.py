import sys
import numpy as np
import matplotlib.pyplot as plot

def readData( fileName ):
	print fileName
	with open(fileName) as f:
		name = f.readline() #skip first line
		d = f.read()
	d = d.split('\n')
	d.pop() #remove last empty line	
	return {'data':d, 'name':name}

#read all files
argv = sys.argv
datas = [[]]
names = []
for i in range(1, len(argv)):
	data = readData(argv[i])
	names.append(data['name'])
	datas.append(data['data'])

#prepare plot
plot.figure(figsize=(12,6))
plot.xlabel('iterations')
plot.ylabel('loss')
plot.title('loss curve')

#plot all files
for d in datas:
	plot.plot(d, linewidth=1.0)

plot.legend(names)

plot.show()