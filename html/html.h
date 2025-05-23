#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <variant>

struct HTMLNode;

enum HTMLNodeType
{
  TYPE_ELEMENT,
  TYPE_TEXT
};

using HTMLAttributes = std::unordered_map<std::string, std::string>;
using HTMLNodes = std::vector<HTMLNode>;

struct HTMLElementData
{
  std::string tagName;
  HTMLNodes children = {};
  HTMLAttributes attributes = {};
};

using HTMLNodeData = std::variant<HTMLElementData, std::string>;


struct HTMLNode
{
  HTMLNodeType type;
  HTMLNodeData data;

  void print(int indent = 0) const;

  static HTMLNode createText(const std::string& text);
  static HTMLNode createElement(
    const std::string& tag,
    const HTMLAttributes& attrs = {},
    const HTMLNodes& children = {}
  );
};
