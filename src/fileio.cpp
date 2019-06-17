#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstring>
#include "fileio.hpp"
#include "winscreenio.hpp"

// file  and console I/O function definitions
void initialize(int* field_choices, int n)
{
    for (int i = 0; i < n; i++) field_choices[i] = 0;
}

// int function fills selection array with user choices
void getChoices(int* field_choices, int n)
{
    int no_choices = 0;
    std::string tmp;
    std::stringstream ss;

    getline(std::cin, tmp);
    ss << tmp;
	while (ss.good() && no_choices < n) {
		ss >> field_choices[no_choices++];
		// ss.ignore(1, ',');
		ss.ignore();	// this works
	}
}

void getFields(std::vector<Type>& fields, const Type& record)
{
	int i = 0, j = 0;

	while (i < record.length()) {
		fields.push_back("");
		while (record[i] != ',' && i < record.length())
			if (record[i] != '\"')
				fields[j] += record[i++];
			else {	// we're escaped right?
				i++;	// step through the quote
				while (record[i] != '\"') 
					fields[j] += record[i++];
				i++;	// step through the quote
			}
		j++;
		i++;
	}
}	

void getField(std::string& field, const std::string& record, int n)
{
	int i = 0, j = 0;

	field.clear();
	while (i < record.length()) {
		while (record[i] != ',' && i < record.length())
			if (record[i] != '\"')
				if (j + 1 == n)
					field += record[i++];
				else
					i++;
			else {
				i++;
				while (record[i] != '\"') {
					if (j + 1 == n)
						field += record[i++];
					else
						i++;
				}
				i++;
			}
		if (j + 1 == n) return;
		i++;
		j++;
	}
	field.clear();
}

void getField(char* field, const char* record, int n)
{
	int i = 0, j = 0, idx = 0;

	memset(field, 0, FLDLEN);
	while (i < RECSIZ && idx < FLDLEN) {
		while (record[i] != ',' && i < RECSIZ && idx < FLDLEN) {
			if (record[i] != '"') {
				if (j + 1 == n)
					field[idx++] = record[i++];
				else
					i++;
			} 
			else {
				i++;
				while (record[i] != '"') {
					if (j + 1 == n)
						field[idx++] = record[i++];
					else
						i++;
				}
				i++;
			}
		}
		if (j + 1 == n) return;
		i++;
		j++;
	}
	memset(field, 0, FLDLEN);
}

bool containsComma(const std::string& str)
{
	int n = str.length();
	while (str[n] != ',' && n > -1)
		n--;
	if (n > 0)
		return true;
	else
		return false;
}

// so's we's know's we's can's do's it's
void writeTable(std::fstream& sequential, const std::vector<Type>& relation)
{
	int maxima[NOFLDS];
	std::vector<Type> flds;
	std::vector< std::vector<Type> > table;
	std::string stars(75, '*');

	for (int i = 0; i < NOFLDS; i++) maxima[i] = 0;
	// get the header
	getFields(flds, relation[0]);
	table.push_back(flds);
	flds.clear();
	// but it doesn't contribute to computing maximum field widths
	for (int i = 1; i < relation.size(); i++) {
		getFields(flds, relation[i]);
		table.push_back(flds);
		// compute field widths
		for (int j = 0; j < flds.size(); j++)
			if (flds[j].length() > maxima[j]) maxima[j] = flds[j].length();
		flds.clear();
	}
	std::stringstream hdr;
	// for (int j = 0; j < table[0].size(); j++) hdr << ' ' << std::setw(maxima[j]) << std::left << table[0][j].substr(0, maxima[j]);
	for (int j = 0; j < table[0].size(); j++) 
		hdr << ' ' << std::setw(maxima[j]) << std::left << table[0][j].substr(0, maxima[j]) << ' ';
	sequential << hdr.str() << std::endl;
	for (int i = 1; i < relation.size(); i++) {
		sequential << ' ' << std::setw(maxima[0]) << std::right << std::setfill('0') << table[i][0] << ' ';
		sequential << std::setfill(' ');
		for (int j = 1; j < table[i].size(); j++) 
			sequential << ' ' << std::setw(maxima[j]) << std::left << table[i][j] << ' ';
		sequential << std::endl;
	}
}
