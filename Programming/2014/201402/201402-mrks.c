#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
	
#define	ONE				"00000000000000000000000000000001"
#define	ZERO			"00000000000000000000000000000000"
#define	ONE_FLOAT		"10000000000000000000000000000000 00"
#define	ZERO_FLOAT		"00000000000000000000000000000000 00"
#define	SQRT5_FLOAT		"22360679774997896964091736687313 00"
#define	INV_TWO_FLOAT	"05000000000000000000000000000000 00"
#define	INV_SQRT5_FLOAT	"04472135954999579392818347337462 00"
	
unsigned long long int f(int x)
{
	int i;
	unsigned long long value[2] = {0, 1};
	
	for (i=1; i<x; i++) value[i % 2] += value[(i+1) % 2];
	return value[0] + value[1];
}

int add32Int(const char *n, const char *m, char *result)
{
	result[32] = '\0';
	
	int i, c = 0;
	for (i=0; i<32; i++) {
		div_t tmp = div(c + (n[31-i] - '0') + (m[31-i] - '0'), 10);
		result[31-i] = tmp.rem + '0';
		c = tmp.quot;
	}
	
	return c;
}

char *f_str(int x, char *result)
{
	int i;
	char f1[] = ZERO, f2[] = ONE;

	for (i=1; i<x; i++) add32Int(f1, f2, (i % 2 == 0) ? f1 : f2);
	add32Int(f1, f2, result);
	return result;
}

int mul32Float(const char *n, const char *m, char *result)
{
	int i, j;
	char tmp[(32 + 1) + 1];
	char denormalize[32 + 32 + 1];
	
	memset(denormalize, '0', 64);
	for (i=0; i<32; i++) {
		memset(tmp, '0', 32+1);
		for (j=0; j<(m[31-i] - '0'); j++) tmp[0] += add32Int(n, &tmp[1], &tmp[1]);
	
		char backup = denormalize[32-i + 32];
		denormalize[31-i] = tmp[0] + add32Int(&tmp[1], &denormalize[32-i], &denormalize[32-i]);
		if (i != 0) denormalize[32-i + 32] = backup;
	}
	
	int exp = atoi(&n[33]) + atoi(&m[33]) + 1;
	for (i=0; (i<32 && i<exp); i++) if (denormalize[i] != '0') break;
	strncpy(result, &denormalize[i], 32);
	snprintf(&result[33], 3, "%02d", exp - i);
	
	result[32] = ' ';
	result[35] = '\0';
	return 32 - i;	// 捨てた桁数でも返しておこう
}

int add32Float(const char *n, const char *m, char *result)
{
	int i;
	char denormalize[(32 + 1) + 1];
	int n_exp = atoi(&n[33]);
	int m_exp = atoi(&m[33]);
	
	memset(result, '0', 32);
	int offset = abs(n_exp - m_exp);
	strncpy(&result[offset], (n_exp > m_exp) ? m : n, 32-offset);
	denormalize[0] = '0' + add32Int(result, !(n_exp > m_exp) ? m : n, &denormalize[1]);
	
	int exp = ((n_exp > m_exp) ? n_exp : m_exp) + 1;
	for (i=0; (i<33 && i<exp); i++) if (denormalize[i] != '0') break;
	int c = i;
	
	strncpy(result, &denormalize[i], 32);
	for (i=0; i<32; i++) if (result[i] == '\0') result[i] = '0';
	snprintf(&result[33], 3, "%02d", exp - c);
	result[32] = ' ';
	result[35] = '\0';

	return c;
}

char *phi(char *result)
{
	add32Float(ONE_FLOAT, SQRT5_FLOAT, result);
	mul32Float(result, INV_TWO_FLOAT, result);
	return result;
}

char *g(int x, char *result)
{
	int i;
	char phivalue[36];
	
	phi(phivalue);
	strncpy(result, ONE_FLOAT, 36);
	
	for (i=0; i<x; i++) mul32Float(result, phivalue, result);
	mul32Float(result, INV_SQRT5_FLOAT, result);

	return result;
}

