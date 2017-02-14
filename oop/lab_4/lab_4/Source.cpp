#include "List.h"
#include "File.h"

int main() {
	List<CFile> Base;
	List<CFile> Trash;
	List<CFile>::iterator it;
	int i;
	bool flag = true;
	string choice_temp, ch_number;
	string str, str_temp;

	Base.load("base.csv");
	
	while (true) {
		// print Base
		cout << "Base:\n";
		if (Base.empty()) cout << "\tnone\n";
		else 
			for (it = Base.begin(), i = 1; it != Base.end(); ++it, ++i) {
				cout << " " << i << ".\t";
				(*it).Print();
			}
		cout << endl;

		// print Trash
		cout << "Trash:\n";
		if (Trash.empty()) cout << "\tnone\n";
		else 
			for (it = Trash.begin(), i = 1; it != Trash.end(); ++it, ++i) {
				cout << " " << i << ".\t";
				(*it).Print();
			}
		cout << endl;

		// choice
		cout << "Choice:\n 1. Rename\n 2. Remove\n 3. Restore\n 4. Remove obsolete relatively dates\n 5. Exit\n\nMake a choise: ";
		cin >> choice_temp;
		while (choice_temp < "1" || choice_temp > "5"){
			cout << "Error! Make a choice: ";
			cin >> choice_temp;
		}

		if (choice_temp == "1" || choice_temp == "2"){
			cout << "Choose from Base: ";
			cin >> ch_number;
			if (stoi(ch_number) == 0) cout << "Error!\n"; 
			else {
				i = 1;
				it = Base.begin();
				while (it != Base.end() && i < stoi(ch_number)) {
					++it;
					++i;
				}
				if (it == Base.end()) cout << "Error!\n";
				else {
					if (choice_temp == "1") {
						cout << "Enter new name: ";
						cin >> str;
						Base.rename(it, str);
					}
					if (choice_temp == "2")
						Base.splice(it, Trash);
				}
			}

		}
		else if(choice_temp == "3"){
			cout << "Choose from Trash: ";
			cin >> ch_number;
			if (stoi(ch_number) == 0) cout << "Error!\n";
			else {
				i = 1;
				it = Trash.begin();
				while (it != Trash.end() &&	i < stoi(ch_number)) {
					++it;
					++i;
				}
				if (it == Trash.end()) cout << "Error!\n";
				else if (choice_temp == "3")
						Trash.splice(it, Base);
			}
		}
		else if(choice_temp == "4"){
			cout << "Enter date [DD.MM.RR]: ";
			cin >> str;
			Base.remove(str, Trash);
		}
		else if (choice_temp == "5") break;

		system("pause");
		system("cls");
	}

	
	return 0;
}
