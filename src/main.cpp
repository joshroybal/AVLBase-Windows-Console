// AVL class test driver program
// imitates some of the functionality of a relational database system
// Type is std::string
// this version will cap the record length at that of the longest csv line
// Type is std::string
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <iomanip>
#include <sstream>
#include "avl.hpp"
#include "fileio.hpp"
#include "winscreenio.hpp"

std::fstream sequential;
std::fstream direct;

int input(int[], int[], int[], Type&, Type&, int);
void process(std::vector<Type>&, int[], int[], int[], const Type&, int, int);
void output(const std::vector<Type>&);

int main(int argc, char *argv[])
{
	int select_fields[NOFLDS], find_fields[NOFLDS], order_fields[NOFLDS];
	char buf[RECSIZ];
	char fldbuf[FLDLEN];
	Type field, target, line, header;
	std::vector<Type> relation;

	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
		return 1;
	}

	// this is the beginning of the new approach I shall take
	sequential.open(argv[1], std::ios::in);
	getline(sequential, header);	// eat header before continuing
	int nf = getNoFlds(header);
	// in the first pass we have to determine the record length we want to fix for
	// the direct file
	int reclen = 0;
	int n = 0;
	int factor = 2;
	if (sequential.is_open()) {
		while (getline(sequential, line)) {
			if (n % factor == 0) {
				screenRecord(line);
				factor *= 2;
				// this is a screen delay loop
				float t1 = float(clock()) / CLOCKS_PER_SEC;
				while (true) {
					float t = float(clock()) / CLOCKS_PER_SEC;
					if (t - t1 > 0.25) break;
				}
				ClearScreen();
			}
			if (line.length() > reclen) reclen = line.length();
			n++;
		}
		sequential.close();
	}
	else {
		std::cout << "I/O ERROR" << std::endl;
		return 0;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x02);
	std::cout << " no. of records = " << n << std::endl;
	std::cout << " record length = " << reclen << std::endl;
	// now actually convert the records
	sequential.open(argv[1], std::ios::in);
	getline(sequential, line);	// eat header line before continuing
	direct.open("direct.dat", std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
	if (sequential.is_open()) {
		while (getline(sequential, line))
			direct.write(line.c_str(), reclen);
		sequential.close();
	}
	else {
		std::cout << "I/O ERROR" << std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
		return 0;
	}
	direct.close();
	Type reply("yes");
	while (reply[0] == 'y' || reply[0] == 'Y') {
		if (input(select_fields, find_fields, order_fields, target, header, nf)) return 1;
		// the header line for the select table may as well be constructed now by the computer
		int idx = 0;
		Type table_header("rec. no.");
		while (select_fields[idx]) {
			table_header += ",";
			getField(field, header, select_fields[idx++]);
			table_header += field;
		}
		relation.push_back(table_header);
		// process data based on user input
		process(relation, select_fields, find_fields, order_fields, target, reclen, nf);
		// ClearScreen();
		screenReport(relation);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x02);
		std::cout << ' ' << relation.size()-1 << " records in relation" << std::endl;
		std::cout << " output reports to files? (y/n) ";
		getline(std::cin, reply);
		if (reply[0] == 'y' || reply[0] == 'Y') {
			ClearScreen();
			std::cout << " writing reports" << std::endl;
			output(relation);
		}
		relation.clear();
		std::cout << " continue processing? (y/n) ";
		getline(std::cin, reply);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
	std::cout << " Copyright (c) 2015-2019 Josh Roybal" << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	return 0;
}

int input(int select_fields[], int find_fields[], int order_fields[], Type& target, Type& header, int nf)
{
	ClearScreen();
	displayMenu(header);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x02);
	std::cout << " select fields : ";
	initialize(select_fields, NOFLDS);   // zeroes out choice array
	getChoices(select_fields, NOFLDS);   // gets 'select' field choices from user	
	if (select_fields[0] == 0) {
		std::cout << " thank-you and good-bye" << std::endl;
		std::cout << "\033[0m";
		return 1;
	}
	std::cout << " search fields : ";
	initialize(find_fields, NOFLDS);    // zeroes out choice array
	getChoices(find_fields, NOFLDS);    // gets 'where domain=?' field choices from user
	std::cout << " enter search string : ";
	getline(std::cin, target);
	std::cout << " order fields : ";
	initialize(order_fields, NOFLDS);   // zeroes out choice array
	getChoices(order_fields, NOFLDS);   // gets 'order by' field choices from user
	return 0;
}

