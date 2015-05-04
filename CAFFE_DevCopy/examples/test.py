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
net = caffe.Net('/home/maurice/Data/RGBDNIR_models_1MioIt/net_NIR_3classes.prototxt', '/home/maurice/Data/RGBDNIR_models_1MioIt/NIR_3cl__iter_1000000.caffemodel', caffe.TEST)

# for p in net.params:
# 	print(p) #layer name
# 	print(net.params[p][0].data.shape)
# 	print(net.params[p][1].data.shape)

#print net parameters and their shape
# for k, v in net.params.items():
# 	print (k, v[0].data.shape)

# filters = net.params['conv2nd_NIRScale0'][0].data
# filters1 = net.params['conv2nd_NIRScale1'][0].data
# filters2 = net.params['conv2nd_NIRScale2'][0].data
# biases =  net.params['conv2nd_NIRScale0'][1].data

# print 'scale 0 filter [0,0]{}'.format(filters[0][0])
# print 'scale 1 filter [0,0]{}'.format(filters1[0][0])
# print 'scale 2 filter [0,0]{}'.format(filters2[0][0])

# cntf = 0
# for f in filters:
# 	cntf += 1
# 	print 'filter nr: {} \n {}'.format(cntf, f)
# cntb = 0
# for b in biases:
# 	cntb += 1
# 	print 'bias nr: {} \n {}'.format(cntb, b)

# print 'filters: {}'.format(cntf)
# print 'biases: {}'.format(cntb)

# ip2_w = net.params['ip1'][0].data;
# ip2_b = net.params['ip1'][1].data;
# print ip2_w
# print ip2_b

# filters = net.params['conv1stNIR0_Y'][0].data
# filters1 = net.params['conv1stNIR1_Y'][0].data
# filters2 = net.params['conv1stNIR2_Y'][0].data
# biases =  net.params['conv1stNIR0_Y'][1].data
# biases1 =  net.params['conv1stNIR1_Y'][1].data
# biases2 =  net.params['conv1stNIR2_Y'][1].data

filters = net.params['conv2nd_NIRScale0'][0].data
biases = net.params['conv2nd_NIRScale0'][1].data
# filters = net.params['conv3rdScale0'][0].data
# biases = net.params['conv3rdScale0'][1].data
# filters = net.params['ip2'][0].data
# biases = net.params['ip2'][1].data

print filters.shape
i = 0
for f in filters:
	# print '{}: scale 0 {}\n scale 1 {}\n scale 2 {}\n'.format(i, f, filters1[i], filters2[i])
	print i
	ii = 0
	for ff in f:
		print ii
		ii +=1 
	i+=1

print biases.shape
i = 0
for b in biases:
 	# print '{}: scale 0: {}\n scale 1: {}\n scale 2: {}\n'.format(i, b, biases1[i], biases2[i])
	print i
	print b
	i+=1