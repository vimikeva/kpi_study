/*!
* file: Employee.h
* Copyright (c) 2015 by Yu.Zorin
*/

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

//An abstract class
class Person{
public:
	Person(){};
	Person(string _f_name, string _l_name, int _age): f_name(_f_name), l_name(_l_name), age(_age){};
	virtual void Display(bool) = 0;
protected:
	string f_name; // first name
	string l_name; // last name
	int age;
};

class Employee : public Person{
public:
	Employee(){};
	Employee(string _f_name, string _l_name, int _age, int _id) : Person(_f_name, _l_name, _age), id(_id) { salary = 0; };
	Employee(const Employee &e);
	Employee& operator = (const Employee &e);
	void SetSalary(int s);
	void SetDepartment(string dept);
	void SetId(int n);
	int GetId() const;
	string GetDepartment() const;
	void Display(bool bl) override;

protected:
	string department;
	int salary;
	int id;
};