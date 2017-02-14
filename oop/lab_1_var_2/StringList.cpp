/*!
 * file: StringList.cpp
 * StringList class realization
 * written: 17/02/2016
 * Autor: Vitovschik
 */
#include "StringList.h"
 
cString::cString(){
    string = NULL;
    size = 0;
}

cString::cString(const char *psz){
    if (!psz){
        string = NULL;
        size = 0;
        return;
    }
    size = strlen(psz);
    string = new char[size + 1];
    strcpy(string, psz);
    string[size] = '\0';
}

cString::cString(const cString& stringsrc){
    if (!stringsrc.string){
        string = NULL;
        size = 0;
        return;
    }
    size = strlen(stringsrc.string);
    string = new char[size + 1];
    strcpy(string, stringsrc.string);
    string[size] = '\0';
}

cString::~cString(){
    delete[] string;
}

int cString::Getlength() const{
    return size;
}

bool cString::Isempty() const{
    if (size) return true;
    return false;
}

void cString::Empty(){
    delete[] string;
    string = NULL;
    size = 0;
}

void cString::SetAt(int nindex, char ch){
    if (nindex < 0 || nindex >= size || ch == '\0') return;
    string[nindex] = ch;
}

int cString::Compare(const cString& s) const{
    return strcmp(string, s.string);
}

int cString::Find(char ch) const{
    if (ch == '\0') return -1;
    char *str = strchr(string, ch);
    if (str) return str - string;
    return -1;
}

int cString::Find(char *pszsub) const{
    if(!pszsub) return -1;
    char *str = strstr(string, pszsub);
    if(str) return str - string;
    return -1;
}

cString cString::Mid(int nfirst, int ncount) const{
    cString str;
    int i;
    if (ncount > size)
        ncount = size - nfirst;
    str.size = ncount;
    str.string = new char[ncount + 1];
    for(i = nfirst; i < ncount; i++)
        str.string[i - nfirst] =  string[i];
    str.string[ncount] = '\0';
    return str;
}

cString cString::Left(int ncount) const{
    cString str;
    int i;
    if (ncount > size) ncount = size;
    str.size = ncount;
    str.string = new char[ncount + 1];
    for(i = 0; i < ncount; i++)
        str.string[i] = string[i];
    str.string[ncount] = '\0';
    return str;
}

cString cString::Right(int ncount) const{
    cString str;
    int i;
    if(ncount > size) ncount = size;
    str.size = ncount;
    str.string = new char[ncount + 1];
    for(i = 0; i < ncount; i++)
        str.string[i] = string[size - i - 1];
    str.string[ncount] = '\0';
    return str;
}

cString& cString::operator =(const cString& stringsrc){
    if (this == &stringsrc) return *this;
    delete[] string;
    size = stringsrc.size;
    string = new char[size+1];
    strcpy(string, stringsrc.string);
    string[size] = '\0';
    return *this;
}

const cString& cString::operator =(const unsigned char* psz){
    delete[] string;
    if (!psz){
        string = NULL;
        return *this;
    }
    size = strlen(string);
    string = new char[size+1];
    strcpy(string, (char*)psz);
    string[size] = '\0';
    return *this;
}

char cString::operator [](int indx){
    return string[indx];
}

cString cString::operator +(const cString& stringsrc){
    cString str;
    if (!stringsrc.size) return *this;
    str.string = new char[size + stringsrc.size + 1];
    if(size) strcpy(str.string, string);
    strcat(str.string, stringsrc.string);
    str.size =size + stringsrc.size;
    return str;
}

cString& cString::operator +=(const cString& stringsrc){
    char * str; = new char[size + stringsrc.size + 1];
    if (size) strcpy(str, string);
    delete[] string;
    string = str;
    str += size;
    if(stringsrc.size) strcpy(str, stringsrc.string);
    return *this;
}

void cString::Print(){
    if (string)
        printf("%s", string);
}