void process(std::vector<Type>& relation, int select_fields[], int find_fields[], int order_fields[], const Type& target, int reclen, int nf)
{
	char buf[RECSIZ];
	char fldbuf[FLDLEN];
	char keybuf[RECSIZ];
	Type record, field;
	AVL* BaseTree = new AVL();

	direct.open("direct.dat", std::ios::binary | std::ios::in);
	// read in the search keys
	int n = 0;
	float t1 = float(clock()) / CLOCKS_PER_SEC;
	while (direct.read(buf, reclen)) {
		memset(keybuf, 0, RECSIZ);
		int idx = 0;
		while (find_fields[idx] != 0) {
			getField(fldbuf, buf, find_fields[idx++]);
			if (idx) strcat(keybuf, ",");
			strcat(keybuf, fldbuf);
		}
		BaseTree->insert( std::string( keybuf ), ++n );
	}
	float t2 = float(clock()) / CLOCKS_PER_SEC;
	std::cout << " no. of records = " << n << std::endl;
	std::cout << ' ' << BaseTree->size() << " keys read into binary search tree in " << t2 - t1 << " seconds." << std::endl;
	std::cout << " height of tree = " << BaseTree->height() << std::endl;
	t1 = float(clock()) / CLOCKS_PER_SEC;
	std::vector<int> matchIdx = BaseTree->search(target);
	t2 = float(clock()) / CLOCKS_PER_SEC;
	std::cout << " AVL tree search completed in " << t2 - t1 << " seconds." << std::endl;
	t1 = float(clock()) / CLOCKS_PER_SEC;
	delete BaseTree;	// we don't need it any longer - so we delete it
	t2 = float(clock()) / CLOCKS_PER_SEC;
	std::cout << " AVL tree deleted in " << t2 - t1 << " seconds." << std::endl;
	std::cout << ' ' << matchIdx.size() << " matches found." << std::endl;
	if (matchIdx.size() == 0) {
		direct.close();
		return;
	}
	t1 = float(clock()) / CLOCKS_PER_SEC;
	direct.clear();	// clear it out of the end-of-file state
	// we don't need to read the matches into a new tree and reload the index to 
	// the matching nodes from it unless the user wants to sort her data
	if (order_fields[0] >= 1 && order_fields[0] <= nf) {
		AVL* MatchTree = new AVL();
		for (int i = 0; i < matchIdx.size(); i++) {
			direct.seekg((matchIdx[i] - 1) * reclen, std::ios::beg);
			direct.read(buf, reclen);
			record = buf;
			int idx = 0;
			// construct the key from the fields the user want to extract from the buf
			Type key;
			while (order_fields[idx] != 0) {
				if (idx) key += ",";
				getField(field, record, order_fields[idx++]);
				key += field;
			}
			MatchTree->insert(key, matchIdx[i]);
		}
		t2 = float(clock()) / CLOCKS_PER_SEC;
		std::cout << " matches inserted into AVL tree in " << t2 - t1 << " seconds." << std::endl;
		matchIdx.clear();	// clear it and re-use it
		MatchTree->loadIdxVector(matchIdx);
		delete MatchTree;	// we don't need it anymore - delete it
	}
	// grab the selected fields to write to the relation
	// our final relation is written to a CSV (comma delimited) format file
	t1 = float(clock()) / CLOCKS_PER_SEC;
	for (int i = 0; i < matchIdx.size(); i++) {
		direct.seekg((matchIdx[i] - 1) * reclen, std::ios::beg);
		direct.read(buf, reclen);
		record = buf;
		int idx = 0;
		// construct the key from the fields the user wants to extract from the record
		std::stringstream ss;
		ss << matchIdx[i];
		Type key(ss.str());
		while (select_fields[idx] != 0) {
			key += ",";
			getField(field, record, select_fields[idx++]);
			if (containsComma(field)) {
				key += "\"";
				key += field;
				key += "\"";
			}
			else
				key += field;
		}
		relation.push_back(key);
	}
	direct.close();	// all good things must come to an end
	t2 = float(clock()) / CLOCKS_PER_SEC;
	std::cout << " relation fields retrieved in " << t2 - t1 << " seconds." << std::endl;
}

void output(const std::vector<Type>& relation)
{
	float t1 = float(clock()) / CLOCKS_PER_SEC;
	sequential.open("report.csv", std::ios::out);
	for (int i = 0; i < relation.size(); i++) sequential << relation[i] << std::endl;
	sequential.close();
	float t2 = float(clock()) / CLOCKS_PER_SEC;
	std::cout << " output written to file report.csv in " << t2 - t1 << " seconds." << std::endl;
	t1 = float(clock()) / CLOCKS_PER_SEC;
	sequential.open("report.txt", std::ios::out);
	writeTable(sequential, relation);
	sequential.close();
	t2 = float(clock()) / CLOCKS_PER_SEC;
	std::cout << " output written to file report.txt in " << t2 - t1 << " seconds." << std::endl;
	// screenReport(relation);
}
