#include "algo.h"

void search_first_positive_element(matrix *matr){
	int i, j;
	for(i = 0; i < M; i++)
		for(j = 0; j < N; j++)
			if(matr->mas[i][j] > 0){
				matr->first_positive_element = matr->mas[i][j];
				matr->coord_i = i;
				matr->coord_j = j;
				return;
			}
	matr->first_positive_element = -1;
}
