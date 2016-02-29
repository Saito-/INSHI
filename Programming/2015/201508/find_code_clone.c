#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Q1(char* fname)
{
	int count = 0;
	char tmp;
	FILE* fp;
	if ((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "File Open Error\n");
		return -1;
	}

	while ((tmp = fgetc(fp)) != EOF) {
		if (tmp == ';') count++;
	}

	fclose(fp);
	return count;
}

void Q2(char* fname)
{
	FILE* fp;
	char pattern[] = "main";
	char str[1024];
	char* ptr;
	int line = 0;

	if((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "File Open Error\n");
		return;
	}
	
	while (fgets(str, sizeof(str), fp) != NULL) {
		line++;
		if ((ptr = strstr(str, pattern)) != NULL) {
			printf("%d: %s", line, str);
		}
	}
	fclose(fp);
}

void Q3(char* fname)
{
	FILE* fp;
	char str1[1024], str2[1024];

	if((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "File Open Error\n");
		return;
	}
	
	fgets(str1, sizeof(str1), fp);
	if (str1 == NULL) return;
	
	while (fgets(str2, sizeof(str2), fp) != NULL) {
		if (strcmp(str1, str2) == 0) 
			printf("%s", str1);
		strcpy(str1, str2);
	}

	fclose(fp);
}

void Q4(char* fname) 
{
	FILE* fp;
	char str1[1024], str2[1024];
	char** jufuku = NULL;
	int lines = 0, current_pos = 0, n_jufuku = 0, is_exist;
	int i, j, k;

	if((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "File Open Error\n");
		return;
	}
	
	
	while (fgets(str1, sizeof(str1), fp) != NULL) {
		lines++;
	}

	for (i = 0; i < lines; i++) {
		is_exist = 0;
		current_pos++;
		fp = fopen(fname, "r");
		for (j = 0; j < current_pos; j++) {
			fgets(str1, sizeof(str1), fp);
		}
		for (j = current_pos; j < lines; j++) {
			fgets(str2, sizeof(str2), fp);
			if (strcmp(str1, str2) == 0) {
				for (k = 0; k < n_jufuku; k++) {
					if (strcmp(str2, jufuku[k]) == 0) {
						is_exist = 1;
						break;
					}
				}
				if (is_exist == 0) {
					jufuku = (char**)realloc(jufuku, sizeof(char*)*(n_jufuku+1));
					jufuku[n_jufuku] = (char*)malloc(sizeof(char)*(strlen(str1)+1));
					strcpy(jufuku[n_jufuku], str1);
					n_jufuku++;
					break;
				}
			}
		}
	}

	for (i = 0; i < n_jufuku; i++) {
		printf("%s", jufuku[i]);
		free(jufuku[i]);
	}

	free(jufuku);
	fclose(fp);
}

