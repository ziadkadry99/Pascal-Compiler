// KadryAssign5_335.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include "CLex.h"
//#include "CToken.h"
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

	/*CLex LEX("code.txt");
	

	CToken TOKEN;
	

	
	while (LEX.GetToken(TOKEN))

	{
		cout << TOKEN.tokenValueName() << " " << TOKEN.tokenValue << endl;

	}*/


	system("pause");

	return 0;

}

