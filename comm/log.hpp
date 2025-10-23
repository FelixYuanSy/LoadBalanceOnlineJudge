#pragma once
#include <iostream>
#include <fstream>
#include "util.hpp"
namespace ns_log
{
    using namespace ns_util;
    enum
    {
        INFO,
        DEBUG,
        WARNING,
        ERROR,
        FATAL
    };

    // 使用inline加快调用速度
    inline std::ostream &Log(const std::string &level, const std::string &file_name, int line)
    {
        std::string message = "[";
        message += level;
        message += "]";

        message += "[";
        message += file_name;
        message += "]";

        message += "[";
        message += std::to_string(line);
        message += "]";

        message += "[";
        message += TimeUtil::GetTimeStamp();
        message += "]";

        std::cout << message;
        return std::cout;
    }
#define LOG(level) ns_log::Log(#level, __FILE__, __LINE__)
    // 这个宏不能在犯加分号错误!
}