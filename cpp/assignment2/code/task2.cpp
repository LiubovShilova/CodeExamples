#include "simple_node.hpp"
#include <memory>
#include <iostream>

int main(int, char *[])
{
  // The following should compile
  SimpleNode root(13.3);
  SimpleNode new_tree(root);
  SimpleNode newer_tree(std::move(new_tree));
  const auto newest_tree = newer_tree;
  newer_tree.height();
  newer_tree.is_binary();
  newer_tree();

  std::vector<std::unique_ptr<SimpleNode>> ch_vec (0) ;
  ch_vec.emplace_back(std::make_unique<SimpleNode>(SimpleNode(9.4)));
  SimpleNode tree (std::move(ch_vec),14.2);
  return 0;
}
