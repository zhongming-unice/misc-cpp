#include <iostream>
#include <string_view>
 
class A
{
public:
  A() { std::cout << getName() << std::endl; } // note addition of constructor
 
  virtual std::string_view getName() const { return "A"; }
};
 
class B : public A
{
public:
  virtual std::string_view getName() const { return "B"; }
};
 
class C : public B
{
public:
  // Note: Functions in C is non-const.
  virtual std::string_view getName() { return "C"; }
};
 
class D : public C
{
public:
  virtual std::string_view getName() const { return "D"; }
};
 
int main()
{
  C c;
  A &rBase{ c };
  std::cout << rBase.getName() << '\n';
  
  return 0;
}
