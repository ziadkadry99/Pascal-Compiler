#pragma once
#include "CLex.h"
#include "CToken.h"
#include <string>
class TDCParser {


	CToken curToken;
	int index = 0;
	int parsedCount;
	CLex *Lexer;
	
private:

	void ParseError() {
		cout << "Parser error." << endl;
		exit(0);
	}

	void E() {
		C();
	}

	void C() {
		CR();
		CPrime();
	}

	

	void CPrime() {
			if (curToken.tokenValue == "<" || curToken.tokenValue == ">" || curToken.tokenValue == "=") {
				if (Lexer->GetToken(curToken)) {
					parsedCount++;
					CR();
					CPrime();
				}
				

			}
	}

	void CR() {
		T();
	}

	void T() {
		TR();
		TPrime();
	}

	void TPrime() {
		if (curToken.tokenValue == "+" || curToken.tokenValue == "-") {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;
				TR();
				TPrime();
			}
			else {
				ParseError();
			}
			
		}
	}

	void TR() {
		F();
	}

	void F() {
		FR();
		FPrime();
	}

	void FPrime() {
		if (curToken.tokenValue == "*" || curToken.tokenValue == "/") {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;
				FR();
				FPrime();
			}
			else {
				ParseError();
			}

		}
	}

	void FR() {
		if (curToken.tokenValue == "(") {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;
				E();
				if (curToken.tokenValue == ")") {
					if (Lexer->GetToken(curToken)) {
						parsedCount++;
					}
				}
				else {
					ParseError();
				}
			}

		}
		else if (curToken.tokenValueName() == "real") {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;
			}
		}
		else if (curToken.tokenValueName() == "integer") {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;
			}
		}
		
		else if (curToken.tokenValueName() == "word" && !curToken.isSpecial) {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;
			}
		}
		else if (curToken.tokenValue == "+") {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;
				TR();
			}
			else {
				ParseError();
			}
		}
		else if (curToken.tokenValue == "-") {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;
				TR();
			}
			else {
				ParseError();
			}
		}
		else {
			ParseError();
		}
	}


	void Pascal() {
		Block();

		if (curToken.tokenValue == ".") {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;
			}
		}
		else {
			ParseError();
		}

	}

	void Block() {
		if (curToken.tokenValue == "begin") {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;

				Stat();
				MStat();
				if (curToken.tokenValue == "end") {
					if (Lexer->GetToken(curToken)) {
						parsedCount++;
					}
				}
				else {
					ParseError();
				}
			}
		}
		else {
			ParseError();
		}
	}

	void Stat() {
		if (curToken.tokenType == 1 && !curToken.isSpecial) {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;
				if (curToken.tokenValue == ":=") {
					if (Lexer->GetToken(curToken)) {
						parsedCount++;
						E();
					}
				}
				else {
					ParseError();
				}
			}
			else {
				ParseError();
			}

		}
		else if (curToken.tokenValue == "while") {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;
				E();

				parsedCount++;
				if (curToken.tokenValue == "do") {
					if (Lexer->GetToken(curToken)) {
						parsedCount++;
						Stat();
					}
				}
				else {
					ParseError();
				}

				
			}
		}
		else if (curToken.tokenValue == "begin") {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;
				Stat();
				MStat();
				if (curToken.tokenValue == "end") {
					if (Lexer->GetToken(curToken)) {
						parsedCount++;
					}
				}
				else {
					ParseError();
				}
			}
		}
		else if (curToken.tokenValue == "if") {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;
				E();
				if (curToken.tokenValue == "then") {
					if (Lexer->GetToken(curToken)) {
						parsedCount++;
						Stat();
						StatPrime();
					}
				}
				else {
					ParseError();
				}
			}
		}
		
	}

	void StatPrime() {
		if (curToken.tokenValue == "else") {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;
				Stat();
			}
		}
	}

	void MStat() {
		if (curToken.tokenValue == ";") {
			if (Lexer->GetToken(curToken)) {
				parsedCount++;
				Stat();
				MStat();
			}
		}
	}



public:

	bool Parse(CLex *_lexar) {
		Lexer = _lexar;
		Lexer->GetToken(curToken);
		

		if (curToken.tokenValue != "") {
			Pascal();
		}
		else {
			ParseError();
			return false;
		}

		if (curToken.tokenValue != "")
		{
			ParseError();
			return false;
		}
		cout << "Surprisingly, NO parser errors found" << endl;
		return true;
	}


};