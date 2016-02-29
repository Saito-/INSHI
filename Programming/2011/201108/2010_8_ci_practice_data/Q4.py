# coding: utf-8

import sys

from collections import OrderedDict
from copy import deepcopy

if __name__ == '__main__':
	f = open(sys.argv[1])

	dic = OrderedDict()
	line = f.read()

	for i in xrange(len(line) - 6):
		tmp = line[i:i+6]
		if not dic.has_key(tmp):
			dic[tmp] = str(i).zfill(3)

	comptxt = deepcopy(line)

	for i in xrange(len(line) - 6):
		if i >= len(comptxt): break
		tmp = comptxt[i:i+6]
		key = dic.get(tmp)
		if key is None: continue
		for j in xrange(i+1, len(line) - 6):
			tmp2 = comptxt[j:j+6]
			key2 = dic.get(tmp2)
			if key2 is None: continue
			if key == key2:
				#print 'replace', tmp, 'at', j, 'to', key
				pre = comptxt[:j]
				post = comptxt[j:].replace(tmp2, key, 1)
				comptxt = pre + post
	print comptxt
	print len(comptxt)
	print comptxt[-10:]
	f.close()
