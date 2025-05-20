#include "html.h"

HTMLNode HTMLNode::createText(const std::string& text)
{
  HTMLNode node;
  node.type = TYPE_TEXT;
  node.data = text; 
  return node;
}

HTMLNode HTMLNode::createElement(
  const std::string& tag,
  const HTMLAttributes& attrs,
  const HTMLNodes& children)
{
  HTMLNode node;
  node.type = TYPE_ELEMENT;
  node.data = HTMLElementData{tag, children, attrs};
  return node;
}

void HTMLNode::print(int indent) const 
{
  std::string indentation(indent, ' ');
  
  if (type == TYPE_TEXT) {
    std::cout << indentation << "TEXT: " << std::get<std::string>(data) << std::endl;
  } else if (type == TYPE_ELEMENT) {
    const HTMLElementData& elem = std::get<HTMLElementData>(data);
    
    std::cout << indentation << "ELEMENT: " << elem.tagName << std::endl;
    
    std::string id, className;
    for (const auto& [key, value] : elem.attributes) {
      if (key == "id") {
        std::cout << indentation << " - id: " << value<< std::endl;
      } else if (key == "class") {
        std::cout << indentation << " - class: " << value<< std::endl;
      }
    }

    for (const auto& child : elem.children) {
      child.print(indent + 2);
    }
  }
}
