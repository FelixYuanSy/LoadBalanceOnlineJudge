#pragma once
#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>
#include <atomic>

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
        static std::string Stdcout(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stdout");
        }
        static std::string CompileError(const std::string &file_name)
        {
            return AddSuffix(file_name, ".compile_error");
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
        
        static std::string UniqueFile()
        {
            // 在时间工具里增加读取毫秒
            // 原子加锁
            // 读取毫秒
            static std::atomic_uint id(0);
            id++;
            std::string ms_time = TimeUtil::GetMsTime();
            return ms_time + "_" + std::to_string(id);
        }
        static bool WriteFile(const std::string &file_name, const std::string &content)
        {
            std::ofstream out(file_name);
            if (!out.is_open())
                return false;
            out.write(content.c_str(), content.size());
            out.close();
            return true;
        }

        static bool ReadFile(const std::string &file_name, std::string *content, bool if_read_enter = false)
        {
            /* 利用输入流读取文件,用getline读取,注意getline自己默认取消换行符读取,如果需要要自己加上 */
            (*content).clear();
            std::ifstream in(file_name);
            if (!in.is_open())
            {
                return false;
            }
            std::string line;
            while (std::getline(in, line))
            {
                (*content) += line;
                (*content) += (if_read_enter ? "\n" : "");
            }
            in.close();
            return true;
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
        static std::string GetMsTime()
        {
            struct timeval time_val;
            gettimeofday(&time_val, nullptr);
            return std::to_string(time_val.tv_sec * 1000 + time_val.tv_usec * 1000);
        }
    };

}