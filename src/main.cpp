#include "main.hpp"



void markdown_help::start() {
    if(arguments.has("v") || arguments.has("h")) {
        std::cout << "markdown-help " << markdown_help_version << "\nCopyright (C) Kacper Bugla 'Kapi' 2022\n\n";
        exit(0);
    }

    if(arguments.has("d")) {
        std::cin.get();
    }

    proj.load_project("mh.config.json");

    if(arguments.has("o")) {
        proj.output_path = std::filesystem::absolute(arguments.get("o"));
    }
    
    if(arguments.has("t")) {
        proj.temp_path = std::filesystem::absolute(arguments.get("t"));
    }

    proj.prepare_build();
}


void markdown_help::run() {
    proj.compile();
}


void markdown_help::end() {
    std::cout << "\n";
}



APF_MAIN(markdown_help)