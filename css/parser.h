#include "../parser/parser.h"
#include "css.h"
#include <vector>

class CSSParser : public Parser {
public:
  CSSParser(std::string s) : Parser(s) {}
  Stylesheet parse();

private:
  SimpleSelector parse_simple_selector();
  std::string parse_identifier();
  bool valid_identifier_char(char c);
  Rule parse_rule();
  std::vector<Rule> parse_rules();
  std::vector<Selector> parse_selectors();
  std::vector<Declaration> parse_declarations();
  Declaration parse_declaration();
  Value parse_value();
  Value parse_length();
  float parse_float();
  Unit parse_unit();
  Value parse_color();
  uint8_t parse_hex_pair();
};
