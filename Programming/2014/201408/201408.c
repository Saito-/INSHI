#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	double x;
	double y;
} Vector2D;

#define Vertex2D Vector2D

Vector2D Vertex_to_Vector(Vertex2D A)
{
	Vector2D a;
	a.x = A.x;
	a.y = A.y;
	return a;
}

Vector2D VectorAB(Vertex2D A, Vertex2D B)
{
	Vector2D X;
	X.x = B.x - A.x;
	X.y = B.y - A.y;
	return X;
}

Vector2D add_Vector2D(Vector2D A, Vector2D B)
{
	Vector2D X;
	X.x = A.x + B.x;
	X.y = A.y + B.y;
	return X;
}

Vector2D mul_Vector2D(Vector2D A, double a)
{
	Vector2D X;
	X.x = A.x * a;
	X.y = A.y * a;
	return X;
}

Vertex2D naibun(Vertex2D A, Vertex2D B, int m, int n) 
{
	Vector2D a = Vertex_to_Vector(A);
	Vector2D b = Vertex_to_Vector(B);
	Vertex2D X;

	double d = 1.0 / (m + n);
	Vector2D n_A = mul_Vector2D(a, n);
	Vector2D m_B = mul_Vector2D(b, m);
	Vector2D ret = mul_Vector2D(add_Vector2D(n_A, m_B), d);
	X.x = ret.x;
	X.y = ret.y;
	return X;
}

double cross_Vector2D_z(Vector2D A, Vector2D B)
{
	return (A.x*B.y - A.y*B.x);
}

double norm_Vector2D(Vector2D A)
{
	return sqrt(A.x*A.x + A.y*A.y);
}

unsigned long long A_d_R0(double d)
{
	unsigned long long num_points;
	
	if (d == 0.0) return 1;

	d = fabs(d);
	num_points = 10.0 / d;
	num_points += 1;
	return num_points * num_points;
}

unsigned long long A_d_R1(double d)
{
	int i, j;
	unsigned long long num_points = 0;
	unsigned long long num_points_x;
	if (d == 0.0) return 0;
	d = fabs(d);
	num_points_x = 10.0 / d;
	num_points_x += 1;
	double *p;
	double x, y;
	p = (double*)malloc(num_points_x*sizeof(double));

	for (i = 0; i < num_points_x; i++) {
		p[i] = d * i;
	}

	for (i = 0; i < num_points_x; i++) {
		x = p[i];
		for (j = 0; j < num_points_x; j++) {
			y = p[j];
			//printf("(%f, %f)", x, y);
			if (((x-5.0)*(x-5.0) + (y-5.0)*(y-5.0)) <= 25.0)  {
				//printf(" is in circle");
				num_points++;
			}
			//putchar('\n');
		}
	}

	free(p);
	return num_points;
}

double calc_tri_area(double d)
{
	return (0.25 * d * d * sqrt(3));
}

double koch_area(int n)
{
	double k_0 = calc_tri_area(10.0);
	double k_n = k_0;
	int i;

	for (i = 1; i <= n; i++) {
		k_n += 3.0 * calc_tri_area(10.0/(3.0*i));
	}
	return k_n;
}

int is_in_tri(Vertex2D A, Vertex2D B, Vertex2D C, Vertex2D P)
{
	Vector2D AB = VectorAB(A, B);
	Vector2D BC = VectorAB(B, C);
	Vector2D CA = VectorAB(C, A);
	
	Vector2D AP = VectorAB(A, P);
	Vector2D BP = VectorAB(B, P);
	Vector2D CP = VectorAB(C, P);

	double z1 = cross_Vector2D_z(AB, BP);
	double z2 = cross_Vector2D_z(BC, CP);
	double z3 = cross_Vector2D_z(CA, AP);

	if ((z1 > 0 && z2 > 0 && z3 > 0) || (z1 < 0 && z2 < 0 && z3 < 0)) 
		return 1;
	return 0;
}

