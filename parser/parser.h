#pragma once
#include <cstddef>
#include <iostream>
#include <string>
#include <functional>

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

protected:
  char next();
  bool starts_with(std::string s);
  void expect(std::string s);
  char consume();
  std::string consume_while(const std::function<bool(char)>& func);
  void consume_whitespace();
  bool eof();
};
