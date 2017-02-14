#include "methods.h"

void matrix_to_array(double matrix[M][N], _array arr[M][N]) {
	int i, j;
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++) {
			arr[i][j].data = matrix[i][j];
			arr[i][j].flag = 1;
		}
}

void choice_main_element(double x[M], _array arr[M][N], int iter) {
	int i, j, p = 0, q = 0;
	double max = arr[0][0].data, temp = 0;
	double *factor = new double[M];
	for (i = 0; i < M; i++)
		factor[i] = 0;
	
	for (i = 0; i < M; i++)
		for (j = 0; j < M; j++)
			if (fabs(arr[i][j].data) > fabs(max) && arr[i][j].flag != 0)
				max = arr[p = i][q = j].data;
	
	for (i = 0; i < M; i++)
		if (i != p)
			factor[i] = -arr[i][q].data / max;
	
	for (i = 0; i < M; i++)
		if (i != p)
			for (j = 0; j < N; j++)
				if (arr[i][j].flag != 0)
					arr[i][j].data = arr[i][j].data + arr[p][j].data * factor[i];
	
	for (i = 0; i < M; i++)
		for (j = 0; j < N; j++)
			if (i == p || j == q)
				arr[i][j].flag = 0;
	delete[] factor;
	
	if (iter < M - 1)
		choice_main_element(x, arr, iter + 1);
	
	for (i = 0; i < M; i++)
		if (i == p) {
			for (temp = 0, j = 0; j < M; j++)
				if (j != q)
					temp = temp + x[j] * arr[i][j].data;
			x[q] = (arr[i][N - 1].data - temp) / arr[p][q].data;
		}
}

void iteration_Seidel(double matrix[M][N], double x[M], double eps) {
	int i, j;
	double q = 0, temp_q, norma;
	double beta[M], alfa[M][M], next_x[M];

	for (i = 0; i < M; i++)
		beta[i] = x[i] = matrix[i][M] / matrix[i][i];

	for (i = 0; i < M; i++)
		for (j = 0; j < M; j++) {
			if (i != j)
				alfa[i][j] = -matrix[i][j] / matrix[i][i];
			else
				alfa[i][j] = 0;
		}


	for (j = 0; j < M; j++)
		q += fabs(alfa[0][j]);
	for (i = 1; i < M; i++) {
		for (temp_q = 0, j = 0; j < M; j++)
			temp_q += fabs(alfa[i][j]);
		q = temp_q > q ? temp_q : q;
	}

	while (true) {
		for (i = 0; i < M; i++) {
			next_x[i] = beta[i];
			for (j = 0; j < i; j++)
				next_x[i] += alfa[i][j] * next_x[j];
			for (j = i; j < M; j++)
				next_x[i] += alfa[i][j] * x[j];
		}

		norma = 0;
		for (i = 0; i < M; i++) {
			norma += (next_x[i] - x[i])*(next_x[i] - x[i]);
		}
		norma = sqrt(norma);

		if (norma <= eps *(1 - q) / q)
			break;
		
		for (i = 0; i < M; i++)
			x[i] = next_x[i];
	}
}
