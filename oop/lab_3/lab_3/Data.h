#pragma once
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class CData{
public:
	CData() = default;
	CData(string dest, string fl, string pname, string fldate);
	~CData() {};

	CData& operator = (CData& d);
	bool operator == (CData& d);
	bool operator < (CData& d);
	bool operator > (CData& d);
	friend ostream& operator << (ostream& stream, CData data);
	friend istream& operator >> (istream& stream, CData& data);

private:
	string m_Destination;
	string m_Flight;
	string m_Passengername;
	string m_Flightdate;
};
