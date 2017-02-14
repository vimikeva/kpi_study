#include "Database.h"

int main(){
    DataBase db;
	Manager* empl2;
	Employee* empl;
    db.LoadFromFile("input.csv");
	db.DisplayAll(); 
	system("pause");
	cout << "----------------------------------------------------------------\n\n";

	db.ArrangeSubordinates();
	db.DisplayAll();
	system("pause");
	cout << "----------------------------------------------------------------\n\n";
	
	empl = new Employee("John", "Wolt", 19, 13);
	empl->SetDepartment("PZ");
	empl->SetSalary(600);
	if(!(db.HireEmployee(empl))) cout << "Error" << endl;
	empl2 = new Manager("Jon", "Snow", 21, 55);
	empl2->SetDepartment("PZ");
	empl2->SetSalary(2000);
	if (!(db.HireEmployee(empl2))) cout << "Error" << endl;
	db.DisplayAll();
	system("pause");
	cout << "----------------------------------------------------------------\n\n";

	db.FireEmployee(0);
	db.DisplayAll();
	system("pause");
	cout << "----------------------------------------------------------------\n\n";
	
	db.DisplayDepartmentEmployees("IT");
	
    return 0;
}
