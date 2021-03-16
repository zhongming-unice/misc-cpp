#include <iostream>
#include <ranges>

int main(int argc, char *argv[])
{
  for (int even : std::ranges::iota_view(1) |
         std::views::filter([](int x){ return x % 2 == 0; }) |
         std::views::take(10) |
         std::views::transform([](int x){ return x * x; } ))
    std::cout << even << std::endl;
  return 0;
}
