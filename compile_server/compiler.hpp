#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string>
#include "../comm/util.hpp"
#include "../comm/log.hpp"
namespace ns_compiler
{
    using namespace ns_util;
    using namespace ns_log;
    class Compiler
    {
        Compiler() {}
        ~Compiler() {}

    public:
        static bool Compile(const std::string &file_name)
        {
            pid_t pid = fork();
            if (pid < 0)
            {
                LOG(ERROR) << "内部错误,创建子进程失败" << "\n";
                return false;
            }
            else if (pid == 0)
            {
                // 子进程干活:
                // 运行 g++ -o target src -std=c++11
                // 编写工具类来拼接后缀
                // 过来的记得写.c_str()符合c语言

                int _stderr = open(PathUtil::Stderr(file_name).c_str(), O_CREAT | O_WRONLY, 0644);
                if (_stderr < 0)
                {
                    LOG(ERROR) << "没有形成_stderr文件" << "\n";
                    exit(1);
                }
                dup2(_stderr, 2);
                execlp("g++", "g++", "-o", PathUtil::Exe(file_name).c_str(), PathUtil::Src(file_name).c_str(), "-std=c++11", nullptr);
                LOG(ERROR) << " 编译器执行失败" << "\n";
                exit(2);
            }
            else
            {

                waitpid(pid, nullptr, 0);
                if (FileUtil::IsFileExist(PathUtil::Exe(file_name)))
                {
                    LOG(INFO) << "编译成功" << "\n";
                    return true;
                }
                LOG(ERROR) << "编译失败,没有可执行程序" << "\n";
                return false;
            }
        }
    };
}