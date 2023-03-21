#ifndef __SPLIT_STRING_H__
#define __SPLIT_STRING_H__

#include <string>
#include <vector>

// split string by separator
inline void SplitString(std::vector<std::string> &result, const std::string &input,
                 const std::string &sep)
{
  size_t cur = 0;
  size_t next = -1;
  do {
    cur = next + sep.size();
    next = input.find(sep, cur);
    result.push_back(input.substr(cur, next - cur));
  } while (next != std::string::npos);
}

#endif
