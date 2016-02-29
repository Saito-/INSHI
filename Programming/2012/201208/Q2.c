#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#define NO 1
#define KTYPE 16
#define POS_LEN 10

typedef struct {
	int x, y, dir;
	bool isOnMap;
} Target;

char map[15][9] = {
	{'|', '-', '-', '-', 'V', '-', '-', '-', '|'},
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
int Life = 5, Score = 0;

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
	
	print_map();

	while (1) {
		char ch = getchar();
		if (ch < 'i' || ch > 'l') continue;
		if (!tgt.isOnMap) {
			map[1][5] = 'O';
			tgt.x = 5; tgt.y = 1;
			tgt.dir = 1;
			tgt.isOnMap = true;
			print_map();
		} else {
			int x = tgt.x;
			int y = tgt.y;
			int nx = x + tgt.dir;
			if (nx > 7) {
				nx = x - 1;
				tgt.dir = -1;
			}
			if (nx < 1) {
				nx = x + 1;
				tgt.dir = 1;
			}
			int ny = y + 1;
			map[y][x] = ' ';
			if (ny > 13) {
				tgt.isOnMap = false;
				Life--;
			} else  
				map[ny][nx] = 'O';
			print_map();
			tgt.x = nx;
			tgt.y = ny;
		}
		if (Life == 0) break;
	}

	printf("Game Over\n");
	printf("Your Score: %d\n", Score);

	return 0;
}
	
