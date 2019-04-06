#include "ccCore/ccFileInfoSplitter.h"

namespace Luna {

ccFileInfoSplitter::ccFileInfoSplitter(const std::string& path) { split(path); }

void ccFileInfoSplitter::split(const std::string& path)
{
  path_ = path;

  split_path(path.c_str(), drive_, dir_, name_, ext_);

  file_name_ = name_;
  file_name_ += ".";
  file_name_ += ext_;
}

bool ccFileInfoSplitter::split_path(
    const std::string& path,
    std::string& drive,
    std::string& dir, std::string& file_name,
    std::string& file_ext)
{

  if (path == "")
    return false;

  char sep = '/';

#ifdef _WIN32
  sep = '\\';
#endif

  size_t pos = path.rfind(sep, path.length());

  if (pos != std::string::npos) {
    dir = path.substr(0, pos);
    std::string full_file_name = path.substr(pos + 1, path.length() - pos);

    pos = full_file_name.rfind(".", full_file_name.length());

    if (pos != std::string::npos) {
      file_name = full_file_name.substr(0, pos);
      file_ext = full_file_name.substr(pos + 1, full_file_name.length() - pos);
    }
  }

  return true;
}

} // namespace Luna