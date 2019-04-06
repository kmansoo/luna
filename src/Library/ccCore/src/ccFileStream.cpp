#include <stdarg.h>
#include <stdlib.h>

#include "ccCore/ccFileStream.h"

namespace Luna {

ccFileStream::ccFileStream()
{
  size_ = 0;
  fp_ = NULL;
}

ccFileStream::~ccFileStream()
{
  if (fp_)
    fclose(fp_);
}

bool ccFileStream::open(const std::string& file_name, const std::string& option)
{
  if (fp_)
    return false;

  if (option.length() == 0)
    fp_ = fopen(file_name.c_str(), "rb+");
  else
    fp_ = fopen(file_name.c_str(), option.c_str());

  if (fp_ == NULL)
    return false;

  size_ = length();

  return true;
}

bool ccFileStream::is_open()
{
  if (fp_ == NULL)
    return false;

  return true;
}

bool ccFileStream::close()
{
  if (fp_) {
    fclose(fp_);

    fp_ = NULL;
  }

  return true;
}

size_t ccFileStream::read(void* buffer, size_t size)
{
  if (fp_ == NULL)
    return 0;

  size_t read_size = fread(buffer, 1, size, fp_);

  return read_size;
}

size_t ccFileStream::read_at(long offset, void* buffer, size_t size)
{
  if (fp_ == NULL)
    return 0;

  if (seek(offset, SEEK_SET) == false)
    return 0;

  size_t read_size = fread(buffer, 1, size, fp_);

  return read_size;
}

size_t ccFileStream::write(const void* buffer, size_t size)
{
  if (fp_ == NULL)
    return 0;

  if (fwrite(buffer, size, 1, fp_) == 1)
    return size;

  size_ = this->length();

  return 0;
}

bool ccFileStream::seek(long offset, int origin)
{
  if (fp_ == NULL)
    return false;

  if (fseek(fp_, offset, origin) != 0)
    return false;

  return true;
}

long ccFileStream::tell()
{
  if (fp_ == NULL)
    return 0;

  return ftell(fp_);
}

bool ccFileStream::move_prev(size_t size)
{
  if (fp_ == NULL)
    return false;

  long cur_pos = ftell(fp_);

  if (cur_pos - (long)size < 0)
    return false;

  fseek(fp_, cur_pos - size, SEEK_SET);

  return true;
}

bool ccFileStream::move_next(size_t size)
{
  if (fp_ == NULL)
    return false;

  long cur_pos = ftell(fp_);

  if (cur_pos - (long)size >= size_)
    return false;

  fseek(fp_, cur_pos + size, SEEK_SET);

  return true;
}

long ccFileStream::length()
{
  if (fp_ == NULL)
    return 0;

  long cur_pos = ftell(fp_);

  fseek(fp_, 0, SEEK_END);

  size_ = ftell(fp_);

  fseek(fp_, cur_pos, SEEK_SET);

  return size_;
}

bool ccFileStream::flush()
{
  if (fp_ == NULL)
    return false;

  fflush(fp_);

  return true;
}

bool ccFileStream::eof()
{
  if (fp_ == NULL)
    return false;

  if (feof(fp_))
    return true;

  if (tell() == size_)
    return true;

  return false;
}

long ccFileStream::error() { return 0; }

long ccFileStream::get_cur_pos_char()
{
  if (fp_ == NULL)
    return ccFile::kEOF;

  long cur_pos = ftell(fp_);

  long in_char = fgetc(fp_);

  fseek(fp_, cur_pos, SEEK_SET);

  return in_char;
}

uint8_t ccFileStream::get_char()
{
  if (fp_ == NULL)
    return ccFile::kEOF;

  return fgetc(fp_);
}

char* ccFileStream::gets(char* string, int n)
{
  if (fp_ == NULL)
    return NULL;

  return fgets(string, n, fp_);
}

int ccFileStream::puts(const char* string)
{
  if (fp_ == NULL)
    return 0;

  return fputs(string, fp_);
}

bool ccFileStream::put_char(uint8_t ch)
{
  if (fp_ == NULL)
    return ccFile::kEOF;

  fputc(ch, fp_);

  return true;
}

long ccFileStream::scanf(const char* format, void* output) { return 0; }

bool ccFileStream::is_exists(const char* file_name)
{
  FILE* fp = fopen(file_name, "r");

  if (fp == NULL)
    return false;

  fclose(fp);

  return true;
}

} // namespace Luna