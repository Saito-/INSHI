# coding: utf-8

import sys

from collections import OrderedDict
from copy import deepcopy

num = '0123456789'

if __name__ == '__main__':
	f = open(sys.argv[1])

	dic = OrderedDict()
	line = f.read()

	s = deepcopy(line)

	print line

	i = 0
	while True:
		if i == len(s) - 6: break
		tmp = s[i]
		for j in xrange(i+1, i+6):
			if s[j] in num:
				key = s[j:j+3]
				val = dic.get(key)
				if val is None:
					rep = s[int(key):int(key)+(j - int(key))]
					new_val = rep * (6 / len(rep))
					dic[key] = new_val
					tmp += rep
					break
				#print 'replace', key, 'at', j, 'to', val
				pre = s[:j]
				post = s[j:].replace(key, val, 1)
				s = pre + post
				tmp += s[j]
				break
			tmp += s[j]
		key = str(i).zfill(3)
		dic[key] = tmp
		i += 1
		#print 'add', key, tmp, 'to dic'
	
	print s
	print len(s)
	print s[-10:]

	f.close()
