#include "parser.h"

void Parser::Print() {
  std::cout << "Parser state:" << std::endl;
  std::cout << "  Position: " << pos << std::endl;
  std::cout << "  Line: " << line << std::endl;
  std::cout << "  Column: " << col << std::endl;
  std::cout << "  Current source: " << (pos < src.length() ? src.substr(pos) : "EOF") << std::endl;
}

char Parser::next() {
  if (eof()) return '\0';
  return src[pos];  
}

bool Parser::starts_with(std::string s) {
  if (pos + s.length() > src.length()) {
    return false;
  }
  
  for (size_t i = 0; i < s.length(); i++) {
    if (src[pos + i] != s[i]) {
      return false;
    }
  }
  
  return true;
}

char Parser::consume() {
  if (eof()) {
    return '\0';
  }
  
  char c = src[pos];
  pos++;
  
  if (c == '\n') {
    line++;
    col = 1;
  } else {
    col++;
  }
  
  return c;
}

bool Parser::eof() {
  return pos >= src.length();
}