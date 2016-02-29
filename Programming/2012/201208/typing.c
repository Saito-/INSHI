#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

#define NO 1
#define KTYPE 16
#define POS_LEN 10

typedef enum { Term, KeyPos, KeyPosComp, Clang, Conversation, InValid } Menu;

char map[16][10] = {
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
}

char *kstr[] = {
	"12345", "67890-^\\",
	"!\"#$%", "&'()=~|",
	"qwert", "yuiop@[",
	"QWERT", "YUIOP`{",
	"asdfg", "hjkl;:]",
	"ASDFG", "HJKL+*}",
	"zxcvb", "nm,./",
	"ZXCVB", "NM<>?",
};

char *cstr[] = {
	"auto", "break", "case", "char", "const", "continue",
	"default", "do", "double", "else", "enum", "extern",
	"float", "for", "goto", "if", "int", "long",
	"register", "return", "short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union", "unsigned", "void",
	"volatile", "while",
	"abort", "abs", "acos", "asctime", "asin", "assert",
	"atan", "atan2", "atexit", "atof", "atoi", "atol",
	"bsearch", "calloc", "ceil", "clearerr", "clock", "cos",
	"cosh", "ctime", "difftime", "div", "exit", "exp",
	"fabs", "fclose", "feof", "ferror", "fflush", " fgetc",
	"fgetpos", "fgets", "floor", "fmod", "fopen", "fprintf",
	"fputc", "fputs", "fread", "free", "freopen", "frexp",
	"fscanf", "fseek", "fsetpos", "ftell", "fwrite", "getc",
	"getchar", "getenv", "gets", "gmtime", "isalnum", "isalpha",
	"iscntrl", "isdigit", "isgraph", "islower", "isprint", "ispunct",
	"isspace", "isupper", "isxdigit", "labs", "ldexp", "ldiv",
	"localconv", "localtime", "loc", "log10", "longjmp",
	"malloc", "memchr", "memcmp", "memcpy", "memmove", "memset",
	"mktime", "modf", "perror", "pow", "printf", "putc",
	"putchar", "puts", "qsort", "raise", "rand", "realloc",
	"remove", "rename", "rewind", "scanf", "setbuf", "setjmp",
	"setlocale", "setvbuf", "signal", "sin", "sinh", "sprintf",
	"sqrt", "srand", "sscanf", "strcat", "strchr", "strcmp",
	"strcoll", "strcpy", "strcspn", "strerror", "strftime", "strlen",
	"strncat", "strncmp", "strncpy", "strpbrk", "strrchr", "strspn",
	"strstr", "strtod", "strtok", "strtol", "strtoul", "strxfrm",
	"system", "tan", "tanh", "time", "tmpfile", "tmpnam",
	"tolower", "toupper", "ungetc", "va_arg", "va_end", "va_start",
	"vfprintf", "vprintf", "vsprintf"
};

char *vstr[] = {
	"Hello!",
	"How are you?",
	"Fine thanks.",
	"I can't complain, thanks.",
	"How do you do?",
	"Good bye!",
	"Good morning!",
	"Good afternoon!",
	"Good evening!",
	"See you later!",
	"Go ahead, Please.",
	"Thank you.",
	"No, thank you.",
	"May I have your name?",
	"I'm glad to meet you.",
	"What time is it now?",
	"It's about seven.",
	"I must go now.",
	"How much?",
	"Where is the restroom?",
	"Excuse me.",
	"Excuse us.",
	"I'm sorry.",
	"I don't know.",
	"I have no change with me.",
	"I will be back.",
	"Are you going out?",
	"I hope I'm not disturbing you.",
	"I'll offer no excuse.",
	"Shall we dance?",
	"Will you do me a favor?",
	"It's very unseasonable.",
	"You are always welcome.",
	"Hold still!",
	"Follow me.",
	"Just follow my lead.",
	"To be honest with you.",
};

int go(const char* str)
{
	int i;
	int len = strlen(str);
	int mistake = 0;

	for (i = 0; i < len; i++) {
		printf(" %s \r", &str[i]);
		fflush(stdout);
		while (getch() != str[i]) mistake++;
	}
	return mistake;
}

void pos_training()
{
	int i;
	int stage;
	int temp, line;
	int len;
	int qno, pno;
	int tno, mno;
	time_t start, end;

	printf("\n単純ポジショントレーニング\r\n");
	printf("練習するブロックを選んでね\r\n");
	printf("第 1 段 (1) 左 %-8s   (2) 右 %-8s\r\n", kstr[0], kstr[1]);
	printf("第 2 段 (3) 左 %-8s   (4) 右 %-8s\r\n", kstr[4], kstr[5]);
	printf("第 3 段 (5) 左 %-8s   (6) 右 %-8s\r\n", kstr[8], kstr[9]);
	printf("第 4 段 (7) 左 %-8s   (8) 右 %-8s\r\n", kstr[12], kstr[13]);;
	
	do {
		printf("番号 (99 で中止): ");
		fflush(stdout);
		temp = getch() - '0';
		printf("%d\r\n", temp);
		if (temp == 99) return;
	} while (temp < 1 || temp > 8);


	line = 4 *((temp - 1) /2) + (temp - 1) % 2;

	printf("%s の問題を %d 回練習します\r\n", kstr[line], NO);

	printf("スペースキーで開始します\r\n");
	fflush(stdout);
	while (getch() != ' ');

	tno = mno = 0;
	len = strlen(kstr[line]);

	start = time(NULL);

	for (stage = 0; stage < NO; stage++) {
		char str[POS_LEN + 1];

		for (i = 0; i < POS_LEN; i++) 
			str[i] = kstr[line][rand() % len];
		str[i] = '\0';
		
		mno += go(str);
		tno += strlen(str);
	}

	end = time(NULL);

	printf("問題: %d 文字/ミス: %d 回\r\n", tno, mno);
	printf("経過時間: %.f 秒\r\n", difftime(end, start));
	fflush(stdout);
}

