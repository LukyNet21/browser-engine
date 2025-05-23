#include "parser.h"
#include <algorithm>

SimpleSelector CSSParser::parse_simple_selector() {
  SimpleSelector selector;

  bool continue_parsing = true;
  while (!eof() && continue_parsing) {
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
      } else {
        continue_parsing = false;
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

std::vector<Rule> CSSParser::parse_rules() {
  std::vector<Rule> rules;
  while (true) {
    consume_whitespace();
    if (eof()) break;
    rules.push_back(parse_rule());
  }

  return rules;
}

std::vector<Selector> CSSParser::parse_selectors() {
  std::vector<Selector> selectors;
  while (true) {
    selectors.push_back(parse_simple_selector());
    consume_whitespace();
    if (next() == ',') {
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
  stylesheet.rules = parse_rules();
  return stylesheet;
};

std::vector<Declaration> CSSParser::parse_declarations(){
  expect("{");
  std::vector<Declaration> declarations;
  while (true) {
    consume_whitespace();
    if (next() == '}') {
      consume();
      break;
    }
    declarations.push_back(parse_declaration());
  }
  return declarations;
};

Declaration CSSParser::parse_declaration() {
  std::string name = parse_identifier();
  consume_whitespace();
  expect(":");
  consume_whitespace();
  Value value = parse_value();
  consume_whitespace();
  expect(";");

  return {name, value};
}

Value CSSParser::parse_value() {
  char c = next();
  if (c >= '0' && c <= '9') {
    return parse_length();
  } else if (c == '#') {
    return parse_color();
  } else {
    Keyword keyword;
    keyword.name = parse_identifier();
    return keyword;
  }
}

Value CSSParser::parse_length() {
  float value = parse_float();
  Unit unit = parse_unit();
  Length length = {value, unit};
  return length;
}

float CSSParser::parse_float() {
  std::string float_str = consume_while([](char c) { 
    return (c >= '0' && c <= '9') || c == '.';
  });
  return std::stof(float_str);
}

Unit CSSParser::parse_unit() {
  std::string unit_str = parse_identifier();
  std::transform(unit_str.begin(), unit_str.end(), unit_str.begin(), 
                 [](unsigned char c){ return std::tolower(c); });
  
  if (unit_str == "px") {
    return Unit::Px;
  } else {
    throw std::runtime_error("Unrecognized unit: " + unit_str);
  }
}

Value CSSParser::parse_color() {
  expect("#");
  
  Color color;
  color.r = parse_hex_pair();
  color.g = parse_hex_pair();
  color.b = parse_hex_pair();
  color.a = 255;
  
  ColorValue colorValue = {color};
  return colorValue;
}

uint8_t CSSParser::parse_hex_pair() {
  if (pos + 2 > src.length()) {
    throw std::runtime_error("Unexpected end of input in color hex value");
  }
  
  std::string hex_pair = src.substr(pos, 2);
  pos += 2;
  
  try {
    return static_cast<uint8_t>(std::stoul(hex_pair, nullptr, 16));
  } catch (const std::exception&) {
    throw std::runtime_error("Invalid hex color value: " + hex_pair);
  }
}
