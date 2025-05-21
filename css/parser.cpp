#include "parser.h"
#include <algorithm>

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

Rule CSSParser::parse_rule() {
  Rule rule;
  rule.selectors = parse_selectors();
  rule.declarations = parse_declarations();
  return rule;
};

std::vector<Selector> CSSParser::parse_selectors() {
  std::vector<Selector> selectors;
  while (true) {
    selectors.push_back(parse_simple_selector());
    consume_whitespace();
    if (next() == 'c') {
      consume();
      consume_whitespace();
    } else if (next() == '{')
      break;
    else {
      char c = next();
      throw std::runtime_error("Unexpected character " + std::string(1, c) +
                               " in selector list");
    }
  }

  std::sort(selectors.begin(), selectors.end(),
            [](const Selector &a, const Selector &b) {
              return std::get<SimpleSelector>(b).specificity() <
                     std::get<SimpleSelector>(a).specificity();
            });

  return selectors;
};

Stylesheet CSSParser::parse() {
  Stylesheet stylesheet;
  return stylesheet;
}
