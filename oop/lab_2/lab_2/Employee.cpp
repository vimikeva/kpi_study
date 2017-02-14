/*!
* file: Employee.cpp
* written: 05/02/2015
* synopsis: Employee class implementation
* Copyright (c) 2015 by Yu.Zorin
*/

#include "Employee.h"

Employee::Employee(const Employee &e){
	(*this) = e;
}

Employee& Employee::operator = (const Employee &e){
	if(this == &e) return *this;
	f_name = e.f_name;
	l_name = e.l_name;
	age = e.age;
	department = e.department;
	salary = e.salary;
	id = e.id;
	return *this;
}

void Employee::SetSalary(int s){
	salary = s;
}

void Employee::SetDepartment(string dept){
	department = dept;
}

void Employee::SetId(int n){
	id = n;
}

int Employee::GetId() const{
	return id;
}

string Employee::GetDepartment() const{
	return department;
}

void Employee::Display(bool bl){
	if(bl)cout << "\t\t";
	cout << "Employee type: employee\n";
	if (bl) cout << "\t\t";
	cout << "id: " << id << endl;
	if (bl)	cout << "\t\t";
	cout << f_name << " " << l_name << "  age: " << age << "  salary: " << salary << endl;
	if (!bl) cout << "department: " << department << endl;
	cout << endl;
}
