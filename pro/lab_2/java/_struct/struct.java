package _struct;

public class struct{
	public double [][]mas;
	public double first_positive_element;
	public int coord_i;
	public int coord_j;
	public int M, N;

	public struct(int m, int n){
		mas = new double[m][n];
		first_positive_element = -1;
		M = m;
		N = n;
	}

}