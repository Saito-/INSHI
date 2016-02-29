# coding: utf-8

import sys

op = '&+'

def str2bool(s):
	x = s.split('|')
	l = []
	for and_lit in x:
		tmp = []
		lit = and_lit.split('&')
		for num in lit:
			tmp.append(int(num))
		if all(tmp):
			l.append(1)
		else:
			l.append(0)
	if any(l):
		return True
	else:
		return False

if __name__ == '__main__':
	f = open(sys.argv[1], 'r')
	sentence = f.readline()[:-1]
	f.close()

	literals = [] 	 
	for ch in sentence:
		if ch in op: 
			continue
		if not ch in literals: 
			literals.append(ch)
	
	literals.sort()
	p = len(literals)
	sentence = sentence.replace('+', '|')
	print sentence

	bool_table = []
	for i in xrange(2**p):
		bool_table.append(format(i, 'b').zfill(p))
	
	for bt in bool_table:
		x = sentence
		for i in xrange(p):
			x = x.replace(literals[i], bt[i])
		if str2bool(x):
			for i in xrange(p):
				print literals[i], '=', bool(int(bt[i])),
			print

