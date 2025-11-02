#pragma once
#include <iostream>
#include <unordered_map>
#include <assert.h>
#include <vector>
#include <string>
#include "../comm/log.hpp"
#include "../comm/util.hpp"
#include "./include/mysql.h"

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
        const std::string oj_questions = "oj_questions";
    const std::string host = "127.0.0.1";
    const std::string user = "OJ_Client";
    const std::string passwd = "123456";
    const std::string db = "oj";
    const int port = 3306;
    class Model
    {

    public:
        Model()
        {
            
        }

       bool QueryMySql(const std::string &sql,std::vector<Question> *out)
       {
            MYSQL *my = mysql_init(nullptr);
            if(mysql_real_connect(my,host.c_str(),user.c_str(),passwd.c_str(),db.c_str(),port,nullptr,0) == nullptr)
            {
                LOG(FATAL)<<"连接数据库失败"<<std::endl;
                return false;
            }
             // 一定要设置该链接的编码格式, 要不然会出现乱码问题
            mysql_set_character_set(my, "utf8");
            LOG(INFO)<<"连接数据库成功呢"<<std::endl;

            if(mysql_query(my,sql.c_str())!=0)
            {
                LOG(WARNING)<<sql<<"execute error"<<std::endl;
                return false;
            }
            MYSQL_RES *res = mysql_store_result(my);
            int row = mysql_num_rows(res);
            int cols = mysql_num_fields(res);
            Question q;

            for(int i = 0 ; i<row;i++)
            {
                MYSQL_ROW row = mysql_fetch_row(res);
                q.number = row[0];
                q.name = row[1];
                q.level = row[2];
                q.desc = row[3];
                q.header = row[4];
                q.tail = row[5];
                q.cpu_limit = atoi(row[6]);
                q.mem_limit = atoi(row[7]);

                out->push_back(q);
            }
            free(res);
            mysql_close(my);
            return 0;


       }
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