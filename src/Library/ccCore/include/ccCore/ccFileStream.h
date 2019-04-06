/*
 * ccFileStream.h
 *
 *  Created on: 2019.02.16
 *      Author: Mansoo Kim(mansoo.kim@stradvision.com)
 */

#pragma once

#include "stdio.h"
#include "ccFile.h"

namespace Luna {

class ccFileStream : public ccFile {
 public:
  ccFileStream();
  virtual ~ccFileStream();

 public:
  virtual bool open(const std::string &file_name, const std::string &option);
  virtual bool close();
  virtual bool is_open();

  virtual size_t read(void *buffer, size_t size);
  virtual size_t read_at(long offset, void *buffer, size_t size);
  virtual size_t write(const void *buffer, size_t size);

  virtual bool seek(long offset, int origin);
  virtual long tell();

  virtual bool move_prev(size_t size = 1);
  virtual bool move_next(size_t size = 1);

  virtual long length();
  virtual bool flush();
  virtual bool eof();
  virtual long error();

  virtual long get_cur_pos_char();
  virtual uint8_t get_char();
  virtual char *gets(char *string, int n);

  virtual int puts(const char *string);
  virtual bool put_char(uint8_t ch);

  virtual long scanf(const char *format, void *output);

 public:
  static bool is_exists(const char *strFileName);

 protected:
  long size_;
  FILE *fp_;
};

}