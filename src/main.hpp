#pragma once
#include <deque>
#include "appframework.hpp"
#include "project.hpp"



class markdown_help : public apf::app {
public:
    void start();
    void run();
    void end();

    project proj;

    version markdown_help_version = {0,1,0};
};