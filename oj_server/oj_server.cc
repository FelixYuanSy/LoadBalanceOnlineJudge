#include <iostream>
#include "../comm/httplib.h"
#include "oj_control.hpp"
#include "oj_view.hpp"

using namespace httplib;
using namespace ns_control;
int main()
{
    Server svr;
    Control ctrl;
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
    svr.Get(R"(/judge/(\d+))", [](const Request &req, Response &resp)
            {
        std::string number = req.matches[1];//用0表示匹配整个地址,1表示捕获第一个捕获组内容(括号里)
        resp.set_content("指定题目的判题: "+number, "text/html;charset=utf-8"); });

    svr.set_base_dir("./wwwroot");
    svr.listen("0.0.0.0", 8089);
    return 0;
}