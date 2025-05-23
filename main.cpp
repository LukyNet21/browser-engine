#include <iostream>
#include "html/parser.h"
#include "css/parser.h"

using namespace std;

int main() {
  HTMLParser parser("<html><body><h1 class=\"test\">Hello World</h1><span id=\"highlighted\">!!!!!</span> I am well!</body></html>");
  HTMLNode node = parser.parse();
  
  cout << "HTML Tree Structure:" << endl;
  node.print();

  CSSParser cssParser("h1 { color: #111111; } .test { margin: auto; padding: 20px; } h1.test {color:red;} #highlighted.blue{color:blue;  font-size: 20px;}");
  Stylesheet stylesheet = cssParser.parse();
  
  cout << "\nCSS Stylesheet:" << endl;
  stylesheet.print();
  
  return 0;
}
