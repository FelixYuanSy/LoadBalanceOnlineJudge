#pragma once

#include "oj_model.hpp"
#include "oj_view.hpp"
#include "../comm/util.hpp"
#include "../comm/log.hpp"
#include "../comm/httplib.h"
#include <vector>
#include <assert.h>
#include <mutex>
#include <jsoncpp/json/json.h>
namespace ns_control
{
    using namespace ns_log;
    using namespace ns_util;
    using namespace ns_model;
    using namespace ns_view;
    using namespace httplib;

    class Machine
    {
    public:
        std::string ip;
        int port;
        uint64_t load;   // 负载
        std::mutex *mtx; // 锁不能被复制,所以建立一个指针

        Machine() : ip(" "), port(0), load(0), mtx(nullptr)
        {
        }
        ~Machine() {}
        void InLoad()
        {
            if (mtx)
                mtx->lock();
            ++load;
            if (mtx)
                mtx->unlock();
        }
        void DecLoad()
        {
            if (mtx)
                mtx->lock();
            --load;
            if (mtx)
                mtx->unlock();
        }

        uint64_t Load()
        {
            uint64_t _load = 0;
            if (mtx)
                mtx->lock();
            _load = load;

            if (mtx)
                mtx->unlock();
            return _load;
        }
    };

    const std::string service_machine = "./conf/service_machine.conf";
    class LoadBalance
    {
    private:
        std::vector<Machine> machines;
        // 在线主机
        std::vector<int> online;
        std::vector<int> offline;
        std::mutex mtx;

    public:
        LoadBalance()
        {
            assert(LoadConf(service_machine));
            LOG(INFO) << "加载" << service_machine << "成功" << std::endl;
        }
        bool LoadConf(const std::string &machine_conf)
        {
            std::ifstream in(machine_conf);
            if (!in.is_open())
            {
                LOG(FATAL) << "载入" << machine_conf << "失败" << std::endl;
                return false;
            }
            std::string line;
            while (std::getline(in, line))
            {
                std::vector<std::string> tokens;
                StringUtil::SplitString(line, &tokens, ":");
                if (tokens.size() != 2)
                {
                    LOG(WARNING) << "切分" << line << "失败" << std::endl;
                    continue;
                }
                Machine m;
                m.ip = tokens[0];
                m.port = atoi(tokens[1].c_str());
                m.load = 0; // 初始负载0
                m.mtx = new std::mutex();
                online.push_back(machines.size());
                machines.push_back(m);
            }
            in.close();
            return true;
        }

        bool SmartChoice(int *id, Machine **m)
        {
            mtx.lock();
            // 1.根据负载选择主机
            // 先找到在线的机器,如果没有在线的报错
            int online_num = online.size();
            if (online_num == 0)
            {
                mtx.unlock();
                LOG(FATAL) << "后台编译机器全部离线" << std::endl;
                return false;
            }
            // 编号就是在线机器,用hash查询
            *id = online[0];
            *m = &machines[online[0]];
            uint64_t min_load = machines[online[0]].Load();
            for (int i = 0; i < online_num; i++)
            {
                if (min_load > machines[online[i]].Load())
                {
                    uint64_t current_load = machines[online[i]].Load();
                    min_load = current_load;
                    *id = online[i];
                    *m = &machines[online[i]];
                }
            }

            mtx.unlock();
            return true;
        }
        void OfflineMachine() {}
        void OnlineMachine() {}
    };
    class Control
    {

    private:
        Model _model;
        View _view;
        LoadBalance load_blance;

    public:
        Control() {}
        ~Control() {}

        bool AllQuestions(std::string *html)
        {
            bool ret = true;
            std::vector<struct Question> all;
            if (_model.GetAllQuestions(&all))
            {
                // 用view方法的将数据构建成网页
                _view.AllExpandHtml(all, html);
            }
            return ret;
        }

        bool Question(std::string number, std::string *html)
        {
            struct Question q;
            if (_model.GetQuestion(number, &q))
            {
                // 单独题目网页
                _view.OneExpandHtml(q, html);
            }
            return true;
        }

        void Judge(const std::string &number, const std::string in_json, const std::string *out_json)
        {
            //根据题目编号来获取对应题目细节
            struct Question q;
            _model.GetQuestion(number,&q);
            //根据传过来的json先进行反序列化,把代码和测试代码进行结合
            Json::Reader reader;
            Json::Value in_value;
            reader.parse(in_json,in_value);
            std::string code = in_value["code"].asString();
            Json::Value compile_value;
            compile_value["code"] = code+q.tail;
            compile_value["input"] = in_value["input"].asString();
            compile_value["cpu_limit"] = q.cpu_limit;
            compile_value["mem_limit"] = q.mem_limit;

            Json::FastWriter writer;
            std::string compile_string = writer.write(compile_value);


        }
    };
}