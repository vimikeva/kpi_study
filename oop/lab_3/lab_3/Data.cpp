#include "Data.h"


CData::CData(string dest, string fl, string pname, string fldate) {
	m_Destination = dest;
	m_Flight = fl;
	m_Passengername = pname;
	m_Flightdate = fldate;
}

CData& CData::operator = (CData& d) {
	m_Destination = d.m_Destination;
	m_Flight = d.m_Flight;
	m_Passengername = d.m_Passengername;
	m_Flightdate = d.m_Flightdate;
	return *this;
}

bool CData::operator == (CData& d) {
	if (m_Flight == d.m_Flight && m_Flightdate == d.m_Flightdate)
		return true;
	return false;
}

bool CData::operator < (CData& d) {
	if (m_Passengername < d.m_Passengername)
		return true;
	return false;
}

bool CData::operator > (CData& d) {
	if (m_Passengername > d.m_Passengername)
		return true;
	return false;
}

ostream& operator << (ostream& stream, CData data) {
	stream << data.m_Passengername << "  ";
	stream << data.m_Destination << "  ";
	stream << data.m_Flight << "  ";
	stream << data.m_Flightdate << endl;
	return stream;
}

istream& operator >> (istream& stream, CData& data) {
	getline(stream, data.m_Destination, ';');
	getline(stream, data.m_Flight, ';');
	getline(stream, data.m_Passengername, ';');
	getline(stream, data.m_Flightdate);
	return stream;
}
