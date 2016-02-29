#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
	int x, y, dir;
	bool isOnMap;
} Target;

typedef struct {
	int x, y;
	bool isOnMap;
} Bullet;

char map[15][9] = {
	{'|', '-', '-', '-', '-', '-', '-', '-', '|'},
	{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
	{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
	{'|', '.', '.', '.', '.', '.', '.', '.', '|'},
};

Target tgt = { 0, 0, 0, false };
Bullet b1 = { 0, 0, false };
Bullet b2 = { 0, 0, false };

int Life, Score, X, V;

int randn(int min, int max)
{
	return min + (int)(rand()*(max - min + 1.0)/(1.0 + RAND_MAX));
}

void init()
{
	X = 4;
	V = randn(1, 7);
	map[0][V] = 'V';
	map[14][X] = 'X';
	Life = 5;
	Score = 0;
}

void move_generator()
{
	map[0][V] = '-';
	V = randn(1, 7);
	map[0][V] = 'V';
}

void generate_tgt()
{
	tgt.x = V + 1; 
	tgt.y = 1;
	tgt.dir = 1;
	tgt.isOnMap = true;
	map[tgt.y][tgt.x] = 'O';
}

void update_tgt()
{
	int x = tgt.x;
	int y = tgt.y;
	if (y == 13) {
		map[y][x] = (x == 0 || x == 8) ? '|' : ' ';
		move_generator();
		tgt.isOnMap = false;
		Life--;
		return;
	}
	int nx, ny;
	ny = y + 1;
	if (x == 0 || x == 8) {
		map[y][x] = '|';
		tgt.dir *= -1;
	} else {
		map[y][x] = ' ';
	}
	nx = x + tgt.dir;
	map[ny][nx] = 'O';
	tgt.x = nx;
	tgt.y = ny;
}

void update_bullets()
{
	int x, y;
	if (b1.isOnMap) {
		x = b1.x;
		y = b1.y;
		map[y][x] = ' ';
		if (b1.y == 1) {
			b1.isOnMap = false;
		} else {
			map[--y][x] = 'e';
			b1.y = y;
		} 
	}
	if (b2.isOnMap) {
		x = b2.x;
		y = b2.y;
		map[y][x] = ' ';
		if (b2.y == 1) {
			b2.isOnMap = false;
		} else {
			map[--y][x] = 'e';
			b2.y = y;
		} 
	}
}

void move_left()
{
	if (X > 1) {
		map[14][X] = '.';
		map[14][--X] = 'X';
	}
}

void move_right()
{
	if (X < 7) {
		map[14][X] = '.';
		map[14][++X] = 'X';
	}
}

void shoot()
{
	if (!b1.isOnMap) {
		b1.x = X;
		b1.y = 13;
		b1.isOnMap = true;
		map[13][X] = 'e';
		return;
	}
	if (!b2.isOnMap) {
		b2.x = X;
		b2.y = 13;
		b2.isOnMap = true;
		map[13][X] = 'e';
		return;
	}
}

void collision()
{
	if (b1.isOnMap) {
		if (b1.x == tgt.x && b1.y == tgt.y) {
			map[tgt.y][tgt.x] = ' ';
			map[b1.y][b1.x] = ' ';
			b1.isOnMap = false;
			tgt.isOnMap = false;
			Score++;
			move_generator();
		}
	}
	if (b2.isOnMap) {
		if (b2.x == tgt.x && b2.y == tgt.y) {
			map[tgt.y][tgt.x] = ' ';
			map[b2.y][b2.x] = ' ';
			b2.isOnMap = false;
			tgt.isOnMap = false;
			Score++;
			move_generator();
		}
	}
}

void print_map()
{
	int i, j;
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 9; j++) {
			putchar(map[i][j]);
		}
		putchar('\n');
	}
}

int main()
{
	int i, j;
	srand(time(NULL));
	
	init();
	
	print_map();

	while (1) {
		char ch = getchar();
		if (ch < 'i' || ch > 'l') continue;
		if (!tgt.isOnMap) {
			generate_tgt();
			update_bullets();
			if (ch == 'j')
				move_left();
			if (ch == 'i')
				shoot();
			if (ch == 'l')
				move_right();
			collision();
		} else {
			update_tgt();
			update_bullets();
			if (ch == 'j')
				move_left();
			if (ch == 'i')
				shoot();
			if (ch == 'l')
				move_right();
			collision();
		}
		print_map();
		if (Life == 0) break;
	}

	printf("Game Over\n");
	printf("Your Score: %d\n", Score);

	return 0;
}
	
