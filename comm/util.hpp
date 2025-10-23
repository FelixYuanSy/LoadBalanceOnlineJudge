#pragma once
#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>

namespace ns_util
{
    // 例如./temp/1234.cpp
    const std::string temp_path = "./temp/";
    class PathUtil
    {
    public:
        static std::string AddSuffix(const std::string &file_name, const std::string suffix)
        {
            std::string path_name = temp_path;
            path_name += file_name;
            path_name += suffix;
            return path_name;
        }

        static std::string Src(const std::string &file_name)
        {
            return AddSuffix(file_name, ".cpp");
        }
        static std::string Exe(const std::string &file_name)
        {
            return AddSuffix(file_name, ".exe");
        }
        static std::string Stderr(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stderr");
        }
        static std::string Stdcin(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stdin");
        }
        static std::string Stdout(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stdout");
        }
    };

    class FileUtil
    {
    public:
        static bool IsFileExist(const std::string &path_name)
        {
            struct stat st;
            if (stat(path_name.c_str(), &st) == 0)
            {
                return true;
            }
            return false;
        }
    };
    class TimeUtil
    {
    public:
        static std::string GetTimeStamp()
        {
            struct timeval time_val;
            gettimeofday(&time_val, nullptr);
            return std::to_string(time_val.tv_sec);
        }
    };
}