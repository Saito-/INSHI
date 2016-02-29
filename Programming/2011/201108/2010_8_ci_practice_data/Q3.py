# coding: utf-8

import sys

from collections import OrderedDict

if __name__ == '__main__':
	f = open(sys.argv[1])

	dic = OrderedDict()
	line = f.read()
	idx = [0] * (len(line) - 6)

	for i in xrange(len(line) - 6):
		tmp = line[i:i+6]
		if not dic.has_key(tmp):
			dic[tmp] = str(i).zfill(3)
			idx[i] = str(i).zfill(3)
		else:
			idx[i] = dic.get(tmp)

	print len(dic)	
	f.close()