void Q5(char* fname) 
{
	FILE* fp;
	char str1[1024], str2[1024];
	char*** ruiji = { NULL };
	int lines = 0, current_pos = 0, n_ruiji = 0, is_exist;
	int i, j, k;

	if((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "File Open Error\n");
		return;
	}
	
	while (fgets(str1, sizeof(str1), fp) != NULL) {
		lines++;
	}

	for (i = 0; i < lines; i++) {
		is_exist = 0;
		current_pos++;
		fp = fopen(fname, "r");
		for (j = 0; j < current_pos; j++) {
			fgets(str1, sizeof(str1), fp);
		}
		int len1 = strlen(str1);
		//if (len1 < 20) continue;
		for (j = current_pos; j < lines; j++) {
			fgets(str2, sizeof(str2), fp);
			int len2 = strlen(str2);
			int len;
			//if (len2 < 20) continue;
			if (len1 < len2) {
				for (k = len1-1; k < len2-1; k++) {
					str1[k] = ' ';
				}
				str1[len2-1] = '\n';
				str1[len2] = '\0';
				len = len2;
			} else if (len1 > len2) {
				for (k = len2-1; k < len1-1; k++) {
					str2[k] = ' ';
				}
				str2[len1-1] = '\n';
				str2[len1] = '\0';
				len = len1;
			}
			int cmp = 0;;
			for (k = 0; k < len; k++) {
				if (str1[k] != str2[k]) cmp++;
			}
			//printf("cmp: %d\n", cmp);
			if (cmp > 0 && cmp < 5) {
				for (k = 0; k < n_ruiji; k++) {
					int cmp1_0 = strcmp(str1, ruiji[k][0]);
					int cmp1_1 = strcmp(str1, ruiji[k][1]);
					int cmp2_0 = strcmp(str2, ruiji[k][0]);
					int cmp2_1 = strcmp(str2, ruiji[k][1]);
					if ((cmp1_0 == 0 && cmp2_1 == 0) || (cmp1_1 == 0 && cmp2_0 == 0)) {
						is_exist = 1;
					}
				}
				if (is_exist == 0) {
					ruiji = (char***)realloc(ruiji, sizeof(char**)*(n_ruiji+1));
					ruiji[n_ruiji] = (char**)malloc(sizeof(char*)*2);
					ruiji[n_ruiji][0] = NULL;
					ruiji[n_ruiji][1] = NULL;
					ruiji[n_ruiji][0] = (char*)realloc(ruiji[n_ruiji][0], sizeof(char)*(strlen(str1)+1));
					ruiji[n_ruiji][1] = (char*)realloc(ruiji[n_ruiji][1], sizeof(char)*(strlen(str2)+1));
					strcpy(ruiji[n_ruiji][0], str1);
					strcpy(ruiji[n_ruiji][1], str2);
					n_ruiji++;
					break;
				}
			}
		}
	}

	for (i = 0; i < n_ruiji; i++) {
		printf("Pair %d\n", i);
		printf("%s", ruiji[i][0]);
		printf("%s", ruiji[i][1]);
		free(ruiji[i][0]);
		free(ruiji[i][1]);
		free(ruiji[i]);
	}

	free(ruiji);
	fclose(fp);
}

void Q6(char* fname) 
{
	FILE* fp;
	char str1[1024], str2[1024];
	char*** ruiji = { NULL };
	int lines = 0, current_pos = 0, n_ruiji = 0, is_exist;
	int i, j, k;

	if((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "File Open Error\n");
		return;
	}
	
	while (fgets(str1, sizeof(str1), fp) != NULL) {
		lines++;
	}

	for (i = 0; i < lines; i++) {
		is_exist = 0;
		current_pos++;
		fp = fopen(fname, "r");
		for (j = 0; j < current_pos; j++) {
			fgets(str1, sizeof(str1), fp);
		}
		int len1 = strlen(str1);
		//if (len1 < 20) continue;
		for (j = current_pos; j < lines; j++) {
			fgets(str2, sizeof(str2), fp);
			int len2 = strlen(str2);
			int len;
			//if (len2 < 20) continue;
			if (len1 < len2) {
				for (k = len1-1; k < len2-1; k++) {
					str1[k] = ' ';
				}
				str1[len2-1] = '\n';
				str1[len2] = '\0';
				len = len2;
			} else if (len1 > len2) {
				for (k = len2-1; k < len1-1; k++) {
					str2[k] = ' ';
				}
				str2[len1-1] = '\n';
				str2[len1] = '\0';
				len = len1;
			}
			int step_1 = 0;
			int step_2 = 0;
			char tmp1[1024];
			char tmp2[1024];
			strcpy(tmp1, str1);
			strcpy(tmp2, str2);
			for (k = 0; k < len; k++) {
				if (tmp1[k] != tmp2[k]) {
					if (tmp1[k] == ' ') {
						tmp1[k] = tmp2[k];
						step_1++;
					} else {
						tmp1[k] = ' ';
						step_1++;
					}
				}
			}
			strcpy(tmp1, str1);
			strcpy(tmp2, str2);
			for (k = 0; k < len; k++) {
				if (tmp1[k] != tmp2[k]) {
					if (tmp2[k] == ' ') {
						tmp2[k] = tmp1[k];
						step_2++;
					} else {
						tmp2[k] = ' ';
						step_2++;
					}
				}
			}

			if (step_1 == 0 || step_2 == 0) continue;
			//printf("cmp: %d\n", cmp);
			if (step_1 < 4 || step_2 < 4) {
				for (k = 0; k < n_ruiji; k++) {
					int cmp1_0 = strcmp(str1, ruiji[k][0]);
					int cmp1_1 = strcmp(str1, ruiji[k][1]);
					int cmp2_0 = strcmp(str2, ruiji[k][0]);
					int cmp2_1 = strcmp(str2, ruiji[k][1]);
					if ((cmp1_0 == 0 && cmp2_1 == 0) || (cmp1_1 == 0 && cmp2_0 == 0)) {
						is_exist = 1;
					}
				}
				if (is_exist == 0) {
					ruiji = (char***)realloc(ruiji, sizeof(char**)*(n_ruiji+1));
					ruiji[n_ruiji] = (char**)malloc(sizeof(char*)*2);
					ruiji[n_ruiji][0] = NULL;
					ruiji[n_ruiji][1] = NULL;
					ruiji[n_ruiji][0] = (char*)realloc(ruiji[n_ruiji][0], sizeof(char)*(strlen(str1)+1));
					ruiji[n_ruiji][1] = (char*)realloc(ruiji[n_ruiji][1], sizeof(char)*(strlen(str2)+1));
					strcpy(ruiji[n_ruiji][0], str1);
					strcpy(ruiji[n_ruiji][1], str2);
					n_ruiji++;
					break;
				}
			}
		}
	}

	for (i = 0; i < n_ruiji; i++) {
		printf("Pair %d\n", i);
		printf("%s", ruiji[i][0]);
		printf("%s", ruiji[i][1]);
		free(ruiji[i][0]);
		free(ruiji[i][1]);
		free(ruiji[i]);
	}

	free(ruiji);
	fclose(fp);
}

