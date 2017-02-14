#include "BinTree.h"
#include "Data.h"

int main() {
	CData *temp;
	CBinTree<CData> tree;
	tree.load_from_file("basa.csv");
	tree.print_all();
	cout << endl << endl;
	
	temp = new CData("Kiev", "165", "Vitovschik", "17.11.16");
	tree.add_node(*temp);
	tree.print_all();
	cout << endl << endl;

	tree.del_node(*temp);
	tree.print_all();
	cout << endl << endl;

	tree.print_criteria("013", "13.11.16");
	cout << endl << endl;
	tree.print_all();
	cout << endl << endl;

	return 0;
}
