#include <iostream>
#include <fmt/color.h>
#include "glm/vec4.hpp"
#include "imgui.h"
int main()
{
    std::cout << "Hello World!\n";
    fmt::print(fmt::emphasis::bold | fg(fmt::color::red),
        "Elapsed time: {0:.2f} seconds", 1.23);
}