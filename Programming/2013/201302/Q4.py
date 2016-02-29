# coding: utf-8

import sys

from copy import deepcopy

op = '!()&+'

def parse(s):
	st = ['']
	sp = 0
	for i in xrange(len(s)):
		if s[i] == '(':
			st.append('')
			sp += 1
		if s[i] == ')':
			st[sp] += ')'
			tmp = st[sp]
			#ret.append(st.pop())
			sp -= 1
			st[sp] += tmp
		else:
			st[sp] += s[i]
	return st


def str2bool(st):
	l = len(st)
	for i in xrange(l-1, 0-1, -1):
		x = st[i-1]
		s = st[i]
		OR = []
		for or_lits in s[1:-1].split('|'):
			AND = []
			for and_lits in or_lits.split('&'):
				print and_lits
				for num in and_lits:
					if len(num) == 2:
						n = (int(num[1]) + 1) % 2
						AND.append(bool(n))
					else:
						AND.append(bool(int(num)))
			if all(AND):
				OR.append(True)
			else:
				OR.append(False)
		if any(OR):
			for j in xrange(i):
				st[j] = x.replace(s, '1')
		else:
			for j in xrange(i):
				st[j] = x.replace(s, '0')
		print st
	return bool(st[0])

if __name__ == '__main__':
	f = open(sys.argv[1], 'r')
	sentence = '(' + f.readline()[:-1] + ')'
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

	st = parse(sentence)
	print st

	bool_table = []
	for i in xrange(2**p):
		bool_table.append(format(i, 'b').zfill(p))

	res = []
	for bt in bool_table:
		x = deepcopy(st)
		for j in xrange(len(st)):
			for i in xrange(p):
				x[j] = x[j].replace(literals[i], bt[i])
		f = str2bool(x)
		res.append(f)
		if f:
			for i in xrange(p):
				print literals[i], '=', bool(int(bt[i])),
			print
	if not any(res):
		print 'none'
