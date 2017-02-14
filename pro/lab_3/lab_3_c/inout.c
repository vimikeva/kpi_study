#include "struct.h"
#include "inout.h"
#include "time.h"

//input martix

void creat_sorted_array(matrix *matr){
	int i, j;
	double elem = -N*M;
	srand(time(NULL));
	for(i = 0; i < M; i++)
		for(j = 0; j < N; j++){
			elem += rand()%3;
			matr->mas[i][j] = elem / 10;
		}
}

void creat_inversely_sorted_array(matrix *matr){
	int i, j;
	double elem  = N*M;
	srand(time(NULL));
	for(i = 0; i < M; i++)
		for(j = 0; j < N; j++){
			elem -= rand()%5;
			matr->mas[i][j] = elem / 10;
		}
}

void creat_randomly_sorted_array(matrix *matr){
	int i, j;
	srand(time(NULL));
	for(i = 0; i < M; i++)
		for(j = 0; j < N; j++)
			matr->mas[i][j] = (rand()%(2*N*M) - N*M) / 10.0;
}

void creat_keyboard_input(matrix *matr){
	printf("Enter elements the matrix [%d][%d]:\n", M, N);
	int i, j;
	for(i = 0; i < M; i++)
		for(j = 0; j < N; j++)
			scanf("%lf", &(matr->mas[i][j]));
}

//print matrixt

void print_matrix(matrix *matr){
	int i, j;
	printf("Print matrix [%d][%d]:\n", M, N);
	for(i = 0; i < M; i++){
		for(j = 0; j < N; j++)
			printf("%5.1lf", matr->mas[i][j]);
		printf("\n");
	}
	printf("\n");
}

void print_first_positive_element(matrix *matr){
	search_first_positive_element(matr);
	
	if (matr->first_positive_element < 0)
		printf("Not found positive element!\n");
	else
		printf("First positive element [%d][%d]: %.1lf\n\n",
			matr->coord_i, matr->coord_j, matr->first_positive_element);
}
