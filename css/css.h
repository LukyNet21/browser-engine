#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

enum Unit {
  Px,
};

struct Color {
  uint8_t r, g, b, a;
};

struct Keyword {
  std::string name;
};

struct Length {
  float value;
  Unit unit;
};

struct ColorValue {
  Color color;
};

using Value = std::variant<Keyword, Length, ColorValue>;

struct Declaration {
  std::string name;
  Value value;
};

struct SimpleSelector {
  std::optional<std::string> tag_name;
  std::optional<std::string> id;
  std::vector<std::string> class_names;
};

using Selector = std::variant<SimpleSelector>;

struct Rule {
  std::vector<Selector> selectors;
  std::vector<Declaration> declarations;
};

struct Stylesheet {
  std::vector<Rule> rules;
};
