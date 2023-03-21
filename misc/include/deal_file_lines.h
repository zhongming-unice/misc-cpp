#ifndef __DEAL_FILE_LINES_H__
#define __DEAL_FILE_LINES_H__

#include <dirent.h>
#include <stdio.h>
#include <string>

typedef void (*func_deal_line)(std::string);

// read all files in a directory
inline void LoadFiles(std::string dir_path, func_deal_line func)
{
  DIR *dirp = opendir(dir_path.c_str());
  if (!dirp) return;

  struct dirent *entp;
  FILE *fstream = NULL;
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  while (entp = readdir(dirp)) {
    if (entp->d_type == DT_REG) {
      std::string file_path = dir_path + "/" + entp->d_name;
      fstream = fopen(file_path.c_str(), "r");
      if (!fstream) continue;
      while (nread = getline(&line, &len, fstream) != -1)
        func(std::string(line));
    }
  }
}

#endif
