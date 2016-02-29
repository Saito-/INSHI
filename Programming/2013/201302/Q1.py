# coding: utf-8

import sys

if __name__ == '__main__':
	f = open(sys.argv[1], 'r')
	line = f.readline()[:-1].split('+')
	f.close()
	for rit in line:
		print rit
