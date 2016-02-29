#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* 定義通りに g(n) を計算する関数 */
unsigned long long g_n(int n)
{
	if (n == 0) 
		return 1;
	else 
		return (1103515245 * g_n(n-1) + 12345) % 67108864;
}

/* h(n) を計算する関数 */
unsigned long long h_n(int n, unsigned long long* g_array)
{
	return g_array[n] % 1024;
}

/* メモを利用して g(n) を計算する関数 */
unsigned long long g_n_memo(int n, unsigned long long* g_array)
{
	return (1103515245 * g_array[n-1] + 12345) % 67108864;
}

/* メモ配列を作成する関数 */
void make_g_array(int n, unsigned long long* g_array)
{
	int i;
	for (i = 1; i < n+1; i++) {
		g_array[i] = g_n_memo(i, g_array);
	}
}

/* メモ配列の k 番目に新しい値を格納する関数 */
void add_g_array(int k, unsigned long long* g_array)
{
	g_array[k] = g_n_memo(k, g_array);
}

/* メモ配列を作成する関数 */
void print_g_array(int n, unsigned long long* g_array)
{
	int i;
	for (i = 0; i < n+1; i++) {
		printf("g_array[%d] = %lld\n", i, g_array[i]);
	}
}

/* g(n+k) = g(n) となる最小の k を見つける関数 */
int g_n_k(int n, unsigned long long* g_array)
{
	int k = 0;
	int i = 1;
	make_g_array(n, g_array);
	long long g_n = g_array[n];
	print_g_array(n, g_array);
	while (1) {
		add_g_array(n+i, g_array);
		long long g_n_k = g_array[n + i];
		//printf("g_array[%d] = %lld\n", n+i, g_array[n+i]);
		if (g_n == g_n_k) return i;
		i++;
	}
	return -1;
}

/* h(n+k) = h(n) となる最小の k を見つける関数 */
int h_n_k(int n, unsigned long long* g_array)
{
	int k = 0;
	int i = 1;
	make_g_array(n, g_array);
	long long hn = h_n(n, g_array);
	//printf("h(%d) = %llu\n", n, hn);
	while (1) {
		add_g_array(n+i, g_array);
		long long hnk = h_n(n + i, g_array);
		//printf("g_array[%d] = %lld\n", n+i, g_array[n+i]);
		if (hn == hnk) return i;
		i++;
	}
	return -1;
}

int main()
{
	int n = 1;
	int count = 0;
	int i, k;
	unsigned long long g_2 = g_n(2);
	unsigned long long g_3 = g_n(3);
	unsigned long long* g_array;

	g_array = (unsigned long long*)malloc(sizeof(unsigned long long) * UINT_MAX);
	g_array[0] = 1;

	make_g_array(3, g_array);

	printf("g(1) = %llu\n", g_array[1]);
	printf("g(2) = %llu\n", g_array[2]);
	printf("g(3) = %llu\n", g_array[3]);

	printf("g(1) = %llu = g(1 + %d)\n", g_array[1], g_n_k(1, g_array));
	printf("g(2) = %llu = g(2 + %d)\n", g_array[2], g_n_k(2, g_array));
	printf("g(3) = %llu = g(3 + %d)\n", g_array[3], g_n_k(3, g_array));
	
	printf("h(1) = %llu\n", h_n(1, g_array));
	printf("h(2) = %llu\n", h_n(2, g_array));
	printf("h(3) = %llu\n", h_n(3, g_array));

	printf("h(1) = h(1 + %d)\n", h_n_k(1, g_array));
	printf("h(2) = h(2 + %d)\n", h_n_k(2, g_array));
	printf("h(3) = h(3 + %d)\n", h_n_k(3, g_array));
	
	return 0;
}