void pos_training2()
{
	int i;
	int stage;
	int temp, line;
	int sno;
	int select[KTYPE];
	int len[KTYPE];
	int tno, mno;
	time_t start, end;
	char* format = "第 %d 段 (%2d) 左 %-8s (%2d) 右 %-8s "
		"(%2d)[左] %-8s (%2d)[右] %-8s\r\n";
	
	printf("\n複合ポジショントレーニング\r\n");
	printf("練習するブロックを選んでね (複数可)\r\n");

	for (i = 0; i < 4; i++) {
		int k = i * 4;
		printf(format, i+1, k+1, kstr[k], k+2, kstr[k+1],
			k+3, kstr[k+2], k+4, kstr[k+3]);
	}

	fflush(stdout);
		
	sno = 0;
	while (1) {
		printf("\r番号 (0 で選択終了, スペースキーで中止): \r\n");
		fflush(stdout);
		do {
			temp = getch() - '0';
			fflush(stdout);
			if (temp == -16) return;
		} while ((temp < 0 || temp > KTYPE) && temp != 0);

		if (temp == 0) break;
		for (i = 0; i < sno; i++) 
			if (temp == select[i]) {
				printf("\rその段は既に選ばれてる\r\n");
				break;
			}
		if (i == sno)
			select[sno++] = temp;
	}
	printf("以下のブロックの問題を %d 回練習します\r\n",  NO);

	for (i = 0; i < sno; i++) 
		printf("%s ", kstr[select[i] - 1]);

	printf("スペースキーで開始します\n\r");
	fflush(stdout);
	while (getch() != ' ');

	tno = mno = 0;
	for (i = 0; i < sno; i++) 
		len[i] = strlen(kstr[select[i] - 1]);

	start = time(NULL);

	for (stage = 0; stage < NO; stage++) {
		char str[POS_LEN + 1];

		for (i = 0; i < POS_LEN; i++) {
			int q = rand() % sno;
			str[i] = kstr[select[q]-1][rand() % len[q]];
		}
		str[i] = '\0';
		
		mno += go(str);
		tno += strlen(str);
	}

	end = time(NULL);

	printf("問題: %d 文字/ミス: %d 回\n\r", tno, mno);
	printf("経過時間: %.f 秒\n\r", difftime(end, start));
}

void word_training(const char *mes, const char *str[], int n)
{
	int stage;
	int qno, pno;
	int tno, mno;
	time_t start, end;

	printf("\n%s を %d 個練習します\n\r", mes, NO);

	printf("スペースキーで開始します\n\r");
	fflush(stdout);
	while (getch() != ' ');

	tno = mno = 0;
	pno = n;

	start = time(NULL);

	for (stage = 0; stage < NO; stage++) {
		do {
			qno = rand() % n;
		} while (qno == pno);
	
		mno += go(str[qno]);
		tno += strlen(str[qno]);
		pno = qno;
	}

	end = time(NULL);

	printf("問題: %d 文字/ミス: %d 回\n\r", tno, mno);
	printf("経過時間: %.f\n\r", difftime(end, start));
	fflush(stdout);
}

int SelectMenu()
{
	int ch;

	do {
		printf("\n練習を選択してね\n\r");
		printf("(1) 単純ポジション (2) 複合ポジション\n\r");
		printf("(3) C言語の単語    (4) 英会話  (0) 終了\n\r");
		fflush(stdout);
		ch = getch() - '0';
		fflush(stdout);
	} while (ch < Term || ch >= InValid);

	return (Menu)ch;
}

int main()
{
	Menu menu;
	int cn = sizeof(cstr) / sizeof(cstr[0]);
	int vn = sizeof(vstr) / sizeof(vstr[0]);

	initscr();
	cbreak();
	noecho();
	refresh();

	srand(time(NULL));

	do {
		switch (menu = SelectMenu()) {
		case KeyPos:
			pos_training();
			break;
		case KeyPosComp:
			pos_training2();
			break;
		case Clang:
			word_training("C言語の単語", cstr, cn);
			break;
		case Conversation:
			word_training("英会話", vstr, vn);
			break;
		}
	} while (menu != Term);
	
	printf("スペースキーで終了します\n\r");
	fflush(stdout);
	while (getch() != ' ');

	endwin();

	return 0;

}
	
