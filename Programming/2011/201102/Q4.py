# coding: utf-8

import random
import time

dic = { 'A':0, 'B':1, 'C':2 }
te = { 0:'O', 1:'X' }


class Ban:
	def __init__(self, field=None, turn=0):
		if field is None:
			field = [
				['-', '-', '-'],
				['-', '-', '-'],
				['-', '-', '-'],
			]
		self.field = field
		self.turn = turn
	
	def AI(self):
		key = te[self.turn % 2]
		while True:
			row = random.randint(0, 2)
			col = random.randint(0, 2)
			if self.field[row][col] != '-': continue
			self.field[row][col] = key
			break

	def judge(self):
		key = te[self.turn % 2]
		for i in xrange(3):
			count = 0
			for j in xrange(3):
				if self.field[i][j] != key: break
				count += 1
			if count == 3:
				print key, 'win'
				return True

		for j in xrange(3):
			count = 0
			for i in xrange(3):
				if self.field[i][j] != key: break
				count += 1
			if count == 3:
				print key, 'win'
				return True
		
		count = 0
		for i in xrange(3):
			if self.field[i][i] != key: break
			count += 1
		if count == 3:
			print key, 'win'
			return True

		count = 0
		for i in xrange(3):
			if self.field[i][2-i] != key: break
			count += 1
		if count == 3:
			print key, 'win'
			return True
		self.turn += 1
		if self.turn == 9:
			print 'draw'
			return True
		return False

if __name__ == '__main__':
	ban = Ban()
	print ban.turn
	for l in ban.field:
		for ch in l:
			print ch,
		print
	while True:
		print ban.turn
		time.sleep(1)
		ban.AI()
		for l in ban.field:
			for ch in l:
				print ch,
			print
		if ban.judge(): break
