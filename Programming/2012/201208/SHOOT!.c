#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define	WIDTH	8
#define	HEIGHT	14
#define	MAX(a, b)	(((a) > (b)) ? (a) : (b))

int randomAGE(int min, int max);
void openDeal();

int main(int argc, char *argv[])
{
	// Curses
	setlocale(LC_ALL, "");
	initscr();
	start_color();
	use_default_colors();

	openDeal();
	endwin();

	return 0;
}

int randomAGE(int min, int max)
{
	static int flg = 0;

	if (flg == 0) {
		srand(time(NULL));
		flg = 1;
		rand();
	}

	if (min == 0) max++;
	return (min + (int)((double)max * rand() / (RAND_MAX + 1.0)));
}


void openDeal()
{
	// Curses
	keypad(stdscr, TRUE);
	leaveok(stdscr, TRUE);
	curs_set(0);
	noecho();
	cbreak();
	timeout(500);

	// 情報
	int mt[] = {-1, -1, 0};	// {x,y,direction}	まと
	int tm1[] = {-1, -1};	// {x,y}			たま
	int tm2[] = {-1, -1};
	int vpoint = 0;
	int xpoint = randomAGE(0, WIDTH-1);
	int score = 0;
	int life = 5;

	// 本体
	int i, j;
	int width, height;	
	while (1) {
		erase();
		getmaxyx(stdscr, height, width);
		int x = (width - 3*(WIDTH+1)) / 2;
		int y = (height - 2*(HEIGHT+1)) / 2;
		int yanmar = (x < 0 || (y - 1) < 0);
		
		if (yanmar) {
			move(MAX((height-1)/2, 0), MAX((width-8)/2, 0));
			addstr("DAMEDESU");
			addstr(str);
			refresh();
			continue;
		}
		
		
		// 描画
		for (i=0; i<WIDTH+1; i++) {
			move(y, x+(3*i));
			vline(0, 2*HEIGHT+1);
		}
		
		for (j=0; j<HEIGHT+1; j++) {
			move(y+2*j, x);
			hline(0, 3*WIDTH+1);
		}
		
		for (i=0; i<WIDTH+1; i++) {
			for (j=0; j<HEIGHT+1; j++) {
				move(y+(2*j), x+(3*i));
				addch(
					(i == 0) ? ACS_LTEE :
					(i == WIDTH) ? ACS_RTEE :
					(j == 0) ? ACS_TTEE :
					(j == HEIGHT) ? ACS_BTEE :
					ACS_PLUS);
			}
		}
		
		mvaddch(y, x, ACS_ULCORNER);
		mvaddch(y, x+(3*WIDTH), ACS_URCORNER);
		mvaddch(y+(2*HEIGHT), x, ACS_LLCORNER);
		mvaddch(y+(2*HEIGHT), x+(3*WIDTH), ACS_LRCORNER);

		if (tm1[0] != -1) mvaddch(y+(2*tm1[1])+1, x+(3*tm1[0])+1, ACS_BULLET);
		if (tm2[0] != -1) mvaddch(y+(2*tm2[1])+1, x+(3*tm2[0])+1, ACS_BULLET);
		if (mt[0] != -1) mvaddch(y+(2*mt[1])+1, x+(3*mt[0])+1, ACS_DIAMOND);
		mvaddch(y, x+(3*vpoint)+1, ACS_DARROW);
		mvaddch(y+(2*HEIGHT), x+(3*xpoint)+1, ACS_UARROW);
		
		move(y-1, x);
		printw("Score: %d", score);
		
		move(y+(2*HEIGHT)+1, x);
		if (life > 0) {
			printw("Life: %d", life);
		} else {
			printw("Game Over");
			refresh();
			break;
		}
		
		// 指示待ち
		int key = getch();
		if (key == 'i' && (tm1[0] == -1 || tm2[0] == -1)) {
			int *tm = (tm1[0] == -1) ? tm1 : tm2;
			tm[0] = xpoint;
			tm[1] = HEIGHT;
		} else if (key == 'j' && xpoint > 0) {
			xpoint--;
		} else if (key == 'l' && xpoint < (WIDTH - 1)) {
			xpoint++;
		}
		
		// 的発射
		vpoint = randomAGE(0, WIDTH-1);
		if (mt[0] == -1) {
			mt[0] = vpoint;
			mt[1] = -1;
			mt[2] = 1;
		}
		
		// 進める
		if (tm1[0] != -1) tm1[1]--;
		if (tm2[0] != -1) tm2[1]--;
		if (mt[0] != -1) {
			if (mt[0] == (WIDTH - 1)) mt[2] = -1;
			if (mt[0] == 0) mt[2] = 1;
			mt[0] += mt[2];
			mt[1]++;
		}
		
		// 消す
		if (mt[0] != -1) {
			if (mt[0] == tm1[0] && mt[1] == tm1[1]) {
				tm1[0] = -1;
				mt[0] = -1;
				score++;
			} else if (mt[0] == tm1[0] && mt[1] == tm1[1]) {
				tm2[0] = -1;
				mt[0] = -1;
				score++;
			}
		}
		if (tm1[1] == -1) tm1[0] = -1;
		if (tm2[1] == -1) tm2[0] = -1;
		if (mt[1] == HEIGHT) {
			mt[0] = -1;
			life--;
		}

		refresh();
	}
}
