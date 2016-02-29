# coding: utf-8

class Ban:
	def __init__(self, field):
		if field is None:
			field = [
				['-', '-', '-'],
				['-', '-', '-'],
				['-', '-', '-'],
			]
		self.field = field

if __name__ == '__main__':
	f = [
		['O', 'X', '-'],
		['O', 'X', 'X'],
		['O', 'O', '-']
	]
	ban = Ban(f)
	for l in ban.field:
		for ch in l:
			print ch,
		print
