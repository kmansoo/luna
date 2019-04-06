/*
 * ccFile.h
 *
 *  Created on: 2019.02.16
 *      Author: Mansoo Kim(mansoo.kim@stradvision.com)
 */

#pragma once

#include "stdio.h"
#include "ccFile.h"

namespace Luna {

class ccFileMemory : public ccFile {
 public:
  ccFileMemory(uint8_t *buffer = NULL, size_t size = 0);
  virtual ~ccFileMemory();

 public:
  virtual bool open();
  virtual bool close();
  virtual bool is_open() {return true;};

  virtual void set_memory(const char *buffer, size_t size) {};

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
  virtual bool put_char(uint8_t ch);
  virtual int puts(const char *string) {return  0;};

  virtual long scanf(const char *format, void *output);

 public:
  static bool is_exists(const char *strFileName);

 protected:
  uint8_t *get_buffer(bool detach_buffer = true);
  bool alloc(size_t alloc_size);
  void free_buffer();

  uint8_t *buffer_;
  size_t size_;
  bool free_on_close_;
  long position_;  //  current position
  long edge_;      //  buffer size
};
}
