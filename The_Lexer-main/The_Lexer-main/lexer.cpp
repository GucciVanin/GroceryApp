#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <regex>
#include <iomanip>
#include <fstream>
using namespace std;

string readtext(const string& filename) 
{
    ifstream file(filename);
    string textFile;

    if (file.is_open()) 
	{
        // Read the entire file into the string
        file.seekg(0, ios::end);
        textFile.reserve(file.tellg());
        file.seekg(0, ios::beg);

        textFile.assign((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());

        file.close();
    } 
	
	else 
	{
        cerr << "Unable to open file: " << filename << endl;
    }

    return textFile;
}


int main()
{
	ofstream outputFile("output.txt");

  streambuf *original = std::cout.rdbuf();
  cout.rdbuf(outputFile.rdbuf());


	cout<< "Token      |Lexeme " << endl;
	cout<< "-----------|------" << endl;

	// not reading whole word i++ only i or i and + probably something worng with the pattern
	string filename = "input_scode.txt";
    string fileContents = readtext(filename);
	string keywords[] = { "cin", "cout", "while", "if", "else", "for", "int", "float", "endl", "double" };
	string regexId = "\\b(?!(cin|cout|while|for|if|else|int|float|endl|double)\\b)[a-zA-Z_]\\w*\\b"; 
	string symbol = "[{}();,]";
	string ope = "\\++ |\\-- |\\<< |\\>> |[\\+\\-*/=<>!%] |== |>= |<= |!= "; // adding space before == like | == |,
	// it adds a space before == in output
	// the pattern in ope should match the one in input in i++ ) where there is a space after i++
	// so we add \\++ | here we add space after ++
	// regex reads from left to right so if the small pattern match the larg one after | is ignored
	string regexDig = "\\d+";
	regex regexDigit(regexDig);
	regex regexIdentifier(regexId);
	regex regexOper(ope);
	regex regexSy(symbol);
	smatch match;
	string::const_iterator  searchStart(fileContents.cbegin());



	while (regex_search(searchStart, fileContents.cend(), match, regexDigit)) // != eol()
	{
		//string matchDig = match[0];
		//double number = stod(matchDig);
		cout << "constant   |" << match[0]; // matchDig
		// match[0] points to the entire matched substring
		searchStart = match.suffix().first; // i++ and its like \n or newline
		cout << endl;
	}


	searchStart = fileContents.cbegin(); // reset starting point to th beginning of the fileContents string


	while (regex_search(searchStart, fileContents.cend(), match, regexIdentifier))
	{



		cout << "Identifier |" << match[0];
		searchStart = match.suffix().first;
		cout << endl;

	}

	searchStart = fileContents.cbegin();

	while (regex_search(searchStart, fileContents.cend(), match, regexOper))
	{
		if (match[0] != ' ')
		{
			//cout << "space  |"<< match[0]; compiler reads white space 
			//searchStart = match.suffix().first;
			cout << "Operator   |" << match[0];
			//searchStart = match.suffix().first;
			cout << endl;

		}

		searchStart = match.suffix().first; // like i++ keeps
		

		//else
		//{
			//cout << "Operator   |" << match[0];
			//searchStart = match.suffix().first;
		//}
		//cout << endl;
	}


	for (string keyword : keywords)
	{
		if (fileContents.find(keyword) != string::npos)
		{
			cout << "keyword    |" << keyword << endl;
		}
	}

	searchStart = fileContents.cbegin();

	while (regex_search(searchStart, fileContents.cend(), match, regexSy))
	{

		cout << "Symbol     |" << match[0];

		searchStart = match.suffix().first;
		cout << endl;
	}

	std::cout.rdbuf(original);
	outputFile.close();
	return 0;
}
