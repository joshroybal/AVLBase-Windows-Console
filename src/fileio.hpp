#ifndef _FILE_IO_
#define _FILE_IO_

#include <vector>

// global constants
const int RECSIZ = 256;
const int NOFLDS = 20;
const int FLDLEN = 50;

// global type
typedef std::string Type;

// global function prototypes
void initialize(int*, int);
void getChoices(int*, int);
int get_fields(char (*)[FLDLEN], const char*);
void getFields(std::vector<std::string>&, const std::string&);
void get_field(char*, const char*, int);
void getField(std::string&, const std::string&, int);
bool containsComma(const std::string&);
void writeTable(std::fstream&, const std::vector<std::string>&);

#endif
