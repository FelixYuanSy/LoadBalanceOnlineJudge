#include <iostream>
#include "../comm/httplib.h"
#include "../comm/util.hpp"
#include "oj_control.hpp"
#include "oj_view.hpp"

using namespace httplib;
using namespace ns_control;
using namespace ns_util;
static Control *ctrl_ptr = nullptr;

void Recovery(int signo)
{
    ctrl_ptr->RecoveryMachine();
}
int main()
{
    Server svr;
    Control ctrl;
    signal(SIGQUIT, Recovery);
    // 获取所有题目列表
    svr.Get("/all_questions", [&ctrl](const Request &req, Response &resp)
            {   std::string html;
                ctrl.AllQuestions(&html);
                resp.set_content(html, "text/html; charset=utf-8"); });

    // 根据题号获取题目列表
    svr.Get(R"(/question/(\d+))", [&ctrl](const Request &req, Response &resp)
            { 
                std::string number = req.matches[1];
                std::string html;
                ctrl.Question(number,&html);
                resp.set_content(html, "text/html; charset=utf-8"); });
    // 判题页面
    svr.Post(R"(/judge/(\d+))", [&ctrl](const Request &req, Response &resp)
             {
        LOG(INFO) << "收到请求" <<std::endl;
        std::string number = req.matches[1];//用0表示匹配整个地址,1表示捕获第一个捕获组内容(括号里)
        std::string result_json;
        ctrl.Judge(number,req.body,&result_json);
        resp.set_content(result_json, "application/json;charset=utf-8"); });

    svr.set_base_dir("./wwwroot");
    svr.listen("0.0.0.0", 8080);
    return 0;
}