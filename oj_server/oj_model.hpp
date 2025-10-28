#pragma once
#include <iostream>
#include <unordered_map>
#include <assert.h>
#include <vector>
#include <string>
#include "../comm/log.hpp"
#include "../comm/util.hpp"

namespace ns_model
{
    using namespace ns_log;
    using namespace ns_util;

    struct Question
    {
        std::string number; // 序号
        std::string name;   // 名称
        std::string level;  // 难度 简单, 中等,困难
        int cpu_limit;      // 题目时间要求
        int mem_limit;      // 空间要求

        std::string desc;   // 题目描述
        std::string header; // 在线编辑代码
        std::string tail;   // 题目测试用例
    };

    const std::string questions_list = "./questions/questions_list";
    const std::string questions_path = "./questions/";
    class Model
    {
    private:
        std::unordered_map<std::string, Question> questions;

    public:
        Model()
        {
            assert(LoadQuestionList(questions_list));
        }

        bool LoadQuestionList(const std::string &questions_list)
        {
            std::ifstream in(questions_list);
            if (!in.is_open())
            {
                LOG(FATAL) << "加载题目库失败,检查是否存在题目库" << std::endl;
                return false;
            }
            std::string line;
            while (getline(in, line))
            {
                std::vector<std::string> tokens;
                // 切割字符串line后存入tokens

                if (tokens.size() != 5)
                {
                    LOG(WARNING) << "加载个别题目失败,检查文件" << std::endl;
                    continue;
                }
                // 存入Question结构体
                Question q;
                q.number = tokens[0];
                q.name = tokens[1];
                q.level = tokens[2];
                q.cpu_limit = atoi(tokens[3].c_str());
                q.mem_limit = atoi(tokens[4].c_str());

                std::string path = " ./questions/";
                path += q.number;
                path += "/";

                FileUtil::ReadFile(path + "desc.txt", &q.desc, true);
                FileUtil::ReadFile(path + "header.cpp", &q.header, true);
                FileUtil::ReadFile(path + "tail.cpp", &q.tail, true);

                questions.insert({q.number, q});
            }
            LOG(INFO) << "加载题库成功" << std::endl;
            in.close();
            return true;
        }
        bool GetAllQuestions(std::vector<Question> *out)
        {
            if (questions.size() == 0)
            {
                LOG(ERROR) << "用户题库获取失败" << std::endl;
                return false;
            }
            for (const auto &iter : questions)
            {
                out->push_back(iter.second);
            }
            return true;
        }

        bool GetQuestion(const std::string &number, Question *q)
        {
            const auto &iter = questions.find(number);
            if (iter == questions.end())
            {
                LOG(ERROR) << "用户获取题目:  " << number << "失败" << std::endl;
                return false;
            }
            (*q) = iter->second;
            return true;
        }
    };
}