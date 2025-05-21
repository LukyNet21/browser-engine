#include "parser.h"

SimpleSelector CSSParser::parse_simple_selector() {
  SimpleSelector selector;
  
  while (!eof()) {
    switch (next()) {
      case '#':
        consume();
        selector.id = parse_identifier();
        break; 
      case '.':
        consume();
        selector.class_names.push_back(parse_identifier());
        break;
      case '*':
        consume();
        break;
      default:
        if (valid_identifier_char(next())) {
          selector.tag_name = parse_identifier();
        }
        break;
    }
  }
  
  return selector;
}

std::string CSSParser::parse_identifier() {
  return consume_while([this](char c) { return valid_identifier_char(c); });
}

bool CSSParser::valid_identifier_char(char c) {
  return std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_';
}

Stylesheet CSSParser::parse() {
  Stylesheet stylesheet;
  return stylesheet;
}