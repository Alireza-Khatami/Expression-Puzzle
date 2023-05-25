#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
#include <cmath>

// Helper function to check if a character is an operator
bool isOperator(char c) {
	return (c == '+' || c == '*');
}
bool isOperator(const char* c) {
	return (*c == '+' ||  *c == '*');
}

// Helper function to perform an operation
int performOperation(char op, int op1, int op2) {
	if (op == '+') {
		return op1 + op2;
	}
	else if (op == '*') {
		return op1 * op2;
	}
	else {
		return 0;
	}


//This result file is for determening the value of a string expression. the  isbigger attribute is for when
//a operand is larger than N. This way we no longer have to add any more alphabets to this string anymore
struct result {
	bool isbigger;
	int val;
	result (bool isbigger , int val): isbigger(isbigger),val(val){}
};

//gets a line and holds N , alphabet, and the length of the alphabet not containing + or *
struct expresssionComponents {
public:
	int N;
	int len;
	vector<string> alphabets;
	expresssionComponents(const std::string& line) {
		char delimiter = ' ';
		std::stringstream ss(line);
		string temp;
		std::getline(ss, temp, delimiter);
		this->len = stoi(temp);
		for (int i = 0; i < this->len; i++) {
			std::getline(ss, temp, delimiter);
			alphabets.push_back(temp);
		}
		alphabets.push_back("+");
		alphabets.push_back("*");
		std::getline(ss, temp, delimiter);
		this->N = stoi(temp);
	}
};

// Function to evaluate a string expression
result	evaluateExpression(string expression, int N) {
	bool isbigger;
	stack<int> values;
	stack<char> operators;
	for (int i = 0; i < expression.length(); i++) {
		// If the current character is a whitespace, skip it
		if (expression[i] == ' ') {
			continue;
		}
		// If the current character is a digit, convert it to an integer and push it onto the values stack
		else if (isdigit(expression[i])) {
			int val = 0;
			while (i < expression.length() && isdigit(expression[i])) {
				val = (val * 10) + (expression[i] - '0');
				i++;
			}
			if (val > N)
				return {true,val };
			i--;
			values.push(val);
		}
		// If the current character is an operator, push it onto the operators stack
		else if (isOperator(expression[i])) {
			while (!operators.empty() && (expression[i] != '+') && (operators.top() == '*')) {
				int op2 = values.top();
				values.pop();
				int op1 = values.top();
				values.pop();
				char op = operators.top();
				operators.pop();
				int val = performOperation(op, op1, op2);
				values.push(val);
			}
			operators.push(expression[i]);
		}
	}
	// Evaluate the remaining operators in the operators stack
	while (!operators.empty()) {
		if (values.size() == 1 && operators.size() == 1)
			return { false,0 };
		if (operators.size() > values.size())	
			return {true,0};
		int op2 = values.top();
		values.pop();
		int op1 = values.top();
		values.pop();
		char op = operators.top();
		operators.pop();
		int val = performOperation(op, op1, op2);
		values.push(val);
	}

	// The final result is the only value left in the values stack
	if (values.top() > N)
		return { true, values.top() };
	return { false,values.top() };
}

//int main(int argc, char* argv[]) {
int main() {
	string filename("input.txt");
	ofstream myfile;
	myfile.open(string("output.txt"));
	std::ifstream input_file(filename);
	std::string line;
	int fileLen = 0;
	if (std::getline(input_file, line))
		 fileLen = std::stoi(line);
	else {
		std::cout << "couldn't read file";
		return -1;
	}
	for (int k = 0; k < fileLen; k++) {
		std::getline(input_file, line);
		expresssionComponents EC(line);
		vector<string> expressions({});
		for (int i = 0;i < EC.len; i++)
			expressions.push_back(EC.alphabets[i]);
		bool flag = true;// when founded a candidate we no longer need to search for the rest of the vector 
		string finalResult = "";
		int len;
		while (!expressions.empty() && flag) {
			len = expressions.size();
			for (int i = 0; i < len && !expressions.empty(); i++) {
				result  res = evaluateExpression(expressions[i], EC.N);
				std::cout << EC.N << " != " << expressions[i] << " == " << res.val<<endl;
				if (res.isbigger){// the usage of isbiger attribute in the result struct
					expressions.erase(expressions.begin());
					i--;
					continue;
				}
				if (res.val == EC.N) {
					finalResult = expressions[i];
					flag = false;
					break;
				}
				for (string alphabet : EC.alphabets) {
					if (isOperator(*(expressions[i].end()-1)) && isOperator(alphabet.c_str()))//if the end of the string is already an operator, another operator is not needed
						continue;
					expressions.push_back(expressions[i] + alphabet);
				}
				expressions.erase(expressions.begin());
				i--;
			}
		}
		if (flag)
			myfile << "N" << "\n";
		else {
			myfile << finalResult.size() << " ";
			myfile << finalResult<< "\n";
		}				
	}
	myfile.close();
	input_file.close();
	return 0;
} 