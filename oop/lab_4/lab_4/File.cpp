#include "File.h"

CFile::CFile() {
	_filename = NULL; 
	_creation_date = NULL;
	_size = 0;
}

CFile::CFile(string date){
	_filename = NULL;
	_size = 0;
	_creation_date = new char[date.length() + 1];
	strcpy(_creation_date, date.c_str());
}

CFile::CFile(char *_fn, char *_cd, int _s){
	_filename = new char[strlen(_fn) + 1];
	strcpy(_filename,_fn);
	_creation_date = new char[strlen(_cd) + 1];
	strcpy(_creation_date, _cd);
	_size = _s;
}

CFile::CFile(CFile &data){
	_filename = new char[strlen(data._filename) + 1];
	strcpy(_filename, data._filename);
	_creation_date = new char[strlen(data._creation_date) + 1];
	strcpy(_creation_date, data._creation_date);
	_size = data._size;
}

void CFile::rename(string newname){
	delete[] _filename;
	_filename = new char[newname.length() + 1];
	strcpy(_filename, newname.c_str());
}

ostream & operator<<(ostream & stream, const CFile& data){
	stream << data._filename << "\t\t";
	stream << data._creation_date << "\t\t";
	stream << data._size;
	return stream;
}

istream & operator>>(istream & stream,CFile & data){
	string str;
	getline(stream, str, ';');
	data._filename = new char[str.length() + 1];
	strcpy(data._filename, str.c_str());
	getline(stream, str, ';');
	data._creation_date = new char[str.length() + 1];
	strcpy(data._creation_date, str.c_str());
	getline(stream, str);
	data._size = stoi(str);
	return stream;
}

CFile & CFile::operator=(const CFile & data){
	if (this == &data) return *this;
	delete[] _filename;
	_filename = new char[strlen(data._filename) + 1];
	strcpy(_filename, data._filename);
	delete[] _creation_date;
	_creation_date = new char[strlen(data._creation_date) + 1];
	strcpy(_creation_date, data._creation_date);
	_size = data._size;
	return *this;
}

bool CFile::operator==(const CFile & data) const{
	return strcmp(_filename, data._filename) == 0;
}

bool CFile::operator!=(const CFile & data) const{
	return strcmp(_filename, data._filename) != 0;
}

bool CFile::operator<(const CFile & data) const{
	char *s1 = new char[strlen(_creation_date) + 1],
		*s2 = new char[strlen(data._creation_date) + 1];
	int res;
	strcpy(s1, _creation_date);
	strcpy(s2, data._creation_date);
	s1[2] = s1[5] = s2[2] = s2[5] = '\0';
	s1 += 6;
	s2 += 6;
	res = strcmp(s1, s2);
	if (res == 0) {
		s1 -= 3; s2 -= 3;
		res = strcmp(s1, s2);
		if (!res) {
			s1 -= 3; s2 -= 3;
			res = strcmp(s1, s2);
		}
		else{
			s1 -= 3;
			s2 -= 3;
		}
	}
	else{
		s1 -= 6;
		s2 -= 6;
	}

	delete[] s1;
	delete[] s2;
	return res < 0;
}
