#include "sinx.h"

struct Result {
	double res;
	int n;
	double abs_err;
	double rem_term;

	Result() {};
	Result(double r, int k, double abs, double rt) :
		res(r), n(k), abs_err(abs), rem_term(rt) {};
	~Result() {};
	Result(Result& src);
	
	Result& operator=(Result& src);
};

Result::Result(Result& src) {
	res = src.res;
	n = src.n;
	abs_err = src.abs_err;
	rem_term = src.rem_term;
}

Result& Result::operator=(Result& src) {
	if (this == &src)
		return src;

	res = src.res;
	n = src.n;
	abs_err = src.abs_err;
	rem_term = src.rem_term;
	return *this;
}

enum function {SIN, COS};

struct Value {
	function f;
	double x;
	int sign;

	Value() {};
	Value(function fun, double val, int s) :f(fun), x(val), sign(s) {};
	~Value() {};
	Value(Value& src);
	Value& operator=(Value& src);
};

Value::Value(Value& src) {
	f = src.f;
	sign = src.sign;
	x = src.x;
}

Value& Value::operator=(Value& src) {
	if (this == &src)
		return src;
	f = src.f;
	sign = src.sign;
	x = src.x;
	return *this;
}

Value cast(double x){
	const double _2pi = 2 * M_PI;
	int sign = 1;
	function f = SIN;
  
	if (x > 0)
		while (x >= _2pi)
			x -= _2pi;
	else
		while (x <= _2pi)
			x += _2pi;

	if (x >= M_PI){
		sign = -1;
		x-=M_PI;
	}
	if (x >= M_PI_2)
		x = M_PI - x;
	if (x >= M_PI_4){
		f = COS;
		x = M_PI_2 - x;
	}

	return Value(f, x, sign);  
}

Result val_with_prec(double x, double eps) {
	double U, result = 0;
	int k;
	Value val = cast(x);
	U = val.x;
	double sqr_x = U * U;
  
	if (val.f == SIN) {
		for(k = 1; abs(U) >= eps; ++k) {
			result += U;
			U *= -sqr_x/(2*k * (2*k + 1));
		}
	}
	else {
		U = 1;
		for(k = 1; abs(U) >= eps; ++k) {
			result += U;
			U *= -sqr_x/(2*k * (2*k - 1));
		}
	}
	result *= val.sign;

  return Result(result, k, abs(result - sin(x)), U);
}

Result abs_error(double x, int n) {
	double U, result = 0;
	int k = 1;
	double rem_term = 0;
	Value val;

	if(n > 0) {
		val = cast(x);
		U = val.x;
		double sqr_x = U * U;
		if (val.f == SIN) {
			while(--n) {
				result += U;
				U *= -sqr_x/(2*k * (2*k + 1));
				k++;
			}
		}
		else {
			U = 1;
			while(--n) {
				result += U;
				U *= -sqr_x/(2*k * (2*k - 1));
				k++;
			}
		}
		result *= val.sign;
		rem_term = U;
	}

  return Result(result, k, abs(result - sin(x)), rem_term);
}

void table_1(double a, double b) {
	double eps, x = a; // (a + b) / 2;
	Result result;

	cout << " Eps\t  n\t Absolute Error \tRemainder term\n";

	for (eps = 1e-2; eps >= 1e-14; eps *= 1e-3) {
		result = val_with_prec(x, eps);
		cout << eps << "\t  " << result.n << "\t ";
		cout << setw(15) << setprecision(14) << result.abs_err << "\t";
		cout << setw(15) << setprecision(14) << result.rem_term << endl;
	}
	cout << endl;
}

void table_2(double a, double b) {
	double h = (b - a) / 10, x = a; // (a + b) / 2;
	int i, n = 5;// val_with_prec(x, 1e-8).n;
	Result result;

	cout << "  Xi\t Absolute Error\t\tRemainder term\n";

	for (i = 0; i < 10; ++i) {
		x = a + h*i;
		result = abs_error(x, n);
		cout << x << "\t " << setw(15) << setprecision(14) << result.abs_err << " \t";
		cout << setw(15) << setprecision(14) << result.rem_term << endl;
	}
	cout << endl;
}
