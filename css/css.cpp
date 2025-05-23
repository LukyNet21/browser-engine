#include "css.h"
#include <algorithm>
#include <iostream>

Specificity SimpleSelector::specificity() const {
  std::size_t a = id ? 1 : 0;         // ID count
  std::size_t b = class_names.size(); // class count
  std::size_t c = tag_name ? 1 : 0;   // tag count
  return {a, b, c};
}

bool matches(const HTMLNode &node, const Selector &selector) {
  if (std::holds_alternative<SimpleSelector>(selector)) {
    const SimpleSelector &simpleSelector = std::get<SimpleSelector>(selector);
    return matches_simple_selector(node, simpleSelector);
  }
  return false;
}

bool matches_simple_selector(const HTMLNode &node,
                             const SimpleSelector &selector) {
  if (node.type != HTMLNodeType::TYPE_ELEMENT)
    return false;

  const HTMLElementData &elem = std::get<HTMLElementData>(node.data);
  if (selector.tag_name && elem.tagName != *selector.tag_name)
    return false;

  if (selector.id)
    if (elem.attributes.count("id") == 0 ||
        elem.attributes.at("id") != *selector.id)
      return false;

  for (const std::string &class_name : selector.class_names)
    if (elem.attributes.count("class") == 0 ||
        elem.attributes.at("class").find(class_name) == std::string::npos)
      return false;

  return true;
}

std::optional<MatchedRule> match_rule(const HTMLNode& node, const Rule& rule) {
  for (const Selector& selector : rule.selectors) {
    if (matches(node, selector)) {
      Specificity spec;
      if (std::holds_alternative<SimpleSelector>(selector)) {
        spec = std::get<SimpleSelector>(selector).specificity();
      } else {
        spec = {0, 0, 0};
      }
      return std::make_optional<MatchedRule>(std::make_pair(spec, &rule));
    }
  }
  return std::nullopt;
}

std::vector<MatchedRule> matching_rules(const HTMLNode& node, const Stylesheet& stylesheet) {
  std::vector<MatchedRule> matches;
  for (const Rule& rule : stylesheet.rules) {
    std::optional<MatchedRule> matched = match_rule(node, rule);
    if (matched) {
      matches.push_back(*matched);
    }
  }
  
  return matches;
}

PropertyMap specified_values(const HTMLNode& node, const Stylesheet& stylesheet) {
  PropertyMap values;
  if (node.type != HTMLNodeType::TYPE_ELEMENT) {
    return values;
  }
  
  std::vector<MatchedRule> rules = matching_rules(node, stylesheet);
    std::sort(rules.begin(), rules.end(), 
    [](const MatchedRule& a, const MatchedRule& b) {
      return a.first < b.first;
    });
  
  for (const auto& [specificity, rule] : rules) {
    for (const Declaration& declaration : rule->declarations) {
      values[declaration.name] = declaration.value;
    }
  }
  
  return values;
}

StyleNode style_tree(const HTMLNode& root, const Stylesheet& stylesheet) {
  StyleNode styled_node;
  styled_node.node = root;
  
  styled_node.specified_values = specified_values(root, stylesheet);
  
  if (root.type == HTMLNodeType::TYPE_ELEMENT) {
    const HTMLElementData& elem_data = std::get<HTMLElementData>(root.data);
    for (const HTMLNode& child : elem_data.children) {
      styled_node.children.push_back(style_tree(child, stylesheet));
    }
  }
  
  return styled_node;
}

void Stylesheet::print() const {
  for (size_t i = 0; i < rules.size(); i++) {
    const Rule& rule = rules[i];
    std::cout << "Rule #" << (i + 1) << ":" << std::endl;
    
    std::cout << "  Selectors: ";
    for (const Selector& selector : rule.selectors) {
      if (std::holds_alternative<SimpleSelector>(selector)) {
        const SimpleSelector& simple = std::get<SimpleSelector>(selector);
        if (simple.tag_name) {
          std::cout << *simple.tag_name;
        }
        if (simple.id) {
          std::cout << "#" << *simple.id;
        }
        for (const std::string& className : simple.class_names) {
          std::cout << "." << className;
        }
      }
      std::cout << " ";
    }
    std::cout << std::endl;
    
    std::cout << "  Declarations:" << std::endl;
    for (const Declaration& decl : rule.declarations) {
      std::cout << "    " << decl.name << ": ";
      if (std::holds_alternative<Keyword>(decl.value)) {
        std::cout << std::get<Keyword>(decl.value).name;
      }
      else if (std::holds_alternative<Length>(decl.value)) {
        const Length& length = std::get<Length>(decl.value);
        std::cout << length.value;
        if (length.unit == Unit::Px) {
          std::cout << "px";
        }
      }
      else if (std::holds_alternative<ColorValue>(decl.value)) {
        const Color& color = std::get<ColorValue>(decl.value).color;
        std::cout << "#";
        std::cout << std::hex << static_cast<int>(color.r);
        std::cout << std::hex << static_cast<int>(color.g);
        std::cout << std::hex << static_cast<int>(color.b);
        std::cout << std::dec;
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
}
