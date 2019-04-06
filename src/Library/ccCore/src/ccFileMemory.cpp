#include <stdlib.h>
#include <string.h>

#include "ccCore/ccFileMemory.h"

namespace Luna {

ccFileMemory::ccFileMemory(uint8_t* buffer, size_t size)
{
  buffer_ = buffer;
  position_ = 0;
  size_ = edge_ = size;

  free_on_close_ = (bool)(buffer == 0);
}

ccFileMemory::~ccFileMemory() { close(); }

bool ccFileMemory::close()
{
  if ((buffer_) && (free_on_close_)) {
    ::free(buffer_);
    buffer_ = NULL;

    size_ = 0;
  }

  return true;
}

bool ccFileMemory::open()
{
  if (buffer_)
    return false; // Can't re-open without closing first

  position_ = size_ = edge_ = 0;

  buffer_ = (uint8_t*)malloc(1);
  free_on_close_ = true;

  return (buffer_ != 0);
}

uint8_t* ccFileMemory::get_buffer(bool detach_buffer)
{
  // can only detach, avoid inadvertantly attaching to
  // memory that may not be ours [Jason De Arte]
  if (detach_buffer)
    free_on_close_ = false;

  return buffer_;
}

size_t ccFileMemory::read(void* buffer, size_t size)
{
  if (buffer == NULL || buffer_ == NULL)
    return 0;

  if (position_ >= (long)size_)
    return 0;

  if (size == 0)
    return 0;

  long read_size;

  if (position_ + (long)size > (long)size_)
    read_size = (size_ - position_);
  else
    read_size = size;

  memcpy(buffer, buffer_ + position_, read_size);
  position_ += read_size;

  return read_size;
}

size_t ccFileMemory::read_at(long offset, void* buffer, size_t size)
{
  position_ = offset;

  return read(buffer, size);
}

size_t ccFileMemory::write(const void* buffer, size_t size)
{
  if (buffer == NULL || buffer_ == NULL)
    return 0;

  if (size == 0)
    return 0;

  if (position_ + (long)size > (long)edge_) {
    if (!alloc(position_ + size))
      return false;
  }

  memcpy(buffer_ + position_, buffer, size);

  position_ += size;

  if (position_ > (long)size_)
    size_ = position_;

  return size;
}

bool ccFileMemory::seek(long offset, int origin)
{
  if (buffer_ == NULL)
    return false;

  long new_pos = position_;

  switch (origin)
  {
  case ccFile::kBegin: new_pos = offset; break;
  case ccFile::kCurrent: new_pos += offset; break;
  case ccFile::kEnd: new_pos = size_ + offset; break;
  default:
    return false;
  }

  if (new_pos < 0)
    new_pos = 0;

  position_ = new_pos;

  return true;
}

long ccFileMemory::tell()
{
  if (buffer_ == NULL)
    return -1;

  return position_;
}

bool ccFileMemory::move_prev(size_t size)
{
  if (position_ - (long)size < 0)
    return false;

  position_ -= size;

  return true;
}

bool ccFileMemory::move_next(size_t size)
{
  if ((size_t)position_ + (long)size >= (size_t)size_)
    return false;

  position_ += size;

  return true;
}

long ccFileMemory::length()
{
  if (buffer_ == NULL)
    return -1;

  return size_;
}

bool ccFileMemory::flush()
{
  if (buffer_ == NULL)
    return false;

  return true;
}

bool ccFileMemory::eof()
{
  if (buffer_ == NULL)
    return true;

  return (position_ >= (long)size_);
}

long ccFileMemory::error()
{
  if (buffer_ == NULL)
    return -1;

  return (position_ > (long)size_);
}

bool ccFileMemory::put_char(uint8_t ch)
{
  if (buffer_ == NULL)
    return false;

  if (position_ >= edge_) {
    if (!alloc(position_ + 1))
      return false;
  }

  buffer_[position_++] = ch;

  if (position_ > (long)size_)
    size_ = position_;

  return true;
}

long ccFileMemory::get_cur_pos_char()
{
  if (eof())
    return ccFile::kEOF;

  return *(uint8_t*)((uint8_t*)buffer_ + position_);
}

uint8_t ccFileMemory::get_char()
{
  if (eof())
    return ccFile::kEOF;

  return *(uint8_t*)((uint8_t*)buffer_ + position_++);
}

char* ccFileMemory::gets(char* buffer, int size)
{
  size--;
  long ch, index = 0;

  while (index < size) {
    ch = get_char();

    if (ch == ccFile::kEOF)
      return 0;

    buffer[index++] = (char)ch;

    if (ch == '\n')
      break;
  }

  buffer[index] = 0;

  return buffer;
}

long ccFileMemory::scanf(const char* format, void* output) { return 0; }

bool ccFileMemory::alloc(size_t alloc_size)
{
  if (alloc_size > (size_t)edge_) {
    // find new buffer size
    size_t new_buffer_size = (size_t)(((alloc_size >> 16) + 1) << 16);

    // allocate new buffer
    if (buffer_ == NULL)
      buffer_ = (uint8_t*)malloc(new_buffer_size);
    else
      buffer_ = (uint8_t*)realloc(buffer_, new_buffer_size);

    // I own this buffer now (caller knows nothing about it)
    free_on_close_ = true;

    edge_ = new_buffer_size;
  }

  return (buffer_ != 0);
}

void ccFileMemory::free_buffer() { close(); }
}