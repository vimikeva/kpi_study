/*!
 * file: StringList.h
 * StringList class declaration
 * written: 01/06/2012
 * Copyright (c) 2012 by Yu.Zorin
 */
 
#pragma once
#include <string.h>
#include <stdio.h>

class cString{
public:
	//constructors/destructor
	cString();
	cString( const char *psz ); 
	cString( const cString& stringsrc ); 

	~cString();

	//methods
	int Getlength( ) const;
	bool Isempty()const;
	void Empty();
	void SetAt( int nindex, char ch );
	int Compare( const cString& s ) const;
    int Find( char ch ) const;
  	int Find( char *pszsub ) const;
  
	cString Mid( int nfirst, int ncount) const;
	cString Left( int ncount ) const;	
	cString Right( int ncount ) const;

    //operators
    cString& operator =( const cString& stringsrc );
	const cString& operator =( const unsigned char* psz );	
	char operator [](int indx);

	cString operator +( const cString& string );
	cString& operator +=( const cString& string );
    //
	void Print();
private: 
    char *string;
    int size;
};
