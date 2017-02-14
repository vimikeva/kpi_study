#include "methods.h"
#define coef_A 1


double func(double x) {
	return 10 * coef_A * cos(11 * x) * cos(x) * sin(x);
}

double func_C(double x, int n) {
	double temp, t_n = x, t_n1 = 1;

	if (n == 0) return t_n1;
	if (n == 1) return t_n;

	for (double i = 1; i < n; i++) {
		temp = 2 * x* t_n - t_n1;
		t_n1 = t_n;
		t_n = temp;
	}
	return t_n;
}

double choice(double x, int n_a, int n_b = -1) {
	if (n_b == -1)
		return func_C(x, n_a) * func(x);
	return func_C(x, n_a) * func_C(x, n_b);
}

double simpson(double a, double b, double h, int n_a, int n_b = -1) {
	int n = (int)((b - a) / h);
	double simps = 0, x = a;

	h = (b - a) / n;
	if (n_b == -1)
		for (int i = 0; i <= n - 1; ++i, x+=h)
			simps += choice(x, n_a) + 4.0 * choice(x + h / 2, n_a) + choice(x + h, n_a);
	else
		for (int i = 0; i <= n - 1; ++i, x+=h)
			simps += choice(x, n_a, n_b) + 4.0 * choice(x + h / 2, n_a, n_b) + choice(x + h, n_a, n_b);

	return simps * h / 6;
}

double _integral(double a, double b, int n_a, int n_b = -1) {
	double eps = 1e-10;
	double h = pow(eps, 0.25), integ = 0, temp;

	if (n_b == -1)
		do {
			temp = integ;
			integ = simpson(a, b, h, n_a);
			h /= 2;
		} while (fabs(integ - temp) / integ > eps);
	else
		do {
			temp = integ;
			integ = simpson(a, b, h, n_a, n_b);
			h /= 2;
		} while (fabs(integ - temp) / integ > eps);

		return integ;
}

vector<double> method_gaussa(double a, double b, int n) {
	vector<vector<double>> matrix(++n);
	for (int i = 0; i < n; i++) {
		matrix[i].resize(n + 1);
		matrix[i][n] = _integral(a, b, i);
		for (int j = 0; j < n; j++)
			matrix[i][j] = _integral(a, b, i, j);
	}

	vector<double> vect;
	for (int i = 0; i < n; i++) {
		double main_elem = matrix[i][i];
		for (int j = 0; j < n + 1; j++)
			matrix[i][j] = matrix[i][j] / main_elem;

		for (int j = 0; j < n; j++) {
			if (i == j) continue;
			vect = matrix[i];
			transform(vect.begin(), vect.end(), vect.begin(), bind1st(multiplies<double>(), matrix[j][i]));
			transform(matrix[j].begin(), matrix[j].end(), vect.begin(), matrix[j].begin(), minus<double>());
		}
	}

	vector<double> x(n);
	for (int i = 0; i < n; i++)
		x[i] = matrix[i][n];
	return x;
}

double p_m (double x, double a, double b, vector<double> vect) {
	double y = 0;
	for (size_t i = 0; i < vect.size(); i++)
		y += vect[i] * func_C(x, i);
	return y;
}

void answer(double a, double b, double eps) {
	double x, y, h = sqrt(eps), _eps;
	int n = (int)((b - a) / h), iter = 1;
	vector<double> vect;
	h = (b - a) / n;

	do {
		vect = method_gaussa(a, b, iter);
		_eps = 0;
		x = a;
		for (int i = 0; i < n; i++, x += h) {
			y = func(x) - p_m(x, a, b, vect);
			_eps += y * y;
		}
		iter++;
		cout << sqrt(_eps / n) << endl;
	} while (sqrt(_eps / n) > eps);


	x = a;
	ofstream stream("result.txt");
	for (int i = 0; i < n + 1; ++i, x += h) {
		y = p_m(x, a, b, vect);
		stream << x << " ; " << y << endl;
	}
	stream.close();
}

