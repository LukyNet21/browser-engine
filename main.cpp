#include <iostream>
#include "parser/parser.h"

using namespace std;

int main() {
  Parser parser("<html><body><h1>Hello World</h1></body></html>");
  parser.Print();
  
  return 0;
}
