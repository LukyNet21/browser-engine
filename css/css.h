#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include "../html/html.h"

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

using Specificity = std::tuple<size_t, size_t, size_t>;

struct SimpleSelector {
  std::optional<std::string> tag_name;
  std::optional<std::string> id;
  std::vector<std::string> class_names;

  Specificity specificity() const;
};

using Selector = std::variant<SimpleSelector>;

struct Rule {
  std::vector<Selector> selectors;
  std::vector<Declaration> declarations;
};

using PropertyMap = std::unordered_map<std::string, Value>;

struct StyleNode {
  HTMLNode node;
  PropertyMap specified_values;
  std::vector<StyleNode> children;
};

struct Stylesheet {
  std::vector<Rule> rules;
};

bool matches_simple_selector(const HTMLNode& node, const SimpleSelector& selector);
bool matches(const HTMLNode& node, const Selector& selector);

using MatchedRule = std::pair<Specificity, const Rule*>;

std::optional<MatchedRule> match_rule(const HTMLNode& node, const Rule& rule);
std::vector<MatchedRule> matching_rules(const HTMLNode& node, const Stylesheet& stylesheet);
PropertyMap specified_values(const HTMLNode& node, const Stylesheet& stylesheet);
StyleNode style_tree(const HTMLNode& root, const Stylesheet& stylesheet);
