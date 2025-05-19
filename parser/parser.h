#include <cstddef>
#include <iostream>
#include <string>

class Parser {
public:
  std::string src;
  int pos;
  size_t line;
  size_t col;
  
  Parser(std::string s) {
    src = s;
    pos = 0;
    line = 1;
    col = 1;
  }

  void Print();

private:
  char next();
  bool starts_with(std::string s);
  char consume();
  bool eof();
};