void next_tri(Vertex2D A, Vertex2D B, Vertex2D* X, Vertex2D* Y, Vertex2D* Z, double theta)
{
	Vertex2D tmp;
	
	tmp = naibun(B, A, 1, 2);
	X->x = tmp.x;
	X->y = tmp.y;
	
	tmp = naibun(A, B, 1, 2);
	Z->x = tmp.x;
	Z->y = tmp.y;
	
	double x, y;
	double len = norm_Vector2D(VectorAB(A, B)) / 3.0;
	x = X->x + len * cos(theta);
	y = X->y + len * sin(theta);

	Y->x = x;
	Y->y = y;
	/*
	printf("A(%f, %f)\n", X->x, X->y);
	printf("B(%f, %f)\n", Y->x, Y->y);
	printf("C(%f, %f)\n", Z->x, Z->y);
	*/
}

unsigned long long A_d_Kn(d, n)
{
	int i, j, k;
	unsigned long long num_points = 0;
	unsigned long long num_points_x;
	Vertex2D A, B, C;
	A.x = 0.0; A.y = 0.0;
	B.x = 10.0; B.y = 0.0;
	C.x = 5.0; C.y = 5*sqrt(3);
	if (d == 0.0) return 0;
	d = fabs(d);
	num_points_x = 10.0 / d;
	num_points_x += 1;
	double *p;
	int **in;
	p = (double*)malloc(num_points_x*sizeof(double));
	in = (int**)calloc(num_points_x, sizeof(int*));

	for (i = 0; i < num_points_x; i++) {
		in[i] = (int*)calloc(num_points_x, sizeof(int));
	}
	
	for (i = 0; i < num_points_x; i++) {
		p[i] = d * i;
	}
	
	Vector2D P;

	for (i = 0; i < num_points_x; i++) {
		P.x = p[i];
		for (j = 0; j < num_points_x; j++) {
			P.y = p[j];
			int flg = is_in_tri(A, B, C, P);
			if (flg == 1 && in[i][j] == 0) {
				num_points++;
				in[i][j] = 1;
			}
		}
	}
	double theta = M_PI;
	next_tri(A, C, &A, &B, &C, theta);

	for (i = 1; i < n; i++) {
		for (j = 0; j < num_points_x; j++) {
			P.x = p[j];
			for (k = 0; k < num_points_x; k++) {
				P.y = p[k];
				int flg = is_in_tri(A, B, C, P);
				if (flg == 1 && in[j][k] == 0) {
					num_points+= 3*i;
					in[j][k] = 1;
				}
			}
		}
		theta -= (M_PI/3.0);
		next_tri(B, A, &A, &B, &C, theta);
	}
	
	for (i = 0; i < num_points_x; i++) {
		free(in[i]);
	}
	free(in);
	free(p);
	return num_points++;
}

int main()
{
	double d, s, k_2, k_n;
	int n;
	unsigned long long Q1, Q2, Q5, Q6;
	printf("\n-----Q1-----\n");
	printf("Calc A(d, R0)\n");
	printf("d: "); scanf("%lf", &d);
	Q1 = A_d_R0(d);
	printf("A(%f, R0) = %llu\n",d, Q1);
	
	printf("\n-----Q2-----\n");
	printf("Calc 0.25 * A(d, R1) / A(d, R0)\n");
	printf("d: "); scanf("%lf", &d);
	Q1 = A_d_R0(d);
	Q2 = A_d_R1(d);
	printf("A(%f, R0) = %llu\n",d , Q1);
	printf("A(%f, R1) = %llu\n",d , Q2);
	s = (double)Q2 / Q1;
	printf("0.25 * (A(%f, R1) / A(%f, R0)) = %f\n", d, d, s*0.25);

	printf("\n-----Q3-----\n");
	k_2 = koch_area(2);
	printf("k_2: %f\n", k_2);
	
	printf("\n-----Q4-----\n");
	printf("Calc Kn\n");
	printf("n: "); scanf("%d", &n);
	k_n = koch_area(n);
	printf("Kn: %f\n", k_n);

	printf("\n-----Q5-----\n");
	printf("Calc A(d, K2)\n");
	printf("d: "); scanf("%lf", &d);
	Q5 = A_d_Kn(d, 2);
	printf("A(%f, K2): %llu\n", d, Q5);
	
	printf("\n-----Q6-----\n");
	printf("Calc A(d, Kn)\n");
	printf("d: "); scanf("%lf", &d);
	printf("n: "); scanf("%d", &n);
	Q6 = A_d_Kn(d, n);
	printf("A(%f, K%d): %llu\n", d, n, Q6);

	return 0;
}
