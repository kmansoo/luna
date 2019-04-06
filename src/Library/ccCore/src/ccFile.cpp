#include <stdarg.h>
#include <stdlib.h>

#include "ccCore/ccFile.h"

namespace Luna {

bool ccFile::put_char(uint8_t ch) {
  size_t wrote_size = write(&ch, 1);
  return (bool)(wrote_size == 1);
}

const std::string& ccFile::get_string(std::string& buffer) {
  buffer.clear();
  
  while (this->eof() == false) {
    
    char in_char = get_char();

    if (in_char == '\n')
      break;

    buffer.push_back(in_char);
  }

  return buffer;
}

size_t ccFile::format(const char* format, ...) {
  va_list args;

  va_start(args, format);

  //  Guess we need no more than 128 bytes.
  int allocSize, size = 4096;
  char formated_data[4096] = {0};

#ifdef _WIN32
  allocSize = _vsnprintf(formated_data, size, format, args);
#else
  allocSize = vsnprintf(formated_data, size, format, args);
#endif

  va_end(args);

  return write(formated_data, allocSize);
}

}  // namespace Luna