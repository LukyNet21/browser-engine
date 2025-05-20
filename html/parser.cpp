#include "parser.h"
#include <string>

std::string HTMLParser::parse_name() {
  return consume_while(
      [](char c) { return std::isalnum(static_cast<unsigned char>(c)); });
}

HTMLNode HTMLParser::parse_node() {
  if (starts_with("<")) {
    return parse_element();
  }
  return parse_text();
}

HTMLNode HTMLParser::parse_text() {
  return HTMLNode::createText(consume_while([](char c) { return c != '<'; }));
}

HTMLNode HTMLParser::parse_element() {
  expect("<");
  std::string tag_name = parse_name();
  HTMLAttributes attrs = parse_attributes();
  expect(">");

  HTMLNodes children = parse_nodes();

  expect("</");
  expect(tag_name);
  expect(">");

  return HTMLNode::createElement(tag_name, attrs, children);
}

std::pair<std::string, std::string> HTMLParser::parse_attribute() {
  std::string name = parse_name();
  expect("=");
  std::string value = parse_attribute_value();
  return {name, value};
}

std::string HTMLParser::parse_attribute_value() {
  char open_quote = consume();
  if (open_quote != '"' && open_quote != '\'') {
    throw std::runtime_error("Expected attribute value to start with a quote");
  }

  std::string value =
      consume_while([open_quote](char c) { return c != open_quote; });

  char close_quote = consume();
  if (open_quote != close_quote) {
    throw std::runtime_error(
        "Expected attribute value to end with a matching quote (expected '" +
        std::string(1, open_quote) + "', got '" + std::string(1, close_quote) +
        "')");
  }

  return value;
}

HTMLAttributes HTMLParser::parse_attributes() {
  HTMLAttributes attrs = {};

  while (true) {
    consume_whitespace();
    if (next() == '>')
      break;

    std::pair<std::string, std::string> attr = parse_attribute();
    attrs[attr.first] = attr.second;
  }
  
  return attrs;
}

HTMLNodes HTMLParser::parse_nodes() {
  HTMLNodes nodes = {};
  while (true) {
    consume_whitespace();
    if (eof() || starts_with("</"))
      break;
    nodes.push_back(parse_node());
  }

  return nodes;
}

HTMLNode HTMLParser::parse() {
  HTMLNodes nodes = parse_nodes();
  if (nodes.size() == 1) {
    return nodes[0];
  }
 
  return HTMLNode::createElement("html", {}, nodes);
}
