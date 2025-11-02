#pragma once
#include "oj_control.hpp"
// #include "oj_model.hpp"
#include "oj_model_mysql.hpp"
#include <vector>
#include <ctemplate/template.h>

namespace ns_view
{
    using namespace ns_control;
    using namespace ns_model;

    std::string template_path = "./template_html/";
    class View
    {
    private:
    public:
        void AllExpandHtml(std::vector<Question> &all_questions, std::string *html)
        {
            std::string src_html = template_path + "all_questions.html";
            ctemplate::TemplateDictionary root("all_questions");
            for (const auto &q : all_questions)
            {
                ctemplate::TemplateDictionary *sub = root.AddSectionDictionary("question_list");
                sub->SetValue("number", q.number);
                sub->SetValue("name", q.name);
                sub->SetValue("level", q.level);
            }
            ctemplate::Template *tpl = ctemplate::Template::GetTemplate(src_html, ctemplate::DO_NOT_STRIP);
            tpl->Expand(html, &root);
        }

        void OneExpandHtml(Question &question, std::string *html)
        {
            std::string src_html = template_path + "one_question.html";
            ctemplate::TemplateDictionary root("one_question");
            root.SetValue("number", question.number);
            root.SetValue("name", question.name);
            root.SetValue("level", question.level);
            root.SetValue("desc", question.desc);
            root.SetValue("pre_code", question.header);
            ctemplate::Template *tpl = ctemplate::Template::GetTemplate(src_html, ctemplate::DO_NOT_STRIP);
            tpl->Expand(html, &root);
        }
    };
}