#ifndef __UTILS_H_
#define __UTILS_H_
#include <memory>

bool ReadFile(const char *path, std::unique_ptr< char[] > &content, int &len);

#endif  // __UTILS_H_
