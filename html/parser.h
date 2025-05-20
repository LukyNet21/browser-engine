#ifndef html_parser_h
#define html_parser_h

#include "../parser/parser.h"
#include "html.h"
#include <string>

class HTMLParser : public Parser {
public:
  HTMLParser(std::string s) : Parser(s) {}

  HTMLNode parse();

private:
  std::string parse_name();
  HTMLNode parse_node();
  HTMLNode parse_element();
  HTMLNode parse_text();
  HTMLNodes parse_nodes();
  HTMLAttributes parse_attributes();
  std::pair<std::string, std::string> parse_attribute();
  std::string parse_attribute_value();
};

#endif