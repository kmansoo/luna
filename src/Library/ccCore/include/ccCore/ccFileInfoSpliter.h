/*
 * ccFileInfoSpliter.h
 *
 *  Created on: 2019.02.16
 *      Author: Mansoo Kim(mansoo.kim@stradvision.com)
 */

#pragma once

#include <string>

namespace Luna {

class ccFileInfoSpliter
{
public:
    ccFileInfoSpliter() {}
    ccFileInfoSpliter(const std::string& path);

    virtual ~ccFileInfoSpliter() {}

public:
    void    split(const std::string& path);

public:
    std::string     path_;

    std::string     drive_;
    std::string     dir_;
    std::string     name_;
    std::string     ext_;

    std::string     file_name_;

public:
    static bool split_path(const std::string& path, std::string& drive, std::string& dir, std::string& file_name, std::string& file_ext);
};

}