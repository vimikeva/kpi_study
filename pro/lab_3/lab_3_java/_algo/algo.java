package _algo;

import _struct.*;

public class algo{
	public void search_first_positive_element(struct matr){
		int i, j;
		for(i = 0; i < matr.M; i++)
			for (j = 0; j < matr.N; j++)
				if(matr.mas[i][j] > 0){
					matr.first_positive_element = matr.mas[i][j];
					matr.coord_i = i;
					matr.coord_j = j;
					return;
				}
	}
}