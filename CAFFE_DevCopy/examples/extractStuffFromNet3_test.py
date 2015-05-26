import numpy as np
#import matplotlib.pyplot as plt
#matplotlib inline
#import Image

# Make sure that caffe is on the python path:
caffe_root = '../'  # this file is expected to be in {caffe_root}/examples
import sys
sys.path.insert(0, caffe_root + 'python')

import caffe

caffe.set_mode_cpu()
net = caffe.Net('/home/maurice/Desktop/MasterThesis/results/RGBDNIR_Caffe_prototxt/net_DNIR_10classes.prototxt', '/home/maurice/Desktop/MasterThesis/results/caffeModels_and_solverStates/DNIR_10cl_lr0.001_batch1_conv2sep_5__iter_2000000.caffemodel', caffe.TEST)

filters = net.params['conv1stNIR0_Y'][0].data

print filters.shape
i = 0
s = ""
for f in filters:
	# print '{}: scale 0 {}\n scale 1 {}\n scale 2 {}\n'.format(i, f, filters1[i], filters2[i])
	s += "kernel " + str(i) + ":\n"
	for ff in f: #filter kernel
		for line in ff: 
			for entry in line:		
				s += str(entry) + ","
	s += "\n"
	i+=1

filters = net.params['conv2nd_NIRScale0'][0].data

print filters.shape
counter = 0
counter2 = 0
for f in filters:
	print counter
	counter += 1
	s += "kernel " + str(i) + ":\n"
	cnt = 0
	for ff in f: #filter kernel
		print counter2
		print ff
		counter2 += 1
		for line in ff: 
			for entry in line:
				if cnt == 49: #because UV has two channels
					i+=1
					s += "\nkernel " + str(i) + ":\n"		
				s += str(entry) + ","
				cnt += 1
	s += "\n"
	i+=1

#write to file
txtfile = open("kernels.txt", "w")
txtfile.write(s)
txtfile.close()
print "kernels written to file"
