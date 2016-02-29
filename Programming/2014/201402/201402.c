#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ONE             "00000000000000000000000000000001"
#define ZERO            "00000000000000000000000000000000"
#define ONE_FLOAT       "10000000000000000000000000000000 00"
#define ZERO_FLOAT      "00000000000000000000000000000000 00"
#define TWO_FLOAT	"20000000000000000000000000000000 00"
#define INV_SQRT5_FLOAT "04472135954999579392818347337462 00"
#define GOLDEN_FLOAT	"16180339887498948482045868343656 00"

unsigned long long f(int x)
{
	unsigned long long ret;
	unsigned long long* memo;
	int i;
	if (x <= 2) 
		return 1;

	memo = (unsigned long long*)calloc(x, sizeof(unsigned long long));
	memo[0] = 1;
	memo[1] = 1;
	
	for (i = 2; i < x; i++) {
		memo[i] = memo[i-1] + memo[i-2];
	}

	ret = memo[x-1];
	free(memo);
	return ret;
}

int get_exp(const char* f)
{
	return atoi(&f[33]);
}

void set_exp(char* f, int e)
{
	snprintf(&f[33], 3, "%02d", e);
}

void int32_to_float32(const char* from, char* to)
{
	int e = 0;
	int i;

	for (i = 0; i < 32; i++) {
		if (from[i] != '0') {
			e = 31 - i;
			break;
		}
	}
	int lshift = 31-e;
	int len = strlen(&from[lshift]);

	snprintf(to, len+1, "%s", &from[lshift]);
	memset(&to[len], '0', lshift);
	set_exp(to, e);
	to[32] = ' ';
	to[35] = '\0';
}

void float32_to_int32(const char* from, char* to)
{
	int i;
	int e = get_exp(from);
	memset(to, '0', 32);

	int rshift = 31-e;
	for (i = 0; i < 32-rshift; i++) {
		to[i+rshift] = from[i];
	}
	to[32] = '\0';
}

int cmp_int32(const char* i1, const char* i2)
{
	int i;
	for (i = 0; i < 32; i++) {
		int num1 = i1[i] - '0';
		int num2 = i2[i] - '0';
		if (num1 < num2) return -1;
		if (num1 > num2) return 1;
	}
	return 0;
}

int cmp_float32(const char* f1, const char* f2)
{
	int i;
	int e1 = get_exp(f1);
	int e2 = get_exp(f2);

	if (e1 > e2) return 1;
	if (e1 < e2) return -1;

	for (i = 0; i < 32; i++) {
		int num1 = f1[i] - '0';
		int num2 = f2[i] - '0';
		if (num1 < num2) return -1;
		if (num1 > num2) return 1;
	}
	return 0;
}

int add_int32(const char* i1, const char* i2, char* result)
{
	int i;
	int carry = 0;

	for (i = 0; i < 32; i++) {
		int num1 = i1[31-i] - '0';
		int num2 = i2[31-i] - '0';
		int keta = num1 + num2 + carry;
		div_t d = div(keta, 10);
		carry = d.quot;
		result[31-i] = d.rem + '0';
	}
	return carry;
}

int add_float32(const char* f1, const char* f2, char* result)
{
	int i;
	int carry_over = 0;
	char add[34];

	int e1 = get_exp(f1);
	int e2 = get_exp(f2);

	int diff_e = abs(e1 - e2);

	memset(result, '0', 32);
	strncpy(&result[diff_e], (e1 > e2) ? f2 : f1, 32-diff_e);
	add[0] = add_int32(result, !(e1 > e2) ? f2 : f1, &add[1]);

	int e =  ((e1 > e2) ? e1 : e2) + 1;
	for (i = 0; (i < 33 && i < e); i++) 
		if (add[i] != 0)
			break;
	int c = i;

	strncpy(result, &add[i], 32);
	for (i = 0; i < 32; i++) 
		if (result[i] == '\0')
			result[i] = '0';
	set_exp(result, e-c);
	result[32] = ' ';
	result[35] = '\0';

	return c;
}

void f_int32(int x, char* result)
{
	char** memo;
	int i, j;
	if (x <= 2) {
		char one[] = "00000000000000000000000000000001";
		strcpy(result, one);
		return;
	}

	memo = (char**)malloc(x * sizeof(char*));
	for (i = 0; i < x; i++) {
		memo[i] = (char*)malloc(33 * sizeof(char));
	}

	memo[0] = "00000000000000000000000000000001";
	memo[1] = "00000000000000000000000000000001";

	for (i = 2; i < x; i++) {
		add_int32(memo[i-1], memo[i-2], memo[i]);
	}

	strcpy(result, memo[x-1]);
	free(memo);
}

void f_float32(int x, char* result)
{
	char tmp[33];
	f_int32(x, tmp);
	int32_to_float32(tmp, result);
}

