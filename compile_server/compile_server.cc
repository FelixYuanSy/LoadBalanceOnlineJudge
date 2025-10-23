#include "compiler.hpp"

using namespace ns_compiler;
int main()
{
    std::string file_name = "test";
    Compiler::Compile(file_name);

    return 0;
}