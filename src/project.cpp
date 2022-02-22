#include "project.hpp"



project::project() {}
project::~project() {}

void project::load_project(std::filesystem::path path) {
    path = std::filesystem::absolute(path);
    std::stringstream ss(read(path));
    nlohmann::json json;
    ss >> json;

    _load_project_json_project(&json);
    _load_project_json_files(&json);
    _load_project_json_menu(&json);
}


void project::_load_project_json_project(nlohmann::json* json) {
    if(json->contains("project")) {
        if(json->at("project").contains("name") && json->at("project")["name"].is_string()) {
            name = json->at("project")["name"];
        } else {
            std::cerr << "Project has no name!\n";
            exit(-1);
        }

        if(json->at("project").contains("default-file") && json->at("project")["default-file"].is_string()) {
            default_file = json->at("project")["default-file"];
        } else {
            std::cerr << "Project has to have default-file!\n";
            exit(-1);
        }
    } else {
        std::cerr << "json file must contain project object!\n";
        exit(-1);
    }
}

void project::_load_project_json_files(nlohmann::json* json) {
    if (json->contains("files") && json->at("files").is_array() && json->at("files")[0].is_string()) {
        for (size_t i = 0; i < json->at("files").size(); i++) {
            if(json->at("files")[i].is_string()) {
                files.push_back(std::filesystem::absolute(json->at("files")[i]));
            } else {
                std::cerr << "One of the files in json file is not a string!\n";
            }
        }
    } else{
        std::cerr << "No files found in json file, generated help file will be empty!\n";
    }
}

void project::_load_project_json_menu(nlohmann::json* json, menu_item* item) {
    if(item == nullptr) {
        if(!json->contains("menu")) {
            std::cerr << "No menu defined in json file!\n";
            return;
        }

        for (size_t i = 0; i < json->at("menu").size(); i++) {
            menu.push_back(menu_item());
            _load_project_json_menu(&json->at("menu")[i], &menu[i]);
        }

        debug_print_menu();
    } 

    else {
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
                _load_project_json_menu(&json->at(1)[i], &item->contents->at(i));
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
                _load_project_json_menu(&json->at(2).at(i), &item->contents->at(i));
            }
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