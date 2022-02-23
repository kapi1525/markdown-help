#include "project.hpp"



project::project() {}
project::~project() {}


void project::load_project(std::filesystem::path file) {
    std::cout << "Loading project: " << file.string() << "...\n";

    std::stringstream ss(read(std::filesystem::absolute(file)));
    nlohmann::json json;
    ss >> json;

    // Loading files
    for (size_t i = 0; i < json.at("files").size(); i++) {
        files.push_back(std::filesystem::absolute(json.at("files").at(i)));
    }

    // Loading project object
    if(json.contains("project")) {
        if(json.at("project").contains("name")) {
            name = json.at("project")["name"];
        } else {
            std::cerr << "Project has no name, update json file!\n";
            exit(-1);
        }

        if(json.at("project").contains("default-file")) {
            default_file = get_file_pointer(json.at("project")["default-file"]);
        } else {
            std::cerr << "Project has no default-file, update json file!\n";
            exit(-1);
        }
    }
    
    // Loading toc/menu
    if(json.contains("menu")) {
        menu.resize(json.at("menu").size());
        for (size_t i = 0; i < json.at("menu").size(); i++) {
            load_toc(&json.at("menu")[i], &menu[i]);
        }
    } else {
        std::cerr << "No toc/menu found in json file, update json file!\n";
    }
}



std::filesystem::path* project::get_file_pointer(std::filesystem::path file) {
    file = std::filesystem::absolute(file);
    for (size_t i = 0; i < files.size(); i++) {
        if(files[i] == file) {
            return &files[i];
        }
    }

    std::cerr << "get_file_pointer(): Couldnt find pointer to file: " << file.string() << ".";
    exit(-1);
}


void project::load_toc(nlohmann::json* json, menu_item* item) {
    if(json->size() == 2 && json->at(0).is_string() && json->at(1).is_string()) {
        item->name = json->at(0);
        for (size_t i = 0; i < files.size(); i++) {
            if(files[i] == std::filesystem::absolute(json->at(1))) {
                item->file = &files[i];
            }
        }
    }
    
    else if (json->size() == 2 && json->at(0).is_string() && json->at(1).is_array()) {
        item->name = json->at(0);
        for (size_t i = 0; i < json->at(1).size(); i++) {
            item->contents->push_back(menu_item());
            load_toc(&json->at(1)[i], &item->contents->at(i));
        }
    }

    else if (json->size() == 3 && json->at(0).is_string() && json->at(1).is_string() && json->at(2).is_array()) {
        item->name = json->at(0);
        for (size_t i = 0; i < files.size(); i++) {
            if(files[i] == std::filesystem::absolute(json->at(1))) {
                item->file = &files[i];
            }
        }
        for (size_t i = 0; i < json->at(2).size(); i++) {
            item->contents->push_back(menu_item());
            load_toc(&json->at(2).at(i), &item->contents->at(i));
        }
    }
}



void project::debug_print_menu(menu_item* item, size_t level) {
    for (size_t i = 0; i < level; i++) {
        std::cout << "  ";
    }

    if(item == nullptr) {
        std::cout << "\nMenu:\n";
        for (size_t i = 0; i < menu.size(); i++) {
            debug_print_menu(&menu[i], level+1);
        }
        std::cout << "\n";
    }
    
    else {
        std::cout << item->name << " - ";
        if(item->file != nullptr) {
            std::cout << item->file->string();
        }
        std::cout << "\n";
        for (size_t i = 0; i < item->contents->size(); i++) {
            debug_print_menu(&item->contents->at(i), level+1);
        }
    }
}