#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Token {
	int stateIndex;
	bool isReservedWord;
	vector<char> tokenValue;
	Token()
	{
		isReservedWord = false;
	}

	Token(int index, vector<char> value)
	{
		stateIndex = index;
		tokenValue = value;
	}

	bool operator == (const Token& token) const
	{
		if (token.stateIndex == stateIndex)
		{
			for (int i = 0; i < tokenValue.size(); i++)
			{
				if (tokenValue.at(i) == token.tokenValue.at(i))
				{

				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
		return true;
	}
} token;

namespace scannerOutput
{
	vector<Token> tokens;
}

namespace parser
{
	int index = 0;
}

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

bool isReserverWords(Token token)
{
	Token reserved;
	reserved.stateIndex = 2;
	vector<char> temp{ 'i', 'f' };
	reserved.tokenValue = temp;
	if (token == reserved)
	{
		return true;
	}
	reserved.tokenValue.clear();
	char a[4] = { 't', 'h', 'e', 'n' };
	reserved.tokenValue.insert(reserved.tokenValue.end(), a, a + 4);
	if (token == reserved)
	{
		return true;
	}

	reserved.tokenValue.clear();
	char a1[4] = { 'e', 'l', 's', 'e' };
	reserved.tokenValue.insert(reserved.tokenValue.end(), a1, a1 + 4);
	if (token == reserved)
	{
		return true;
	}

	reserved.tokenValue.clear();
	char a2[3] = { 'e', 'n', 'd' };
	reserved.tokenValue.insert(reserved.tokenValue.end(), a2, a2 + 3);
	if (token == reserved)
	{
		return true;
	}

	reserved.tokenValue.clear();
	char a3[6] = { 'r', 'e', 'p', 'e', 'a', 't' };
	reserved.tokenValue.insert(reserved.tokenValue.end(), a3, a3 + 6);
	if (token == reserved)
	{
		return true;
	}

	reserved.tokenValue.clear();
	char a4[5] = { 'u', 'n', 't', 'i','l' };
	reserved.tokenValue.insert(reserved.tokenValue.end(), a4, a4 + 5);
	if (token == reserved)
	{
		return true;
	}

	reserved.tokenValue.clear();
	char a5[4] = { 'r', 'e', 'a', 'd' };
	reserved.tokenValue.insert(reserved.tokenValue.end(), a5, a5 + 4);
	if (token == reserved)
	{
		return true;
	}

	reserved.tokenValue.clear();
	char a6[5] = { 'w', 'r', 'i', 't', 'e' };
	reserved.tokenValue.insert(reserved.tokenValue.end(), a6, a6 + 5);
	if (token == reserved)
	{
		return true;
	}
	return false;
}
vector<Token> scanner()
{
	vector<Token> tokens;
	States state = initial;
	States nextState = initial;
	States preState = finish;

	char* tinyCode = "read x; if (x<0) then {do not compute if x <= 0} fact := 1;  ahmedtrrte fact := fact * x; x := x - 1 until x = 0; write fact {output factorial end} end";
	vector <char> token;
	for (int i = 0; i <= strlen(tinyCode) + 1; i++){
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
			Token x;
			x.stateIndex = preState;
			x.tokenValue = token;
			if (x.stateIndex == letter)
			{
				if (isReserverWords(x))
				{
					x.isReservedWord = true;
				}
				else
				{
					x.isReservedWord = false;
				}
			}
			tokens.push_back(x);
			nextState = initial;
			//cout << endl;
			token.clear();
			i--;
		}
		state = nextState;
	}
	return tokens;
}

bool match(Token excepectedToken)
{
	
	if (scannerOutput::tokens.at(parser::index) == excepectedToken)
	{
		parser::index++;
		return true;
	}
	else
	{
		return false;
	}
}

bool matchTerminals(int state)
{
	if (parser::index < scannerOutput::tokens.size())
	{
		if (scannerOutput::tokens.at(parser::index).stateIndex == state)
		{
			parser::index++;
		}
		else
		{
			return false;
		}
		return true;
	}
	else
	{
		return true;
	}
	
}

bool matchSem()
{
	if (parser::index < scannerOutput::tokens.size())
	{
		if (scannerOutput::tokens.at(parser::index).tokenValue.at(0) == ';')
		{
			parser::index++;
		}
		else
		{
			return false;
		}
		return true;
	}
	else
	{
		return true;
	}
	
}

bool matchOp(int kind)
{
	if (parser::index < scannerOutput::tokens.size())
	{
		if (kind == 0) // mulOp
		{
			if (scannerOutput::tokens.at(parser::index).tokenValue.at(0) == '*' || scannerOutput::tokens.at(parser::index).tokenValue.at(0) == '/')
			{
				parser::index++;
				return true;
			}
			return false;
		}
		else if (kind == 1) // addOp
		{
			if (scannerOutput::tokens.at(parser::index).tokenValue.at(0) == '+' || scannerOutput::tokens.at(parser::index).tokenValue.at(0) == '-')
			{
				parser::index++;
				return true;
			}
			return false;
		}
		return false;
	}
	else
	{
		return true;
	}
}

bool matchCompareOp()
{
	if (parser::index < scannerOutput::tokens.size())
	{
		if (scannerOutput::tokens.at(parser::index).tokenValue.at(0) == '<' || scannerOutput::tokens.at(parser::index).tokenValue.at(0) == '=')
		{
			parser::index++;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
	
}

bool rules(int ruleIndex)
{
	cout << parser::index;
	cout << endl;
	cout << ruleIndex << endl;
	if (ruleIndex == 0) // factor ==> (exp) | number | identifier
	{
		
		vector<char> x{ '(' };
		token.tokenValue = x;
		Token token(6,x);
		if (match(token)){
			if (rules(6)) // match exp
			{
				vector<char> x1{ ')' };
				token.tokenValue = x1;
				Token token(6, x1);
				if (match(token))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		else if (matchTerminals(3)) // match number
		{
			return true;
		}
		else if (matchTerminals(2)) // match identifier
		{
			return true;
		}
		return false;
	}
	else if (ruleIndex == 1) // mulop ==> *|/
	{
		if (matchOp(0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (ruleIndex == 2) // term ==> factor {mulOp factor}
	{
		if (rules(0))
		{
			while (rules(1))
			{
				if (rules(0))
				{
					
				}
				else
				{
					return false;
				}
			}
			return true;
		}
		else 
		{
			parser::index--;
			return false;
		}
		return false;
	}
	else if (ruleIndex == 3) // addOp ==> +|-
	{
		if (matchOp(1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (ruleIndex == 4) // simple_exp ==> term {addOp term}
	{
		if (rules(2))
		{
			while (rules(3))
			{
				if (rules(2))
				{
					
				}
				else
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (ruleIndex == 5) // comparision op ==> < | =
	{
		if (matchCompareOp())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (ruleIndex == 6) // exp ==> simple_exp [comparision simple_exp] 
	{
		if (rules(4))
		{
			if (rules(5))
			{
				if (rules(4))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	else if (ruleIndex == 7) // write_stmt ==> write exp
	{
		Token write;
		string w = "write";
		vector<char> x;
		for (int i = 0; i < w.size(); i++)
		{
			x.push_back(w[i]);
		}
		write.stateIndex = 2;
		write.tokenValue = x;
		if (match(write))
		{
			if (rules(6))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else if (ruleIndex == 8) // read_stmt ==> read identifier
	{
		cout << "read\n";
		Token read;
		string w = "read";
		vector<char> x;
		for (int i = 0; i < w.size(); i++)
		{
			x.push_back(w[i]);
		}
		read.stateIndex = 2;
		read.tokenValue = x;
		if (match(read))
		{
			if (matchTerminals(2))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else if (ruleIndex == 9) // assign_stmt ==> identifier := exp
	{
		if (matchTerminals(2))
		{
			if (matchTerminals(4))
			{
				if (rules(6))
				{
					return true;
				}
				else
				{
					parser::index--;
					return false;
				}
			}
			else
			{
				parser::index--;
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else if (ruleIndex == 10) // repeat_stmt ==> repaeat stmt_seq until exp
	{
		Token repeat;
		string w = "repeat";
		vector<char> x;
		for (int i = 0; i < w.size(); i++)
		{
			x.push_back(w[i]);
		}
		repeat.tokenValue = x;
		repeat.stateIndex = 2;
		if (match(repeat))
		{
			if (rules(13))
			{
				Token until;
				w = "until";
				vector<char> x2;
				for (int i = 0; i < w.size(); i++)
				{
					x2.push_back(w[i]);
				}
				until.tokenValue = x2;
				until.stateIndex = 2;
				if (match(until))
				{
					if (rules(6))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else if (ruleIndex == 11) // if_stmt ==> if exp then stmt_seq [else stmt_seq] end
	{
		Token iF;
		string w = "if";
		vector<char> x;
		for (int i = 0; i < w.size(); i++)
		{
			x.push_back(w[i]);
		}
		iF.stateIndex = 2;
		iF.tokenValue = x;
		if (match(iF))
		{
			if (rules(6))
			{
				Token then;
				string w = "then";
				vector<char> x;
				for (int i = 0; i < w.size(); i++)
				{
					x.push_back(w[i]);
				}
				then.stateIndex = 2;
				then.tokenValue = x;
				if (match(then))
				{
					if (rules(13))
					{
						Token elSe;
						string w = "else";
						vector<char> x;
						for (int i = 0; i < w.size(); i++)
						{
							x.push_back(w[i]);
						}
						elSe.stateIndex = 2;
						elSe.tokenValue = x;
						if (match(elSe))
						{
							if (rules(13))
							{
								return true;
							}
							else
							{
								return false;
							}
						}
						else
						{
							Token end;
							string w = "end";
							vector<char> x;
							for (int i = 0; i < w.size(); i++)
							{
								x.push_back(w[i]);
							}
							end.stateIndex = 2;
							end.tokenValue = x;
							if (match(end))
							{
								return true;
							}
							else
							{
								return false;
							}
						}
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else if (ruleIndex == 12) // stmt ==> if_stmt | repeat_stmt | assign_stmt | read_stmt | write_stmt
	{
		if (rules(11))
		{
			return true;
		}
		else if (rules(10))
		{
			return true;
		}
		else if (rules(9))
		{
			return true;
		}
		else if (rules(8))
		{
			return true;
		}
		else if (rules(7))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (ruleIndex == 13) // stmt_seq ==> stmt {; stmt}
	{
		if (rules(12))
		{
			while (matchSem())
			{
				if (parser::index >= scannerOutput::tokens.size())
				{
					return true;
				}
				if (rules(12)) // add here stopping condition for end code
				{
					
				}
				else
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (ruleIndex == 14) // program ==> stmt_seq
	{
		if (rules(13))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}

int main() {
	scannerOutput::tokens = scanner();
	/*
	for (int i = 0; i < scannerOutput::tokens.size(); i++)
	{
		for (int j = 0; j < scannerOutput::tokens.at(i).tokenValue.size(); j++)
		{
			cout << scannerOutput::tokens.at(i).tokenValue.at(j);
		}
		cout << "\t" << scannerOutput::tokens.at(i).stateIndex << '\t'<<scannerOutput::tokens.at(i).isReservedWord;
		cout << endl;
	}
	*/
	cout << rules(14) << endl;
	system("pause");
	return 0;
}