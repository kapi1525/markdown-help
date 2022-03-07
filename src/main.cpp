#include "main.hpp"



void markdown_help::start() {
    if(arguments.has("v")) {
        std::cout << "markdown-help " << markdown_help_version << "\nCopyright (C) Kacper Bugla 'Kapi' 2022\n";
        std::cout << "\n";
        exit(0);
    }

    if(arguments.has("h")) {
        std::cout << "Program syntax: markdown-help [options]\n";
        std::cout << "\n";
        std::cout << "Avaiable options:\n";
        std::cout << "-v            Show version string.\n";
        std::cout << "-h            Show this help message.\n";
        std::cout << "-d            Wait for user to press enter before executing, usefull for ataching debugger.\n";
        std::cout << "-i <name>     Creates empty project with specified name.\n";
        std::cout << "-p <path>     Path to directory with mh.json or file that should be used instead mh.json as config.\n";
        std::cout << "-o <path>     Override output path.\n";
        std::cout << "-t <path>     Override temporary path.\n";
        std::cout << "\n";
        exit(0);
    }

    if(arguments.has("d")) {
        std::cout << "Press enter to continue";
        std::cin.get();
    }

    if(arguments.has("i")) {
        std::string_view name = arguments.get("i");
        if (!name.empty()) {
            std::cout << "Creating empty project \"" << name << "\"...\n";
            std::ofstream proj_file("mh.json");
            if(proj_file.is_open()) {
                proj_file << "{\n";
                proj_file << "    \"project\": {\n";
                proj_file << "        \"name\": \""<< name <<"\",\n";
                proj_file << "        \"default-file\": \"file.md\"\n";
                proj_file << "    },\n\n";
                proj_file << "    \"menu\": [[\"Menu item\", \"file.md\"]],\n";
                proj_file << "    \"files\": [\"file.md\"]\n";
                proj_file << "}\n";
            } else {
                std::cerr << "Failed to create file!\n\n";
                exit(-1);
            }

            std::ofstream md_file("file.md");
            if(md_file.is_open()) {
                md_file << "\n# Markdown file\n\nBlah Blah Blah Blah......";
            } else {
                std::cerr << "Failed to create file!\n\n";
                exit(-1);
            }
        } else {
            std::cerr << "When using -i you must pass project name, for more info type -h for help.\n\n";
            exit(-1);
        }
        
        std::cout << "\n";
        std::cout << "Created empty project \"" << name << "\"\n";
        std::cout << "To build your project run: \"markdown-help\" in this directory.\n";
        std::cout << "\n";
        exit(0);
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

    proj = new project(project_file.string());

    if(arguments.has("o")) {
        proj->output_path = std::filesystem::absolute(arguments.get("o"));
    }
    if(arguments.has("t")) {
        proj->temp_path = std::filesystem::absolute(arguments.get("t"));
    }
}


void markdown_help::run() {
    if(!proj->build()) {
        apf::log::fatal("Compilation terminated.");
        abort();
    }
}


void markdown_help::end() {
    std::cout << "\n";
}



APF_MAIN(markdown_help)