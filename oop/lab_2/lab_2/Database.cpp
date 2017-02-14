/*!
* file: Database.cpp
* written: 05/03/2015
* synopsis: Database class implementation
* Copyright (c) 2015 by Yu.Zorin
*/
#include "Database.h"
#define MAX_STR 255

bool DataBase::LoadFromFile(const char *file){
	Employee *empl;
	Manager *mang;
	string *str = new string[7];
	int i;
	ifstream fin(file);
	employees.clear();
	if (!fin.is_open()) return false;
	while(!fin.eof()){
		for (i = 0; i < 6; i++)
			getline(fin, str[i], ';');
		getline(fin, str[6]);
		if (atoi(str[0].c_str())){
			mang = new Manager(str[2], str[3], atoi(str[4].c_str()), atoi(str[1].c_str()));
			mang->SetDepartment(str[5]);
			mang->SetSalary(atoi(str[6].c_str()));		
			employees.push_back(mang);
		}
		else{
			empl = new Employee(str[2], str[3], atoi(str[4].c_str()), atoi(str[1].c_str()));
			empl->SetDepartment(str[5]);
			empl->SetSalary(atoi(str[6].c_str()));
			employees.push_back(empl);
		}
	}
	fin.close();
	return true;
}

void DataBase::ArrangeSubordinates(){
	vector<Person*>::iterator it = employees.begin(), p_it;
	while (it != employees.end()) {
		if (typeid(**it) == typeid(Manager))
			for (p_it = employees.begin(); p_it != employees.end(); ++p_it)
				if (typeid(**p_it) == typeid(Employee) &&
					dynamic_cast<Manager*>(*it)->GetDepartment() == dynamic_cast<Employee*>(*p_it)->GetDepartment())
					dynamic_cast<Manager*>(*it)->AddSubordinate(*p_it);
		++it;
	}
}

Person* DataBase::HireEmployee(Person *empl){
	vector <Person*>::iterator it = employees.begin();
	while (it != employees.end()) {
		if (typeid(*empl) == typeid(Manager) && typeid(**it) == typeid(Manager) &&
			dynamic_cast<Manager*>(*it)->GetDepartment() == dynamic_cast<Manager*>(empl)->GetDepartment())
			return nullptr;
		if (dynamic_cast<Employee*>(*it)->GetId() == dynamic_cast<Employee*>(empl)->GetId())
			return nullptr;
		++it;
	}
	employees.push_back(empl);
	ArrangeSubordinates();
	return empl;
}

bool DataBase::FireEmployee(int id){
	vector<Person*>::iterator it = employees.begin(), _it;
	while(it != employees.end()){
		if(id == dynamic_cast<Employee*>(*it)->GetId()){
			if (typeid(**it) == typeid(Employee))
				for (_it = employees.begin(); _it != employees.end(); ++_it) {
					if (typeid(**_it) == typeid(Manager) && dynamic_cast<Employee*>(*it)->GetDepartment() == dynamic_cast<Manager*>(*_it)->GetDepartment()) {
						dynamic_cast<Manager*>(*_it)->DellSubordinate(id);
						break;
					}
				}
			employees.erase(it);
			break;
		}
		++it;
	}
	return true;
}

void DataBase::DisplayDepartmentEmployees(string _department){
	vector<Person*>::iterator it = employees.begin();
	while(it != employees.end()){
		if(_department == dynamic_cast<Employee*>(*it)->GetDepartment())
			if (typeid(**it) == typeid(Manager))
				dynamic_cast<Manager*>(*it)->Display(false);
			else if (typeid(**it) == typeid(Employee))
				dynamic_cast<Employee*>(*it)->Display(false);
		++it;
	}
}

void DataBase::DisplayAll(){
	vector<Person*>::iterator it = employees.begin();
	while (it != employees.end()){
		(*it)->Display(0);
		++it;
	}
}
