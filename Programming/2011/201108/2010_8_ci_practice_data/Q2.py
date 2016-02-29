# coding: utf-8
import sys

nums = '0123456789'

if __name__ == '__main__':
	f = open(sys.argv[1])
	
	count = 0
	line = f.read()
	
	for ch in line:
		if ch in nums: count += 1

	print count / 3

	f.close()
