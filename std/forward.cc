#include <iostream>
#include <string>
#include <utility>

class A {

  std::string var1, var2;
public:
  void set(const std::string& v1, const std::string& v2) {
    var1 = v1;
    var2 = v2;
    std::cout << "use copy" << std::endl;
  }

  void set(std::string&& v1, std::string&& v2) {
    var1 = std::move(v1);
    var2 = std::move(v2);
    std::cout << "use move" << std::endl;
  }
  void print(){
    std::cout << var1 << "   " << var2 << std::endl;
  }

  template<typename T1, typename T2>
  void set(T1&& v1, T2&& v2) {
    var1 = std::forward<T1>(v1);
    var2 = std::forward<T2>(v2);
    std::cout << "use forward" << std::endl;
  }
};

int main() {

  A a1, a2;
  
  std::string b = "string1";
  std::string c = "string2";
  a1.set(b, c);
  a1.print();
  
  a2.set("string1", "string2");
  a2.print();
}
