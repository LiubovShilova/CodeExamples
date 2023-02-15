#ifndef SIMPLE_NODE_HPP
#define SIMPLE_NODE_HPP

#include <vector>
#include <memory>

class SimpleNode
{
public:

  class Invalid
  {
  };
  explicit SimpleNode(double content);
  SimpleNode(std::vector<std::unique_ptr<SimpleNode>> children, double content);

  SimpleNode(const SimpleNode &other);
  SimpleNode(SimpleNode &&other);

  SimpleNode &operator=(const SimpleNode &other);
  SimpleNode &operator=(SimpleNode &&other);

  std::size_t nchild() const;
  std::size_t height() const;
  bool is_binary() const;

  double &operator()();
  double operator()() const;

private:
  double content_;
  std::vector<std::unique_ptr<SimpleNode>> children_;
};

#endif // SIMPLE_NODE_HPP
