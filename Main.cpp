

#include <iostream>
#include <string>
#include <fstream>
#include "CLex.h"
#include "CToken.h"
#include "CBUParser.h"
#include "TDCParser.h"
using namespace std;

int main()

{
	CLex LEX("code.txt");
	TDCParser Parser;
	Parser.Parse(&LEX);
	system("pause");
	return 0;

}

