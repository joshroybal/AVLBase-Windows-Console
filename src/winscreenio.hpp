#ifndef _SCREEN_IO_
#define _SCREEN_IO_

#include <vector>
#include <Windows.h>
typedef std::string Type;

// global function prototypes
int getNoFlds(const Type&);
void displayMenu(const Type&);
void screenRecord(const Type&);
void displayLine(const Type&);
void ClearScreen();
void screenReport(const std::vector<Type>& relation);
int maximum(int, int);
int minimum(int, int);

#endif
