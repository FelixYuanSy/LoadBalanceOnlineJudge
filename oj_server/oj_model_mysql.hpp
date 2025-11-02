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

    const std::string oj_questions = "oj_questions";
    class Model
    {

    public:
        Model()
        {
            
        }

       bool QueryMySql(const std::string &sql,std::vector<Question> *out){}
        bool GetAllQuestions(std::vector<Question> *out)
        {
            std::string sql = "select * from ";
            sql += oj_questions;
            return QueryMySql(sql,out);

        }

        bool GetQuestion(const std::string &number, Question *q)
        {
            bool res = false;
            std::string sql ="select * from ";
            sql+=oj_questions;
            sql += "where number=";
            sql+=number;
            std::vector<Question> result;
            if(QueryMySql(sql,&result))
            {
                if(result.size()==1)
                {
                    *q = result[0];
                    res = true;
                }
            }
            return res;
        }
    };
}