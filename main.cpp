#include <iostream>
#include "html/parser.h"

using namespace std;

int main() {
  HTMLParser parser("<html><body><h1 class=\"test\">Hello World</h1><span id=\"highlighted\">!!!!!</span> I am well!</body></html>");
  HTMLNode node = parser.parse();
  
  cout << "HTML Tree Structure:" << endl;
  node.print();
  
  return 0;
}
