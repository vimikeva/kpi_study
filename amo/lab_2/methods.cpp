//methods.cpp
#include "methods.h"

struct Result {
	double x;
	double clar_root;
	int count_iter;

	Result() {};
	Result(double x, double clar_root, int count_iter) :
		x(x), clar_root(clar_root), count_iter(count_iter) {};
	Result(Result& src);
	~Result() {};

	Result& operator=(Result& src);
};

Result::Result(Result& src) {
	x = src.x;
	clar_root = src.clar_root;
	count_iter = src.count_iter;
}

Result& Result::operator=(Result& src) {
	if (this == &src)
		return src;
	x = src.x;
	clar_root = src.clar_root;
	count_iter = src.count_iter;
	return *this;
}

double first_div_fx(double x) {
	return 3 * sin(x) / (cos(x) + 2) / (cos(x) + 2) - 1 / 4.0;
}

double fx(double x) {
	return 3 / (2 + cos(x)) - x / 4;
}

Result iteration_method(double m, double M, double x, double eps) {
	double x_n = x, lambda, q;
	int count = 0;

	lambda = 1 / M;
	q = 1 - fabs(m / M);
	x_n = x;
	do {
		count++;
		x = x_n;
		x_n = x - lambda * fx(x);
	} while (fabs(x_n - x) > (1 - q) / q*eps);
	return Result(x_n, abs(x_n - x), count);
}

Result bisection_method(double a, double  b, double eps) {
	double x, c;
	int count = 0;

	while (abs(b - a) >= 2 * eps) {
		c = (a + b) / 2;
		if ((fx(a)*fx(c)) < 0)
			b = c;
		if ((fx(b)*fx(c)) < 0)
			a = c;
		++count;
	}
	x = (a + b) / 2;

	return Result(x, (b - a) * 0.5, count);
}

void table_iter(double a, double b, double x) {
	int i;
	double eps, m, M;
	Result result;
	if (fabs(first_div_fx(a)) > fabs(first_div_fx(b))) {
		M = first_div_fx(a);
		m = first_div_fx(b);
	}
	else {
		M = first_div_fx(b);
		m = first_div_fx(a);
	}

	cout << "Eps " << "\tRoot's value" << "\tDefine root more accurately\n";
	for (i = 0, eps = 1e-2; i <= N; ++i, eps /= 1e3) {
		result = iteration_method(m, M, x, eps);
		cout << eps << " \t";
		cout << setw(15) << setprecision(14) << result.x << "  ";
		cout << setw(15) << setprecision(14) << result.clar_root << endl;
	}
	cout << endl;
}

void table_bis(double a, double b) {
	int i;
	double eps;
	Result result;
	cout << "Eps " << "\tRoot's value" << "\tDefine root more accurately\n";
	for (i = 0, eps = 1e-2; i <= N; ++i, eps /= 1e3) {
		result = bisection_method(a, b, eps);
		cout << eps << " \t";
		cout << setw(15) << setprecision(14) << result.x << "  ";
		cout << setw(15) << setprecision(14) << result.clar_root << endl;
	}
	cout << endl;
}

void table_counts(double a, double b, double x){
	int i;
	double eps, m, M;
	Result result;
	if (fabs(first_div_fx(a)) > fabs(first_div_fx(b))) {
		M = first_div_fx(a);
		m = first_div_fx(b);
	}
	else {
		M = first_div_fx(b);
		m = first_div_fx(a);
	}
	cout << "Eps " << " Iteration method" << " Bisection method\n";
	for (i = 0, eps = 1e-2; i <= N; ++i, eps /= 1e3) {
		cout << eps << " \t";
		cout << setw(5) << iteration_method(m, M, x, eps).count_iter << " \t\t";
		cout << setw(7) << bisection_method(a, b, eps).count_iter << endl;
	}
	cout << endl;
}
