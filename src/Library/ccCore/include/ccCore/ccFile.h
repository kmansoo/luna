/*
 * ccFile.h
 *
 *  Created on: 2019.02.16
 *      Author: Mansoo Kim(mansoo.kim@stradvision.com)
 */

#pragma once

#include "stdio.h"

#include <string>

namespace Luna {

class ccFile {
public:
  ccFile(){};
  virtual ~ccFile(){};

public:
  enum FilePosition {
    kEOF = -1,
    kBegin = 0,
    kCurrent = 1,
    kEnd = 2,
  };

  virtual bool open()
  {
    return false;
  }

  virtual bool open(const std::string& file_name, const std::string& option)
  {
    return false;
  }

  virtual bool close() = 0;
  virtual bool is_open() { return false; }

  virtual void set_memory(const char* buffer, size_t size){};

  virtual size_t read(void* buffer, size_t size) = 0;
  virtual size_t read_at(long offset, void* buffer, size_t size) = 0;
  virtual size_t write(const void* buffer, size_t size) = 0;

  virtual bool seek(long offset, int origin) = 0;
  virtual long tell() = 0;

  virtual bool move_prev(size_t size = 1) = 0;
  virtual bool move_next(size_t size = 1) = 0;

  virtual long length() = 0;
  virtual bool flush() = 0;
  virtual bool eof() = 0;
  virtual long error() = 0;

  virtual long get_cur_pos_char() = 0;
  virtual uint8_t get_char() = 0;
  virtual char* gets(char* string, int n) = 0;
  virtual int puts(const char* string) = 0;
  virtual bool put_char(uint8_t ch) = 0;

  virtual long scanf(const char* format, void* output) = 0;

  virtual size_t format(const char* format, ...);

public:
  virtual const std::string& get_string(std::string& buffer);
};
}
