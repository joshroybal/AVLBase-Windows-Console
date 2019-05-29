#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "winscreenio.hpp"
#include "fileio.hpp"

int getNoFlds(const Type& hdr)
{
	std::vector<Type> flds;
	getFields(flds, hdr);
	return flds.size();
}

void displayMenu(const Type& header)
{
	// int width = 75;
	char c = '*';
	Type stars(77, '*');
	std::vector<Type> flds;
	getFields(flds, header);
	int nf = flds.size();

	displayLine(stars);
	displayLine(" 0 - terminate processing");
	for (int i = 0; i < nf; i++) {
		Type menuItem = " " + std::to_string(i + 1) + " - ";
		menuItem += flds[i];
		displayLine(menuItem);
	}
	displayLine(stars);
}

void screenRecord(const Type& csvline)
{
	int width = 77;
	char c = '*';
	Type stars;
	std::vector<Type> flds;

	stars.assign(width, c);
	getFields(flds, csvline);
	int nf = flds.size();
	std::cout << std::endl;
	displayLine(stars);
	for (int i = 0; i < nf; i++) 
		displayLine(" " + flds[i]);
	displayLine(stars);
	std::cout << std::endl;
}

// provide the string with a leading blank yourself if you insist
void displayLine(const Type& str)
{
	Type tmp = str;
	tmp = "*" + str;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x1f);
	std::cout << std::setw(78) << std::left << tmp << '*';
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	std::cout << std::endl;
}

void screenReport(const std::vector<Type>& relation)
{
	int maxima[NOFLDS];
	std::vector<Type> flds;
	std::vector< std::vector<Type> > table;
	Type stars(77, '*');

	for (int i = 0; i < NOFLDS; i++) maxima[i] = 0;

	// get the header but
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
	displayLine(stars);	// we're not here unless there're matches
	std::stringstream hdr;
	for (int j = 0; j < table[0].size(); j++) 
		hdr << ' ' << std::setw(maxima[j]) << std::left << table[0][j].substr(0, maxima[j]) << ' ';
	displayLine(hdr.str().substr(0, 77));
	int last = minimum((int)relation.size(), 8);
	int first = maximum(8, (int)relation.size() - 8);
	for (int i = 1; i < last; i++) {
		std::stringstream ss;
		ss << ' ' << std::setw(maxima[0]) << table[i][0] << ' ';
		for (int j = 1; j < table[i].size(); j++) 
			ss << ' ' << std::left << std::setw(maxima[j]) << table[i][j] << ' ';
		displayLine(ss.str().substr(0, 77));
	}
	if (first > last && first < relation.size()) std::cout << " . . ." << std::endl;
	for (int i = first; i < relation.size(); i++) {
		std::stringstream ss;
		ss << ' ' << std::setw(maxima[0]) << table[i][0] << ' ';
		for (int j = 1; j < table[i].size(); j++) 
			ss << ' ' << std::left << std::setw(maxima[j]) << table[i][j] << ' ';
		displayLine(ss.str().substr(0, 77));
	}
	displayLine(stars);	// we're not here unless there're matches
}

void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

int maximum(int a, int b)
{
	if (a >= b) return a;
	else return b;
}

int minimum(int a, int b)
{
	if (a <= b) return a;
	else return b;
}