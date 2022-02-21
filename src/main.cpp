#include "main.hpp"



void markdown_help::start() {
    std::cin.get();
    proj.load_project("mdhelp.json");

    if(arguments.has("o")) {
        proj.output_path = std::filesystem::absolute(arguments.get("o"));
    }
    
    if(arguments.has("t")) {
        proj.temp_path = std::filesystem::absolute(arguments.get("t"));
    }

    proj.generate_chm();
}


void markdown_help::run() {
}


void markdown_help::end() {
    std::cout << "\n";
}



APF_MAIN(markdown_help)