void mul_float32(const char* f1, const char* f2, char* result)
{
	int i, j;
	int carry;
	char tmp[33][65];
	char add[65];

	memset(add, '0', 64);

	int e1 = get_exp(f1);
	int e2 = get_exp(f2);

	int e = e1 + e2;

	for (i = 0; i < 32; i++) 
		memset(tmp[i], '0', 64);
	

	for (i = 0; i < 32; i++) {
		int num2 = f2[31-i] - '0';
		for (j = 0; j < 32; j++) {
			int num1 = f1[31-j] - '0';
			int keta = num1 * num2 + carry;
			div_t d = div(keta, 10);
			carry = d.quot;
			tmp[i][63-j-i] = d.rem + '0';
		}
		tmp[i][63-j-i] = carry + '0';
	}

	carry = 0;
	for (i = 0; i < 64; i++) {
		int keta = 0;
		keta += carry;
		for (j = 0; j < 32; j++) {
			keta += tmp[j][63-i] - '0';
		}
		div_t d = div(keta, 10);
		carry = d.quot;
		add[63-i] = d.rem + '0';
	}

	if (add[0] == '0') {
		int rshift = 0;
		for (i = 1; add[i] == '0'; i++) { 
			rshift++;
		}
		e -= rshift;
		for (i = 0; i < 32; i++) {
			result[i] = add[i+rshift+1];
		}
	} else {
		e += 1;
		for (i = 0; i < 32; i++) {
			result[i] = add[i];
		}
	}
	set_exp(result, e);
	result[32] = ' ';
	result[35] = '\0';
}

void sub_int32(const char* i1, const char* i2, char* result)
{
	int i, j;
	int borrow = 0;
	char tmp1[33], tmp2[33];
	char comp_9[33];
	strcpy(tmp1, i1);
	strcpy(tmp2, i2);

	memset(comp_9, '0', 32);
	for (i = 0; i < 32; i++) {
		int num2 = tmp2[31-i] - '0';
		int tmp = 9 - num2;
		comp_9[31-i] = tmp + '0';
	}
	add_int32(comp_9, ONE, comp_9);
	add_int32(i1, comp_9, result);
	result[32] = '\0';
}

void sub_float32(const char* f1, const char* f2, char* result)
{
	int i;
	char tmp[] = ZERO;

	int e1 = get_exp(f1);
	int e2 = get_exp(f2);

	int diff_e = abs(e1 - e2);
	strncpy(&tmp[diff_e], f2, 32-diff_e);

	sub_int32(f1, tmp, tmp);

	int e = ((e1 > e2) ? e1 : e2);
	for (i = 0; (i < 32 && i < e); i++) 
		if (tmp[i] != '0')
			break;

	strncpy(result, &tmp[i], 32);
	int c = i;
	for (i = 0; i < 32; i++) 
		if (result[i] == '\0')
			result[i] = '0';
	set_exp(result, e-c);

	result[32] = ' ';
	result[35] = '\0';
}

void div_int32(const char* i1, const char* i2, char* result)
{
	int i, j;
	char tmp1[33], tmp2[33];
	strcpy(tmp1, i1);

	for (i = 0; i < 32; i++) {
		int q = 0;
		while (1) {
			if (cmp_int32(tmp1, i2) == -1) break;
			sub_int32(tmp1, i2, tmp2);
			q++;
			strcpy(tmp1, tmp2);
		}
		result[i] = q + '0';
		for (j = 0; j < 31; j++) {
			tmp1[j] = tmp1[j+1];
		}
		tmp1[31] = '0';
	}
}

void g(int x, char* result)
{
	char f_x[36], f_x_1[36];
	char tmp[36];
	f_float32(x, f_x);
	f_float32(x-1, f_x_1);
	mul_float32(GOLDEN_FLOAT, f_x, tmp);
	printf("tmp: %s\n", tmp);
	add_float32(tmp, f_x_1, tmp);
	printf("tmp: %s\n", tmp);
	mul_float32(tmp, INV_SQRT5_FLOAT, result);
}

int main()
{
	clock_t start, end;
	char i1[] = "00123456789012345678901234567890";
	char i2[] = "00987654321098765432109876543210";
	char f1[] = "12345678901234567890123456789012 04";
	char f2[] = "98765432109876543210987654321098 09";
	unsigned long long Q1;
	unsigned long long Q2;
	char Q3[33];
	char f_140[33];
	char f_140_f[36];
	char f_139[33];
	char f_139_f[36];
	char golden[36];
	char Q5[36];
	char Q7[36];

	printf("\n-----Q1-----\n");
	Q1 = f(10);
	printf("f(10) = %llu\n", Q1);

	printf("\n-----Q2-----\n");
	start = clock();
	Q2 = f(50);
	end = clock();
	printf("f(50) = %llu\n", Q2);
	printf("time: %f [sec]\n", (double)(end - start)/CLOCKS_PER_SEC);
	
	printf("\n-----Q3-----\n");
	add_int32(i1, i2, Q3);
	printf("%s\n", Q3);

	printf("\n-----Q4-----\n");
	start = clock();
	f_int32(140, f_140);
	end = clock();
	printf("f(140) = %s\n", f_140);
	printf("time: %f [sec]\n", (double)(end - start)/CLOCKS_PER_SEC);

	printf("\n-----Q5-----\n");
	mul_float32(f1, f2, Q5);
	printf("%s\n", Q5);

	printf("\n-----Q6-----\n");
	int32_to_float32(f_140, f_140_f);
	f_int32(139, f_139);
	int32_to_float32(f_139, f_139_f);
	div_int32(f_140, f_139, golden);
	golden[32] = ' ';
	golden[33] = '0';
	golden[34] = '0';
	golden[35] = '\0';
	printf("%s\n", golden);

	printf("\n-----Q7-----\n");
	g(10, Q7);
	printf("%s\n", Q7);
	return 0;
}
