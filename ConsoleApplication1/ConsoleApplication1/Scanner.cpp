#include <iostream>
#include <vector>

using namespace std;

struct Token {
	int stateIndex;
	char* tokenValue;
} token;

enum States { initial = 0, comment = 1, letter = 2, digit = 3, assignment = 4, finish = 5, symbol = 6, error = 7};
enum reserverWords { If, then, eLse, end, repeat, until, read, write };

bool isLetter(char a){
	if (((int)a >= 97 && (int)a <= 122) || ((int)a >= 65 && (int)a <= 90)){
		return true;
	}
	else{
		return false;
	}
}

bool isSymbol(char a){
	if (a == '+' || a == '-' || a == '=' || a == '*' || a == '/' || a == ';' || a == '<' || a == '>' || a == '(' || a == ')'){
		return true;
	}
	else{
		return false;
	}
}

int main() {
	vector<Token> tokens;
	States state = initial;
	States nextState = initial;
	States preState = finish;
	char* tinyCode = "read x; if  < x then {do not compute if x <= 0} fact := 1;  repeat fact := fact * x; x := x - 1 until x = 0; write fact {output factorial end} end";
	vector <char> token;
	for (int i = 0; i <= strlen(tinyCode)+1; i++){
		char a = tinyCode[i];
		if (state == initial){
			if (a == ' '){
				nextState = initial;
			}
			else if (isdigit(a)){
				token.push_back(a);
				nextState = digit;
			}
			else if (a == ':'){
				token.push_back(a);
				nextState = assignment;
			}
			else if (a == '{'){
				nextState = comment;
			}
			else if (isLetter(a)){
				token.push_back(a);
				nextState = letter;
			}
			else if (isSymbol(a)){
				preState = symbol;
				token.push_back(a);
				nextState = finish;
			}
			else{
				token.push_back(a);
				nextState = error;
			}
		}
		else if (state == comment){
			if (a == '}'){
				nextState = initial;
			}
			else{
				nextState = comment;
			}
		}
		else if (state == letter){
			if (isLetter(a)){
				token.push_back(a);
				nextState = letter;
			}
			else{
				i--;
				nextState = finish;
				preState = letter;
			}
		}
		else if (state == digit){
			if (isdigit(a)){
				token.push_back(a);
				nextState = digit;
			}
			else{
				i--;
				preState = digit;
				nextState = finish;
			}
		}
		else if (state == assignment){
			if (a == '='){
				token.push_back(a);
				preState = assignment;
				nextState = finish;
			}
			else{
				i--;
				preState = assignment;
				nextState = finish;
			}
		}
		else if (state == finish){
			for (int i = 0; i < token.size(); i++){
				cout << token.at(i);
			}
			cout << " " << preState;
			nextState = initial;
			cout << endl;
			token.clear();
			i--;
		}
		state = nextState;
	}
	system("pause");
	return 0;
}