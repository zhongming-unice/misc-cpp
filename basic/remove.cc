#include <algorithm>
#include <string>
#include <iostream>
#include <cctype>

// STL中remove（）只是将待删除元素之后的元素移动到容器的前端，
// 而不是删除。若要真正移除，需要搭配使用erase()
int main()
{
    std::string str1 = "Text with some   spaces";
    str1.erase(std::remove(str1.begin(), str1.end(), ' '),
               str1.end());
    std::cout << str1 << '\n';
 
    std::string str2 = "Text\n with\tsome \t  whitespaces\n\n";
    str2.erase(std::remove_if(str2.begin(), 
                              str2.end(),
                              [](unsigned char x){return std::isspace(x);}),
               str2.end());
    std::cout << str2 << '\n';
}
