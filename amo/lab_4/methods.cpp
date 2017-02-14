#include "methods.h"

double function(double x) {
	return pow(3 * x + 1, 5);
}

double F(double x) {
	return pow(3 * x + 1, 6) / 18;
}

double div_4(double x) {
	return 29160 * x + 9720;
}

double Integral(double a, double b, int n) {
	int i;
	double h = (b - a) / n,
		sigma1 = 0, sigma2 = 0;

	for (i = 1; i < n; i++)
		if (i % 2 == 0)
			sigma2 += function(a + i*h);
		else
			sigma1 += function(a + i*h);

	return h / 3 * (function(a) + function(b) + 4 * sigma1 + 2 * sigma2);
}

double Simpson(double a, double b, double* integ) {
	int n;
	double h, delta;
	double eps = 0.000001;
	double x = a, max = fabs(div_4(x));

	while (x < b) {
		x += eps;
		if (fabs(div_4(x)) > max)
			max = fabs(div_4(x));
	}

	h = pow(180 * eps / (b - a) / max, 0.25);
	n = 2 * (int)(0.5*(b - a) / h); 
	h = (b - a) / n;
	delta = fabs(*integ - Integral(a, b, n));

	printf(" | %f | %.5f | %.9f | %.9f  |\n\n", eps, h, Integral(a, b, n), delta);
	return delta;
}

void calculation(double a, double b, double delta, double integ) {
	int n;
	double r, integn, integ2n;
	double eps = 0.000001;

	n = (int)(1 / pow(eps, 0.25));
	integn = Integral(a, b, n);
;
	integ2n = Integral(a, b, n *= 2);
	r = fabs(integn - integ2n) / 15;

	while (r > delta) {
		integn = Integral(a, b, n);
		integ2n = Integral(a, b, n *= 2);
		r = fabs(integn - integ2n) / 15;
	}

	printf(" | %.9f | %.5f | %.9f |\n\n", delta, 0.5 * (b - a) / n, fabs(integ - integ2n));
}
