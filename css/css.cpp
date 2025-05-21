#include "css.h"

Specificity SimpleSelector::specificity() const {
  std::size_t a = id ? 1 : 0;         // ID count
  std::size_t b = class_names.size(); // class count
  std::size_t c = tag_name ? 1 : 0;   // tag count
  return {a, b, c};
}
