#include <iostream>
#include <stack>
#include <string>
#include <iomanip>
#include <vector>

struct GrammarRule {
  char nonTerminal;
  std::vector<char> production;
};

class Parser {
private:
  std::vector<std::string> stackOutput;
  std::vector<std::string> inputOutput;
  std::vector<std::string> actionOutput;
  int counter = 0;

  std::stack<char> parserStack;
  std::vector<GrammarRule> grammarRule = {
    {'E', {'E', '+', 'T'}}, 
    {'E', {'T'}},           
    {'T', {'T', '*', 'F'}},
    {'T', {'F'}},           
    {'T', {'(', 'E', ')'}}, 
    {'F', {'i'}},
  };

  bool getNextState(const std::vector<char> &rule) {
    std::vector<char> stackTemp;
    int count = 1;

    for (int i = 0; i < parserStack.size(); ++i) {

      if (parserStack.top() == rule[rule.size() - count]) {
        stackTemp.push_back(parserStack.top());
        parserStack.pop();
        ++count;
      }

      if (count == rule.size()) {
        for (auto symbol : stackTemp) {
          parserStack.push(symbol);
        }
        return true;
      }
    }

    for (auto symbol : stackTemp) {
      parserStack.push(symbol);
    }

    return false;
  }

  void getStack(){
    std::vector<char> stackTemp;
    
    std::cout << "Current Stack: ";
    
    while(!parserStack.empty()) {
      stackTemp.push_back(parserStack.top());
      std::cout << parserStack.top() << " ";
      // stackOutput[counter] += std::to_string(parserStack.top());
      parserStack.pop();
    }
    std::cout << std::endl;
    while(!stackTemp.empty()) {
      parserStack.push(stackTemp.back());
      stackTemp.pop_back();
    }
  }

public:
  // Parser(const std::unordered_map<std::string, std::string>& parsingTable) :
  // parsingTable(parsingTable) {}
  bool isNonTerminal(char x) {
    if (x >= 'A' && x <= 'Z')
      return false;
    else
      return true;
  }

  void reduce(const GrammarRule &rule) { // Could also possibly take in the stack as an input
    // Reduce the top of the stack by the production rule
    std::cout << "Reducing: " << rule.nonTerminal << " -> ";
    // actionOutput.push_back("Reduction " + std::to_string(rule.nonTerminal) + " -> ");
    for (char symbol : rule.production) {
      std::cout << symbol << " ";
      // actionOutput[counter] += std::to_string(symbol) + " ";
      parserStack.pop();
    }
    // ++counter;
    // Push the new top of the stack onto the stack
    std::cout << std::endl;
    parserStack.push(rule.nonTerminal);
    // std::cout << "Current Top of Stack: " << parserStack.top() << std::endl;
    getStack();
    getReductionRule();
  }

  bool isReductionApplicable() {
    // Placeholder logic for checking if a reduction is applicable
    return parserStack.top() != '$';
  }

  void doShift(const std::string& input){
    std::string temp = input;
  
    for (char inputSymbol : input) {
      std::cout << "Shifting: " << inputSymbol << std::endl;
      // actionOutput.push_back("Shift");
      // ++counter;
      parserStack.push(inputSymbol);
      // inputOutput.push_back(input);

      while (isReductionApplicable()) {
        std::cout << "Looping for a reduction: ";
        getReductionRule();
        std::cout << std::endl;
        break;
      }
      // temp.erase(temp.begin());
      if (inputSymbol == '$') {
        std::cout << "End State encountered!" << std::endl;
        break;
      }
    }
  }

  void getReductionRule() {
    // Placeholder logic for finding the appropriate grammar rule for reduction
    for (const GrammarRule &rule : grammarRule) {
      for (char symbol : rule.production) {
        if (parserStack.top() != rule.production.back())
          break;
        else if (parserStack.top() == rule.production.back() && rule.production.size() == 1){
          reduce(rule);
        }
        else if (getNextState(rule.production))
          reduce(rule);
      }
    }
  }

  void parse(const std::string &input) {

    // parserStack.push('$');
    doShift(input);
    std::cout << "Fully Parsed: " << input << std::endl;
    // parserStack.pop();
    if (parserStack.top() == '$')
      std::cout << "Successfully Parsed" << std::endl;
    else
      std::cout << "This string is not accepted";
  }

  void printOutput(){
    std::cout << std::setw(5) << "Step" << std::setw(10) << "Stack" << std::setw(15) << "Input" << std::setw(20) << "Action" << std::endl;
    std::cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    for (int i = 0; i < counter; ++i) {
      std::cout << std::setw(5) << i + 1 << std::setw(10) << stackOutput[i] << std::setw(15) << inputOutput[i] << std::setw(20) << actionOutput[i] << std::endl;
    }
    
  }
};

int main() {

  Parser parser;
  //parser.parse("(i+i)*i$");
  //parser.parse("i*i$");
  parser.parse("(id*)");
  return 0;
}