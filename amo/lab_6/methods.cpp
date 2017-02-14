#include "methods.h"

#define cof_A 1
double a = -1, b = 1;

double fuction(double x) {
	return 10 * cof_A * cos(11*x) * cos(x) * sin(x);
}

double Fi(int i, int n) {
	return fuction(a + (b - a) / n * i);
}

double* coef_Ai (int n) {
	double h = (b - a) / n, x = a, *A = new double[n];
	for (int i = 0; i < n; ++i, x+=h)
		A[i] = fuction(x);
	return A;
}

double* coef_Ci(int n) {
	double *C = new double[n], *matrix = new double[n * (n + 1)];
	double h = (b - a) / n;

	for (int i = 0; i <= n; ++i)
		matrix[i] = matrix[(n - 1) * (n + 1) + i] = 0;
	matrix[0] = matrix[(n - 1) * (n + 1) + n - 1] = 1;

	for (int i = 1; i < n - 1; ++i)
		for (int j = 0; j <= n; ++j) {
			matrix[i * (n + 1) + j] = 0;
			if (j == i - 1 || j == i + 2) 
				matrix[i * (n + 1) + j] = h;
			else if (j == i) 
				matrix[i * (n + 1) + j] = 4 * h;
			if (j == n) 
				matrix[i * (n + 1) + j] = 6 * (Fi(i - 1, n) - 2 * Fi(i, n) + Fi(i + 1, n)) / h;
		}

	double temp;
	for (int p = 0; p < n - 1; ++p) {
		temp = matrix[p * (n + 1) + p];
		for (int j = 0; j <= n; ++j)
			matrix[p * (n + 1) + j] = matrix[p * (n + 1) + j] / temp;

		for (int i = p + 1; i < n; ++i) {
			temp = matrix[i * ((int)n + 1) + p];
			for (int j = 0; j <= n; ++j)
				matrix[i * (n + 1) + j] = matrix[i * (n + 1) + j] - matrix[p * (n + 1) + j] * temp;
		}
	}

	for (int i = 0; i < n; ++i) {
		C[n - i - 1] = matrix[(n - i - 1) * (n + 1) + n];
		for (int j = 0; j < i; ++j)
			C[n - i - 1] -= matrix[(n - i - 1) * (n + 1) + n - 1 - j] * C[n - i];
	}
	return C;
}

double* coef_Di(double* C, int n) {
	double h = (b - a) / n, *D = new double[n];
	D[0] = C[0];
	for (int i = 1; i < n; ++i)
		D[i] = (C[i] - C[i - 1]) / h;
	return D;
}

double* coef_Bi(double* C, double* D, int n) {
	double h = (b - a) / n, *B = new double[n];
	B[0] = 0;
	for (int i = 1; i < n; ++i)
		B[i] = (h * C[i] / 2) - (h * h * D[i] / 2) + (Fi(i, n) - Fi(i - 1, n)) / h;
	return B;
}

double spline(double x, double *A, double *B, double *C, double *D, int n) {
	double h = (b - a) / n;
	int i = 0;
	if (x <= a || x > b) return 0;
	while (x > a + i * h) ++i;
	double x_xi =x -(a + --i * h);
	return A[i] + B[i] * x_xi + C[i] * x_xi * x_xi / 2 + D[i] * x_xi * x_xi * x_xi / 6;
}

void interpolation(int n) {
	double *A = coef_Ai(n), *C = coef_Ci(n), 
		   *D = coef_Di(C, n), *B = coef_Bi(C, D, n);
	int k = (int)((b - a) * 300);
	double step = (b - a) / (double)k, x, y;

	x = a;
	ofstream stream("results.txt");
	for (int i = 0; i <= k; ++i) {
		y = spline(x, A, B, C, D, n);
		stream << ((abs(x) < 10e-6) ? 0 : x) << " ; ";
		stream << ((abs(y) < 10e-6) ? 0 : y) << endl;
		x += step;
	}
	stream.close();
	return;
}