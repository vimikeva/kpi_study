package _inout;

import java.util.Scanner;
import _struct.*;
import _algo.*;

public class inout{

//input matrix
	public void creat_sorted_array(struct matr){
		int i, j;
		int elem = -matr.N*matr.M;
		for(i = 0; i < matr.M; i++)
			for(j = 0; j < matr.N; j++){
				elem += (i*j+i+j+i)%3;
				matr.mas[i][j] = elem / 10.0;
		}
	}

	public void creat_inversely_sorted_array(struct matr){
		int i, j;
		double elem  = matr.N*matr.M;
		for(i = 0; i < matr.M; i++)
			for(j = 0; j < matr.N; j++){
				elem -= (i*j+j+i)%5;
				matr.mas[i][j] = elem / 10;
			}
	}

	public void creat_randomly_sorted_array(struct matr){
		int i, j;
		for(i = 0; i < matr.M; i++)
			for(j = 0; j < matr.N; j++)
				matr.mas[i][j] = ((2*matr.N*matr.M*i)%15 - (matr.N*matr.M)%(i+j+5)) / 10.0;
	}
  
	public void creat_keyboard_input(struct matr){
		System.out.printf("Enter elements the matrix [%d][%d]:\n", matr.M, matr.N);
		int i, j;
		Scanner in = new Scanner(System.in);
		for(i = 0; i < matr.M; i++)
			for(j = 0; j < matr.N; j++)
				matr.mas[i][j] = in.nextDouble();
	}


// output matrix
	public void print_matrix(struct matr){
		int i, j;
		System.out.printf("Print matrix [%d][%d]:\n", matr.M, matr.N);
		for(i = 0; i < matr.M; i++){
			for(j = 0; j < matr.N; j++)
				System.out.printf("%5.1f", matr.mas[i][j]);
			System.out.printf("\n");
		}
		System.out.printf("\n");
	}

	public void print_first_positive_element(struct matr){
		algo al = new algo();
		al.search_first_positive_element(matr);
	
		if (matr.first_positive_element < 0)
			System.out.printf("Not found positive element!\n");
		else
			System.out.printf("First positive element [%d][%d]: %.1f\n\n",
				matr.coord_i, matr.coord_j, matr.first_positive_element);
	}


}