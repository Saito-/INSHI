#include <stdio.h>
#include <math.h>

/* メモ用の配列 */
unsigned long long f_array[1000001] = { 1, 0 };

/* f(n) を定義通りに計算する関数 */
unsigned long long f_n(int n)
{
	if (n == 0) 
		return 1;
	else 
		return (161 * f_n(n-1) + 2457) % 16777216;
}

/* メモを利用して f(n) を計算する関数 */
unsigned long long f_n_memo(int n)
{
	return (161 * f_array[n-1] + 2457) % 16777216;
}

/* メモ用の配列を作成する関数 */
void make_f_array(int n)
{
	int i;
	for (i = 1; i < n; i++) {
		f_array[i] = f_n_memo(i);
	}
}

/* f(n) の値が偶数になる n の個数を返す関数*/
int count_f_array_even(int n)
{
	int count = 0;
	int i;
	for (i = 0; i < n; i++) {
		if ((f_array[i] % 2) == 0) count++;
	}
	return count;
}

/* f(n) の値が偶数になる奇数 n の個数を返す関数 */
int count_f_array_even_odd_n(int n)
{
	int count = 0;
	int i;
	for (i = 1; i < n; i+=2) {
		if ((f_array[i] % 2) == 0) count++;
	}
	return count;
}

int main()
{
	int n = 1000000;
	int count = 0;
	int count_odd = 0;
	int i;
	unsigned long long f_100 = f_n(100);

	make_f_array(n+1);

	/*
	for (i = 0; i < n+1; i++) {
		printf("f_array[%d] = %lld\n", i, f_array[i]);
	}
	*/

	count = count_f_array_even(100);
	count_odd = count_f_array_even_odd_n(100);
	printf("f(100) = %llu\n", f_100);
	printf("f(100) = %llu\n", f_array[100]);
	printf("f(i) %% 2 = 0: i = %d\n", count);
	printf("f(i) %% 2 = 0 (odd i): i = %d\n", count_odd);
	printf("f(1000000) = %llu\n", f_array[1000000]);
	return 0;
}
