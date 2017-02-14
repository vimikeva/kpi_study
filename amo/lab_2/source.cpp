#include "methods.h"

using namespace std;

int main() {
	double x, a, b;

	a = 4;
	b = 6;
	x = 5.08202;
	cout << "When a = " << a << "; b = " << b << "; x = " << x << endl;
	cout << "\t\tIteration Method\n\n";
	table_iter(a, b, x);
	cout << "\t\tBisection Method\n\n";
	table_bis(a, b);
	cout<<"\tComparison Table of the number of iteration\n\n";
	table_counts(a, b, x);
	cout << "-------------------------------------------------------------\n";

	a = 8;
	b = 9;
	x = 8.47842;
	cout << "When a = " << a << "; b = " << b << "; x = " << x << endl;
	cout << "\t\tIteration Method\n\n";
	table_iter(a, b, x);
	cout << "\t\tBisection Method\n\n";
	table_bis(a, b);
	cout << "\tComparison Table of the number of iteration\n\n";
	table_counts(a, b, x);
	cout << "-------------------------------------------------------------\n";

	a = 9;
	b = 11;
	x = 10.0569;
	cout << "When a = " << a << "; b = " << b << "; x = " << x << endl;
	cout << "\t\tIteration Method\n\n";
	table_iter(a, b, x);
	cout << "\t\tBisection Method\n\n";
	table_bis(a, b);
	cout << "\tComparison Table of the number of iteration\n\n";
	table_counts(a, b, x);
	cout << endl;

	
	return 0;
}