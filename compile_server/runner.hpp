#pragma once
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include "../comm/util.hpp"
#include "../comm/log.hpp"
#include <sys/wait.h>
namespace ns_runner
{
    using namespace ns_util;
    using namespace ns_log;
    class Runner
    {
    public:
        Runner() {}
        ~Runner() {}

        static int Run(const std::string &file_name)
        {
            // 定义变量减少书写
            std::string _execute = PathUtil::Exe(file_name);
            std::string _stdin = PathUtil::Stdcin(file_name);
            std::string _stdout = PathUtil::Stdcout(file_name);
            std::string _stderr = PathUtil::Stderr(file_name);

            // 创建文件0,1,2描述符对应文件
            // stdin只用RD,out和err需要写入权限
            umask(0); // 记得做权限初始化!
            int _stdin_fd = open(_stdin.c_str(), O_CREAT | O_RDONLY, 0644);
            int _stdout_fd = open(_stdout.c_str(), O_CREAT | O_WRONLY, 0644);
            int _stderr_fd = open(_stderr.c_str(), O_CREAT | O_WRONLY, 0644);

            if (_stdin_fd < 0 || _stdout_fd < 0 || _stderr_fd < 0)
            {
                LOG(ERROR) << "Runner: 打开标准描述符文件失败" << "\n";
                return -1;
            }

            pid_t pid = fork();
            if (pid < 0)
            {

                LOG(ERROR) << "RUN: 创建子进程失败" << "\n";
                // 防止文件描述符泄露,浪费句柄资源
                close(_stdin_fd);
                close(_stdout_fd);
                close(_stderr_fd);
                return -2;
            }
            else if (pid == 0)
            {
                // 重定向到文件里
                dup2(_stdin_fd, 0);
                dup2(_stdout_fd, 1);
                dup2(_stderr_fd, 2);

                // 执行调用
                execl(_execute.c_str(), _execute.c_str(), nullptr);
                exit(1);
            }
            else
            {
                // 等待子进程可以获取错误并直接返回
                int status = 0;
                waitpid(pid, &status, 0);
                // 程序运行异常，一定是因为因为收到了信号！
                LOG(INFO) << "运行完毕, info: " << (status & 0x7F) << "\n";
                return status & 0x7F;
            }
        }
    }
}