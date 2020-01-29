#pragma once
#include <string>
#include <windows.h>
#include <fstream>
#include <iostream>
#include "CToken.h"
#include <map>
#include <stack>
#include <string>
using namespace std;

class CBUParser {
private:
	stack<string> stack;
	map<string, int> productions;
	map<string, int> keyMap;
	string* reverseKeyMap; // For debugging purposes 
	char** OPC;
	bool debugMode = false;


	bool LoadProductions(string _filename) {
			fstream productionsFile;
			productionsFile.open(_filename);

			string line;
			int tempSize;
			string fProdPart, sProdPart;
			while (!productionsFile.eof()) {

				productionsFile >> line;
				tempSize = line.size();
				fProdPart = line.substr(0, 2);
				sProdPart = line.substr(2, tempSize);

				productions[sProdPart] = stoi(fProdPart);

				if (debugMode) {
					cout << "Production: " << sProdPart << " Reduces to: " << fProdPart << endl;
				}
			}

			if (productions.size() != 0) {
				if (debugMode) {
					cout << "Productions Loaded successfully" << endl;
				}
				return true;
			}
			else {
				if (debugMode) {
					cout << "Productions Failed to load" << endl;
				}
				return false;
			}
	}

	bool LoadKeyMap(string _filename) {
		fstream KeyMapFile;
		KeyMapFile.open(_filename);

		int size = 0;
		string line;

		while (getline(KeyMapFile, line)) {
			size++;
		}

		KeyMapFile.clear();
		KeyMapFile.seekg(0, ios::beg);

		if (debugMode) {
			cout << size << "Keys found in file" << endl;
		}
		
		reverseKeyMap = new string[size];
		int i = 1;
		while (!KeyMapFile.eof()) {

			KeyMapFile >> line;
			
			if (debugMode) {
				cout << "Key \"" << line << "\" is mapped to " << i << endl;
			}
			reverseKeyMap[i - 1] = line;
			keyMap[line] = i++;

		}

		return true;
	}

	void LoadOPC(string _filename) {

		fstream OPCFile;
		OPCFile.open(_filename);
		int size = 0;
		string line;

		while (getline(OPCFile, line)) {
			size++;
		}

		OPCFile.clear();
		OPCFile.seekg(0, ios::beg);

		OPC = new char* [size];
		for (int i = 0; i < size; ++i)
			OPC[i] = new char[size];

		if (debugMode) {
			cout << "OPC Table of size " << size << "*" << size << " found in the file" << endl;
		}

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				char read = 0;
				OPCFile >> read;
				OPC[i][j] = read;

				if (debugMode) {
					cout << read << " stored in spot [" << i << "][" << j << "] in the OPC Array" << endl;
				}

			}
		}
		if (debugMode) {
			cout << "OPC Table Read: " << endl;
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					cout << OPC[i][j] << "	";

				}
				cout << endl;
			}
		}

	}


	string GetTokenValue(CToken _token){
		if (_token.tokenValueName() == "integer") {
			return _token.tokenValueName();
		}
		else if (_token.tokenValueName() == "real") {
			return _token.tokenValueName();
		}
		else if(_token.tokenValueName() == "special"){
			if (_token.tokenValue == "+") {
				return _token.tokenValue;
			}else if(_token.tokenValue == "-") {
				return _token.tokenValue;
			}
			else if (_token.tokenValue == "*") {
				return _token.tokenValue;
			}
			else if (_token.tokenValue == "/") {
				return _token.tokenValue;
			}
			else if (_token.tokenValue == "(") {
				return _token.tokenValue;
			}
			else if (_token.tokenValue == ")") {
				return _token.tokenValue;
			}
			else {
				return "none";
			}
		}
		else if (_token.tokenValue == "$") {
			return "$";
		}
		else {
			return "none";
		}
	}

			
