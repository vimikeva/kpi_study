/*!
* file: Manager.h
* written: 05/02/2015
* synopsis: Manager class declaration
* Copyright (c) 2015 by Yu.Zorin
*/
#pragma once
#include "Employee.h"
#include <list>

class Manager : public Employee{
public:
	Manager(){};
	Manager(string _f_name, string _l_name, int _age, int _id) : Employee(_f_name, _l_name, _age, _id){};
	Manager(const Manager &m);
	Manager& operator = (const Manager &m);
	Person* AddSubordinate(Person *p);
	void DellSubordinate(int id);
	void DisplaySubordinates();
	void Display(bool) override;

private:
	list <Person *> subordinates; // список підлеглих
};
