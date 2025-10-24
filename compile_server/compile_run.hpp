#pragma once
#include "../comm/log.hpp"
#include "../comm/util.hpp"
#include "compiler.hpp"
#include "runner.hpp"
#include <jsoncpp/json/json.h>

namespace ns_compile_run
{
    using namespace ns_compiler;
    using namespace ns_log;
    using namespace ns_util;
    using namespace ns_runner;

    class CompileAndRun
    {
    public:
        static std::string CodetoDesc(int code, const std::string &file_name)
        {
            std::string desc;
            switch (code)
            {
            case 0:
                desc = "编译运行正常";
                break;
            case -1:
                desc = "提交代码为空";
                break;
            case -2:
                desc = "未知错误";
                break;
            case -3:
                FileUtil::ReadFile(PathUtil::CompileError(file_name), &desc, true);
                break;

            case SIGABRT:
                desc = "内存超过限制";

            case SIGXCPU:
                desc = "cpu使用超时";

            case SIGFPE:
                desc = "浮点数溢出";
            default:
                desc = "未知" + std::to_string(code);
                break;
            }
        }
        /*  start函数处理用户传进来的json文件
            code:用户的代码
            input:对应的输入

            cpu_limit:时间限制
            mem_limit:内存显示

            输出:
            status:状态码
            reason:请求结果

            如果运行成功:
            stdout 程序输出
            stderr 程序错误结果

        */
        static void start(const std::string &in_json, std::string *out_json)
        {
            Json::Value in_value;
            Json::Reader reader;
            reader.parse(in_json, in_value);
            // 把json数据转换类型
            std::string code = in_value["code"].asString();
            std::string input = in_value["input"].asString();
            int cpu_limit = in_value["cpu_limit"].asInt();
            int mem_limit = in_value["mem_limit"].asInt();

            // 决定要传出的数据
            /*
                status, reason, stdout , stderr
            */
            Json::Value out_value;
            int status_code = 0; // 运行的状态
            int run_result = 0;  // 运行结果(信号)
            std::string uq_name;

            // 检测code是否为空
            if (code.size() == 0)
            {
                status_code = -1; // 文件为空
            }

            // 用毫秒级时间戳+原子性 来确保文件路径唯一性(写一个确保唯一性文件的工具类)
            uq_name = FileUtil::UniqueFile();
            // 根据名称创建临时src文件
            if (!FileUtil::WriteFile(PathUtil::Src(uq_name), code))
            {
                status_code = -2; // unkonw error
                goto END;
            }

            if (!ns_compiler::Compiler::Compile(uq_name))
            {
                status_code = -3; // 编译错误
                goto END;
            }

            run_result = ns_runner::Runner::Run(uq_name);
            if (run_result < 0)
            {
                status_code = -2;
            }
            else if (run_result > 0)
            {
                // 直接获取运行结果来看出错
                status_code = run_result;
            }
            else
            {
                status_code = 0;
            }

        END:
            out_value["status"] = status_code;
            out_value["reason"] = CodetoDesc(status_code, uq_name);

            if (status_code == 0)
            {
                std::string _stdout;
                FileUtil::ReadFile(PathUtil::Stdcout(uq_name), &_stdout, true);
                out_value["stdout"] = _stdout;

                std::string _stderr;
                FileUtil::ReadFile(PathUtil::Stderr(uq_name), &_stderr, true);
                out_value["stdout"] = _stderr;
            }
            Json::StyledWriter writer;
            *out_json = writer.write(out_value);
        }
    };
}