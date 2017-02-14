#include "methods.h"

int main() {
	int i;
	double* x = new double[M];
	double matrix[M][N] = { { 11, 5, 1, 20, 75 },
							{ 19, 57, 19, 18, 437 },
							{ 6, 10, 33, 16, 117 },
							{ 45, 14, 5, 14, 149 } };

	double matrix2[M][N] = { { 45, 14, 5, 14, 149 },
							 { 19, 57, 19, 18, 437 }, 
							 { 6, 10, 33, 16, 117 },  
							 { 11, 5, 1, 20, 75 } };  
	_array arr[M][N];
	matrix_to_array(matrix, arr);

	for (i = 0; i < M; i++)
		x[i] = 0;
	
	cout << "Choice main element Method:\n";
	choice_main_element(x, arr, 0);
	for (i = 0; i < M; i++)
		cout << "X[" << i + 1 << "] = " << x[i] << endl;;
	cout << endl;
	
	for (i = 0; i < M; i++)
		x[i] = 0;
	

	cout << "Iteration Seidel Method" << endl;
	iteration_Seidel(matrix2, x, 1e-13);
	for (i = 0; i < M; i++)
		cout << "X[" << i + 1 << "] = " << x[i] << endl;
	cout << endl;
	
	delete[] x;
	return 0;
}