#pragma once
#include <string>
#include <windows.h>
#include <fstream>
#include <iostream>
#include "CToken.h"
#include <algorithm>
#include <cctype>
using namespace std;

//Lexer, reads the input file to be compiled and packages the contents into tokens

class CLex {
	static const int DFAROWSIZE = 128;
	static const int DFACOLSIZE = 12;
	int DFA[DFAROWSIZE][DFACOLSIZE];
	string specialWords[7] = { "begin", "end", "if", "then", "else", "while", "do" };

private:

	fstream fileToLex;
	int getType(int ps) {
		switch (ps)
		{
		case 1:

			return 1;
			break;
		case 2:

			return 2;
			break;
		case 4:

			return 3;
			break;
		case 9:

			return 3;
			break;
		case 10:

			return 4;
			break;
		case 11:

			return 4;
			break;
		default:
			break;
		}
		return 0;
	}
public:

	void LoadDFA() {

		fstream dfaFile;
		dfaFile.open("v1.dfa");
		for (int i = 0; i < DFAROWSIZE; i++) {
			for (int j = 0; j < DFACOLSIZE; j++) {
				int read = 0;
				string curElem = "";
				dfaFile >> read;
				DFA[i][j] = read;

			}
		}

	}


	CLex(string programPath) {
		fileToLex.open(programPath);
		LoadDFA();
	}
	//Checks if the token is a special word(reserved by the language)
	bool isSpecial(CToken token) {
		
		for (int i = 0; i < sizeof(specialWords) / sizeof(string) ; i++) {
			if ( token.tokenValue == specialWords[i]) {
				return true;
			}
		}
		return false;

	}
	
	char to_lowercase(char c) {
		if (c >= 'A' && c <= 'Z') {
			return c + 32;
		}
		return c;
	}

	string str_ToLowerCase(string _input) {
		for(char& c: _input) {
			c = to_lowercase(c);
		}
		return _input;
	}

	
	bool GetToken(CToken& token) {
		int currentState = 0;
		int previousState = 0;
		token.setTokenType(0);
		token.tokenValue = "";
		token.isSpecial = false;

		while (!fileToLex.eof()) {

			char ch = fileToLex.get();
			previousState = currentState;

			currentState = DFA[ch][currentState];
			if (currentState != 0 && currentState != 55 && currentState != 99) {
				token.tokenValue += ch;
			}
			else if (currentState == 55)
			{
				token.tokenValue = str_ToLowerCase(token.tokenValue);
				token.setTokenType(getType(previousState));
				if (token.tokenType == 1) {
					token.isSpecial = isSpecial(token);
				}
				fileToLex.unget();
				return true;
			}
			else if (currentState == 99)
			{
				cout << "\nLex error!" << endl;
				return false;
			}

		}

		token.tokenValue = str_ToLowerCase(token.tokenValue);
		if (token.tokenValue == "") {
			return false;
		}
		else {
			token.setTokenType(getType(previousState));
			
			if (token.tokenValue.length() > 1) {
				token.tokenValue.pop_back();
			}
			if (token.tokenType == 1) {
				token.isSpecial = isSpecial(token);
			}
			return true;
		}

		

	}
};
