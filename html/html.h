
#include <iostream>
#include <vector>
#include <unordered_map>
#include <variant>

enum HTMLNodeType
{
  TYPE_ELEMENT,
  TYPE_TEXT
};

using HTMLAttributes = std::unordered_map<std::string, std::string>;

struct HTMLElementData
{
  std::string tagName;
  std::vector<HTMLNode> children = {};
  HTMLAttributes attributes = {};
};

using HTMLNodeData = std::variant<HTMLElementData, std::string>;

struct HTMLNode
{
  HTMLNodeType type;
  HTMLNodeData data;

  HTMLNode createText(const std::string& text);
  HTMLNode createElement(
    const std::string& tag,
    const HTMLAttributes& attrs = {},
    const std::vector<HTMLNode>& children = {}
  );
};

HTMLNode HTMLNode::createText(const std::string& text)
{
  HTMLNode node;
  node.type = TYPE_TEXT;
  node.data = text; 
  return node;
}

HTMLNode HTMLNode::createElement(
  const std::string& tag,
  const HTMLAttributes& attrs = {},
  const std::vector<HTMLNode>& children = {})
{
  HTMLNode node;
  node.type = TYPE_ELEMENT;
  node.data = HTMLElementData{tag, children, attrs};
  return node;
}