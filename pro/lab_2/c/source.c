#include "inout.h"

int main(){
	matrix *matr = (matrix *) malloc(sizeof(matrix));

	printf("------------------------------------------------\n");
	creat_sorted_array(matr);
	print_matrix(matr);
	print_first_positive_element(matr);

	printf("------------------------------------------------\n");
	creat_inversely_sorted_array(matr);
	print_matrix(matr);
	print_first_positive_element(matr);

	printf("------------------------------------------------\n");
	creat_randomly_sorted_array(matr);
	print_matrix(matr);
	print_first_positive_element(matr);

	printf("------------------------------------------------\n");
	//creat_keyboard_input(matr);
	//print_first_positive_element(matr);

	//printf("------------------------------------------------\n");
	return 0;
}