#pragma once
#include <iostream>
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
            AddSuffix(file_name, ".cpp");
        }
        static std::string Exe(const std::string &file_name)
        {
            AddSuffix(file_name, ".exe");
        }
        static std::string Stderr(const std::string &file_name)
        {
            AddSuffix(file_name, ".stderr");
        }
    };
}