int floatcmp(const char *n, const char *m)
{
	int n_zero = (strncmp(n, ZERO_FLOAT, 32) == 0);
	int m_zero = (strncmp(m, ZERO_FLOAT, 32) == 0);

	if (n_zero && m_zero) return 0;
	if (n_zero) return -1;
	if (m_zero) return 1;
	
	int cmp = (atoi(&n[33]) - atoi(&m[33]));
	return (cmp > 0) ? 1 : (cmp < 0) ? -1: strncmp(n, m, 32);
}

int absub32Float(const char *n, const char *m, char *result)
{
	int i;
	char tmp[] = ZERO;
	int cmp = floatcmp(n, m);
	int n_exp = atoi(&n[33]);
	int m_exp = atoi(&m[33]);

	int offset = abs(n_exp - m_exp);
	strncpy(&tmp[offset], (cmp > 0) ? m : n, 32-offset);
	
	// http://ja.wikipedia.org/wiki/%E5%8A%A0%E7%AE%97%E5%99%A8#.E6.B8.9B.E7.AE.97.E5.99.A8
	for (i=0; i<32; i++) tmp[i] = (9 - (tmp[i] - '0')) + '0';
	add32Int(ONE, tmp, tmp);
	add32Int(!(cmp > 0) ? m : n, tmp, tmp);
	
	int exp = ((n_exp > m_exp) ? n_exp : m_exp);
	for (i=0; (i<32 && i<exp); i++) if (tmp[i] != '0') break;
	strncpy(result, &tmp[i], 32);
	int c = i;	
	for (i=0; i<32; i++) if (result[i] == '\0') result[i] = '0';
	snprintf(&result[33], 3, "%02d", exp - c);
	
	result[32] = ' ';
	result[35] = '\0';
	return c;
}

void findMax()
{
	int i, j;
	char tmp[36];
	char fvalue[36];
	char gvalue[36];
	
	int argmax = 0;
	char max[] = ZERO_FLOAT;
	
	for (i=1; i<=140; i++) {
		f_str(i, tmp);
		g(i, gvalue);
		
		for (j=0; j<32; j++) if (tmp[j] != '0') break;
		strncpy(fvalue, &tmp[j], 32);
		fvalue[32] = ' ';
		snprintf(&fvalue[33], 3, "%02d", 31 - j);
		for (j=0; j<32; j++) if (fvalue[j] == '\0') fvalue[j] = '0';
		
		absub32Float(fvalue, gvalue, tmp);
		if (floatcmp(tmp, max) > 0) {
			strncpy(max, tmp, 36);
			argmax = i;
		}
	}
	
	printf("x = %d, value = \"%s\"\n", argmax, max);
}


int main(int argc, char *argv[])
{
	clock_t start, end;
	char result[64];
	
	printf("(1)\n");
	printf("\tf(10) = %llu\n\n", f(10));

	printf("(2)\n");
	start = clock();
	printf("\tf(50) = %llu\n", f(50));
	end = clock();
	printf("\t%f(s)\n\n", (double)(end - start) / CLOCKS_PER_SEC);

	printf("(3)\n");
	add32Int("00123456789012345678901234567890", "00987654321098765432109876543210", result);
	printf("\t00123456789012345678901234567890 + 00987654321098765432109876543210 = %s\n\n", result);

	printf("(4)\n");
	start = clock();
	printf("\tf(140) = %s\n", f_str(140, result));
	end = clock();
	printf("\t%f(s)\n\n", (double)(end - start) / CLOCKS_PER_SEC);

	printf("(5)\n");
	mul32Float("12345678901234567890123456789012 04", "98765432109876543210987654321098 09", result);
	printf("\t\"12345678901234567890123456789012 04\" * \"98765432109876543210987654321098 09\" = \"%s\"\n\n", result);
	
	printf("(6)\n");
	printf("\tphi = \"%s\"\n", phi(result));
	
	printf("(7)\n");
	printf("\tg(140) = \"%s\"\n", g(140, result));
	
	printf("(8)\n");
	printf("\t");
	findMax();
	
	return 0;
}
