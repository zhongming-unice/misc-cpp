#include <iostream>
#include <csetjmp>
 
std::jmp_buf jump_buffer;
 
[[noreturn]] void a(int count) 
{
    std::cout << "a(" << count << ") called\n";
    std::longjmp(jump_buffer, count+1);  // setjump() 将返回 count+1
}
 
int main()
{
    volatile int count = 0; // 必须为 setjmp 设定局部变量为 volatile
    if (setjmp(jump_buffer) != 9) {
        a(count++);  // This will cause setjmp() to exit
    }
}
