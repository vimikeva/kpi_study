/*!
* file: Database.h
* written: 05/03/2015
* synopsis: Database class definition
* Copyright (c) 2015 by Yu.Zorin
*/
#pragma once
#include "Manager.h"
#include <vector>

class DataBase{
public:
	DataBase(){};
	~DataBase(){}; // no need in destructor
	//creates "flat" database
	bool LoadFromFile(const char *file);
	//arrages "flat" database after loading from the file
	void ArrangeSubordinates();
	//hire a new employee
	Person* HireEmployee(Person *p);
	void DisplayDepartmentEmployees(string _department);
	//fire the employee
	bool FireEmployee(int id);
	void DisplayAll();

private:
	vector <Person *> employees;
};
