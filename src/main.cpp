#include "main.hpp"



void markdown_help::start() {
    std::cin.get();
    proj.load_project("mdhelp.json");
}


void markdown_help::run() {
}


void markdown_help::end() {
    std::cout << "\n";
}



APF_MAIN(markdown_help)