#include <iostream>
#include <memory>
#include <utility>

struct A {
  A(int&& n) { std::cout << "rvalue overload, n = " << n << std::endl; }
  A(int& n) { std::cout << "lvalue overload, n = " << n << std::endl; }
};


class B {
public:
  template<class T1, class T2, class T3>
  B(T1&& t1, T2&& t2, T3&& t3):
    a1(std::forward<T1>(t1)),
    a2(std::forward<T2>(t2)),
    a3(std::forward<T3>(t3)) {}
private:
  A a1, a2, a3;
  
};


template<class T, class U>
std::unique_ptr<T> make_unique1(U&& u){
  return std::unique_ptr<T>(new T(std::forward<U>(u)));
}

template<class T, class... U>
std::unique_ptr<T> make_unique2(U&&... u){
  return std::unique_ptr<T>(new T(std::forward<U>(u)...));
}



int main() {

  auto p1 = make_unique1<A>(2);	// rvalue
  int i = 1;
  auto p2 = make_unique1<A>(i);	// lvalue

  auto p3 = make_unique2<B>(2, i, 1); 
}
