#pragma once
#include <string>
using namespace std;
class CToken {
private:
	
	
public:
	int tokenType = 0;
	bool isSpecial = 0;
	void setTokenType(int _tokenType) {
		tokenType = _tokenType;
	}



	string tokenValue;

	string tokenValueName() {

		if (tokenType) {
			if (tokenType == 1) {
				return "word";
			}
			else if (tokenType == 2)
			{
				return "integer";
			}
			else if (tokenType == 3) {
				return "real";
			}
			else if (tokenType == 4) {
				return "special";
			}
		}
		return NULL;
	}


};