void Q7(char* fname) 
{
	FILE* fp;
	char str1[1024], str2[1024];
	char** jufuku = NULL;
	int lines = 0, current_pos = 0, last_jufuku_lines = 0, jufuku_lines = 0, next_start = 0;
	int i, j, k;

	if((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "File Open Error\n");
		return;
	}
	
	while (fgets(str1, sizeof(str1), fp) != NULL) {
		lines++;
	}

	for (i = 0; i < lines; i++) {
		current_pos++;
		fp = fopen(fname, "r");
		for (j = 0; j < current_pos; j++) {
			fgets(str1, sizeof(str1), fp);
		}
		for (j = current_pos; j < lines; j++) {
			for (k = current_pos; k < next_start; k++) {
				fgets(str2, sizeof(str2), fp);
			}
			if (j - i < 4) continue;
			if (strcmp(str1, str2) == 0) {
				jufuku = (char**)realloc(jufuku, sizeof(char*)*(jufuku_lines+1));
				jufuku[jufuku_lines] = (char*)malloc(sizeof(char)*(strlen(str1)+1));
				strcpy(jufuku[lines], str1);
				jufuku_lines++;
				next_start = j;
			} else {
				if (jufuku_lines >= 4) {
					for (k = 0; k < jufuku_lines; k++) {
						printf("%s",  jufuku[k]);
					}	
				}
				last_jufuku_lines = jufuku_lines;
				jufuku_lines = 0;
			}
		}
	}

	for (i = 0; i < last_jufuku_lines; i++) {
		free(jufuku[i]);
	}

	free(jufuku);
	fclose(fp);
}

int main(int argc, char* argv[])
{
	int i;
	
	if (argc != 2) {
		fprintf(stderr, "Augment Error\n");
		return 1;
	}

	printf("-----Question 1-----\n\n");
	printf("Number of ';' = %d\n", Q1(argv[1]));
	
	printf("-----Question 2-----\n\n");
	Q2(argv[1]);
	
	printf("-----Question 3-----\n\n");
	Q3(argv[1]);
	
	printf("-----Question 4-----\n\n");
	Q4(argv[1]);

	printf("-----Question 5-----\n\n");
	Q5(argv[1]);
	
	printf("-----Question 6-----\n\n");
	Q6(argv[1]);
	
	printf("-----Question 7-----\n\n");
	Q7(argv[1]);

	return 0;
}
