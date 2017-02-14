#pragma once

#define M 7
#define N 7

typedef struct{
	double mas[M][N];
	double first_positive_element;
	int coord_i;
	int coord_j;
} matrix;