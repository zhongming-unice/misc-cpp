#include <iostream>
#include <memory>
#include <experimental/propagate_const>
 
// 接口（widget.h）
class widget {
  class impl;
  std::experimental::propagate_const<std::unique_ptr<impl>> pImpl;
public:
  void draw() const; // 公开 API，将被转发给实现
  void draw();
  bool shown() const { return true; } // 公开 API，实现必须调用之
  widget(int);
  ~widget(); // 定义于实现文件，其中 impl 是完整类型
  widget(widget&&); // 定义于实现文件 
  // 注意：在被移动的对象上调用 draw() 是 UB
  widget(const widget&) = delete;
  widget& operator=(widget&&); // 定义于实现文件
  widget& operator=(const widget&) = delete;
};
 
// 实现（widget.cpp）
class widget::impl {
  int n; // 私有数据
public:
  void draw(const widget& w) const {
    if(w.shown()) // 对公开成员函数的此调用要求回溯引用
      std::cout << "drawing a const widget " << n << '\n';
  }
  void draw(const widget& w) {
    if(w.shown())
      std::cout << "drawing a non-const widget " << n << '\n';
  }
  impl(int n) : n(n) {}
};
void widget::draw() const { pImpl->draw(*this); }
void widget::draw() { pImpl->draw(*this); }
widget::widget(int n) : pImpl{std::make_unique<impl>(n)} {}
widget::widget(widget&&) = default;
widget::~widget() = default;
widget& widget::operator=(widget&&) = default;
 
// 用户（main.cpp）
int main()
{
  widget w(7);
  const widget w2(8);
  w.draw();
  w2.draw();
}
