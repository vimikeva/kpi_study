#include "methods.h"

int  main() {
	double a = 2, b = 8;
	double integral, delta;

	integral = F(b) - F(a);
	printf( "a = %.2lf\nb = %.2lf\nIntegral (F(b) - F(a)) = %.8lf\n\n", a, b, integral);
	
	printf("Composite Simpson's:\n");
	printf( " |     e    |    h    |      Integral      |     delta    |\n");
	delta = Simpson(a, b, &integral);

	printf("Refined calculation:\n");
	printf(" |    delta    |    h    |     Abs     |\n");
	calculation(a, b, delta, integral);

	return 0;
}