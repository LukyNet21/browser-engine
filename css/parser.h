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
  std::vector<Selector> parse_selectors();
  std::vector<Declaration> parse_declarations();
};