public:

	CBUParser(string _productionFileName, string _keyMapFileName, string _OPCFile, bool _debugMode = false)
	{
		debugMode = _debugMode;
		LoadProductions(_productionFileName);
		LoadKeyMap(_keyMapFileName);
		LoadOPC(_OPCFile);
	}

	bool Parse(CToken _incomingToken, bool empty = false) {
		string tokenValue;
		if (empty) {
			tokenValue = '$';
		}
		else {
			tokenValue = GetTokenValue(_incomingToken);
		}
		
		
		if (tokenValue != "none") {
			bool shifted = false;
			int topOfStack;
			int size;
			char operation;
			string toReduce;
			int toPutOn;
			string toPutOnString;

			while (!shifted) {
				if (debugMode) {
					cout << "Stack now contains: " << endl << endl;
					for (std::stack<string> dump = stack; !dump.empty(); dump.pop()) {
						std::cout << dump.top() << '\n';
					}
					cout << endl;
					for (std::stack<string> dump = stack; !dump.empty(); dump.pop()) {
						for (int i = 0; i < dump.top().size() - 2; i += 2) {							
						}
						
					}					
					for (std::stack<string> dump = stack; !dump.empty(); dump.pop()) {
						cout << "< ";
						if (dump.top().size() > 2) {
							for (int i = 0; i < dump.top().size() - 1; i += 2) {
								if (i > 0) {
									cout << "= ";
								}
								cout << reverseKeyMap[stoi(dump.top().substr(i, 2)) - 1] << " ";
							}
						}
						else {
							cout << reverseKeyMap[stoi(dump.top()) - 1] << " ";
						}

						
					}

					cout << endl;
					cout << endl;
					cout << endl;
					cout << endl;
					cout << endl;
				}
				if (!stack.empty()) {
					size = stack.top().size();
					topOfStack = std::stoi(stack.top().substr(size - 2, 2));
					operation = OPC[topOfStack - 1][keyMap[tokenValue] - 1];

					if (debugMode) {
						cout << "Top of the stack contains: " << topOfStack << " Driving token is: " << tokenValue << endl << endl;
						cout << "Operation returned from OPC table is: " << operation << endl << endl;
					}
				}
				else {
					
					operation = OPC[keyMap["$"] - 1][keyMap[tokenValue] - 1];
					if (debugMode) {
						cout << "Top of the stack contains: " << "$" << " Driving token is: " << tokenValue << endl << endl;
						cout << "Operation returned from OPC table is: " << operation << endl << endl;
					}
				}
				if (operation == '>') {

					
					toReduce = stack.top();
					stack.pop();
					if (debugMode) {
						cout << toReduce << " Removed from the stack and will be reduced" << endl << endl;

					}
					while (toReduce.size() < 12)  
					{
						toReduce += "00";
					}
					toPutOn = productions[toReduce];

					toPutOnString = to_string(toPutOn);

					if (toPutOnString.size() < 2)  
					{
						toPutOnString = "0" + toPutOnString;
					}

					//here is the problem
					
					if (stack.size() > 0) {
						size = stack.top().size();
						topOfStack = std::stoi(stack.top().substr(size - 2, 2));
						char operation2 = OPC[topOfStack - 1][toPutOn - 1];

						if (operation2 == '<') {
							stack.push(toPutOnString);
						}
						else if (operation2 == '=') {
							stack.top() += toPutOnString;
						}
					}
					else {
						stack.push(toPutOnString);
					}

					/////////////////////
					
					
					if (debugMode) {
						cout << toPutOnString << " Was obtained from the productions and is pushed to the stack" << endl << endl;

					}

					if (toPutOnString == "00") {
						cout << "Parse Error." << endl << endl;
						return false;

					}
				}
				else if (operation == '<' || operation == '=') {
					toPutOn = keyMap[tokenValue];
					toPutOnString = to_string(toPutOn);
					if (debugMode) {
						cout << operation << " Shift will be performed with " << toPutOnString << endl << endl;

					}
					if (toPutOnString.size() < 2)  // make the string two characters long
					{
						toPutOnString = "0" + toPutOnString;
					}
					if (operation == '<') {
						stack.push(toPutOnString);
						if (debugMode) {
							cout << "New production " << toPutOnString << " was added to the top of the stack" << endl << endl;

						}
					}
					else if (operation == '=') {
						stack.top() += toPutOnString;
						if (debugMode) {
							cout << "New production " << toPutOnString << " was appended to the already existing production on the top of the stack" << endl << endl;
						}
					}
					shifted = true;
				}
				else if (operation == 'A') {
					if (stack.size() == 1 && stack.top().size() == 2) {
						cout << "Bottom up parser accepts the statement." << endl;
						stack.pop();
						shifted = true;
						return true;
					}
					cout << "Parse Error." << endl << endl;
					return false;
				}
				else {
					cout << "Parse Error." << endl << endl;
					return false;
				}

			}


		}
		
		return true;
	}

	

};
