#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <string>


using namespace std;

class CFile {
public:
	CFile();
	CFile(string date);
	CFile(char *, char *, int);
	CFile(CFile&);
	~CFile(){};

	void rename(const string newname);

	friend ostream& operator << (ostream& stream, const CFile& data);
	friend istream& operator >> (istream& stream, CFile& data);
	CFile& operator = (const CFile& data);
	bool operator == (const CFile& data) const;
	bool operator != (const CFile& data) const;
	bool operator < (const CFile& data) const;

private:
	char *_filename;
	char *_creation_date;
	int _size;
};
