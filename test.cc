#include <iostream>
#include <boost/algorithm/string.hpp>
#include <ctemplate/template.h>

int main()
{

    /*测试boost库*/
    // std::string str = "我想:吃:牛:肉";
    // std::string sub = ":";
    // std::vector<std::string> tokens;
    // boost::split(tokens,str,boost::is_any_of(sub),boost::algorithm::token_compress_on);
    // for(auto &iter : tokens)
    // {
    //     std::cout << iter << std::endl;
    // }
    // return 0;

    /*测试ctemplate*/
    std::string in_html = "./test.html";
    std::string value = "测试条目";
    // 创建数据字典
    ctemplate::TemplateDictionary root("test");
    root.SetValue("key", value);

    // 获取要被渲染的网页
    ctemplate::Template *tpl = ctemplate::Template::GetTemplate(in_html, ctemplate::DO_NOT_STRIP);

    // 添加字典到网页中
    std::string out_html;
    tpl->Expand(&out_html, &root);

    std::cout << out_html << std::endl;
}