import _struct.*;
import _inout.*;

public class source{
	public static void main(String[] args){
		struct matr = new struct(3,2);
		inout io = new inout();
		
		System.out.printf("----------------------------------------\n");
		io.creat_sorted_array(matr);
		io.print_matrix(matr);
		io.print_first_positive_element(matr);


		System.out.printf("----------------------------------------\n");
		io.creat_inversely_sorted_array(matr);
		io.print_matrix(matr);
		io.print_first_positive_element(matr);


		System.out.printf("----------------------------------------\n");
		io.creat_randomly_sorted_array(matr);
		io.print_matrix(matr);
		io.print_first_positive_element(matr);

		System.out.printf("----------------------------------------\n");
;		io.creat_keyboard_input(matr);
;		io.print_first_positive_element(matr);

		System.out.printf("----------------------------------------\n");
	}
}
