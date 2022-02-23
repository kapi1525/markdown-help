#include "main.hpp"



void markdown_help::start() {
    if(arguments.has("v") || arguments.has("h")) {
        std::cout << "markdown-help " << markdown_help_version << "\nCopyright (C) Kacper Bugla 'Kapi' 2022\n\n";
        exit(0);
    }

    if(arguments.has("d")) {
        std::cin.get();
    }

    std::filesystem::path project_file = "mh.json";

    if(arguments.has("p")) {
        project_file = arguments.get("p");
        if(!project_file.has_extension()) {
            project_file.concat("/mh.proj.json");
        }
    }

    project_file = std::filesystem::absolute(project_file);

    std::filesystem::current_path(project_file.parent_path());

    if(!std::filesystem::exists(std::filesystem::absolute(project_file))) {
        std::cerr << "Couldnt find project file " << project_file.string() << ".\n";
        exit(-1);
    }

    proj.load_project(project_file.string());

    if(arguments.has("o")) {
        proj.output_path = std::filesystem::absolute(arguments.get("o"));
    }
    if(arguments.has("t")) {
        proj.temp_path = std::filesystem::absolute(arguments.get("t"));
    }
}


void markdown_help::run() {
    proj.build();
}


void markdown_help::end() {
    std::cout << "\n";
}



APF_MAIN(markdown_help)