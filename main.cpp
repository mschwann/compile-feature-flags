#include "impl.hpp"
#include <iostream>

int main() {
  std::cout << isCompileFlag("black"sv) << std::endl;
  std::cout << CompileFlagsTableView.data() << std::endl;
  return 0;
}