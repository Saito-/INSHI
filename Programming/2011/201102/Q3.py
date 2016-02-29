# coding: utf-8

import random

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
	
	def update_field(self, s):
		row = int(s[0]) - 1
		col = dic[s[1]]
		self.field[row][col] = te[self.turn % 2]
	
	def AI(self):
		while True:
			row = random.randint(0, 2)
			col = random.randint(0, 2)
			if self.field[row][col] != '-': continue
			self.field[row][col] = 'X'
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
	while True:
		print ban.turn
		for l in ban.field:
			for ch in l:
				print ch,
			print
		s = raw_input()
		ban.update_field(s)
		for l in ban.field:
			for ch in l:
				print ch,
			print
		if ban.judge(): break
		print ban.turn
		ban.AI()
		for l in ban.field:
			for ch in l:
				print ch,
			print
		if ban.judge(): break
