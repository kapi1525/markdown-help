#include "project.hpp"



project::project(std::filesystem::path file_path) {
    proj_file = std::filesystem::absolute(file_path).make_preferred();

    apf::log::info("Loading project file: \"" + proj_file.string() + "\"...");
    apf::log::note("Currently no chechs are made for required options in json file, if json file dosent contain one of those markdown-help will crash.");

    nlohmann::json json;
    json = json.parse(std::stringstream(std::string(apf::file::read(proj_file))), nullptr, true, true);


    // Loading files
    for (size_t i = 0; i < json.at("files").size(); i++) {
        files.push_back(std::filesystem::absolute(json.at("files").at(i)));
    }

    // Loading project object
    name = json.at("project")["name"];
    default_file = get_file_pointer(json.at("project")["default-file"]);
    
    if(json.at("project").contains("output-path")) {
        output_path = std::filesystem::absolute(json.at("project")["output-path"]);
    }

    if(json.at("project").contains("temp-path")) {
        temp_path = std::filesystem::absolute(json.at("project")["temp-path"]);
    }

    if(json.at("project").contains("create-gitignore")) {
        create_gitignore = json.at("project")["create-gitignore"];
    }

    // Loading toc/menu
    table_of_contents.resize(json.at("menu").size());
    for (size_t i = 0; i < json.at("menu").size(); i++) {
        load_toc(&json.at("menu")[i], &table_of_contents[i]);
    }
}


project::~project() {
    for (size_t i = 0; i < table_of_contents.size(); i++) {
        unload_toc(&table_of_contents[i]);
    }
}



bool project::build() {
    return false;
}



void project::load_toc(nlohmann::json* json, toc_item* item) {
    if(json->size() == 2 && json->at(0).is_string() && json->at(1).is_string()) {
        item->name = json->at(0);
        item->file = get_file_pointer(json->at(1));
    }
    
    else if (json->size() == 2 && json->at(0).is_string() && json->at(1).is_array()) {
        item->name = json->at(0);

        item->contents = new std::deque<toc_item>(json->at(1).size());

        for (size_t i = 0; i < json->at(1).size(); i++) {
            item->contents->push_back(toc_item());
            load_toc(&json->at(1)[i], &item->contents->at(i));
        }
    }

    else if (json->size() == 3 && json->at(0).is_string() && json->at(1).is_string() && json->at(2).is_array()) {
        item->name = json->at(0);
        item->file = get_file_pointer(json->at(1));

        item->contents = new std::deque<toc_item>(json->at(2).size());

        for (size_t i = 0; i < json->at(2).size(); i++) {
            item->contents->push_back(toc_item());
            load_toc(&json->at(2).at(i), &item->contents->at(i));
        }
    }
}


void project::unload_toc(toc_item* item) {
    if(item->contents != nullptr) {
        for (size_t i = 0; i < item->contents->size(); i++) {
            unload_toc(&item->contents->at(i));
        }
        delete item->contents;
    }
}



std::filesystem::path* project::get_file_pointer(std::filesystem::path file) {
    file = std::filesystem::absolute(file).make_preferred();

    if(!std::filesystem::exists(file)) {
        apf::log::fatal("File: \"" + file.string() + "\", dosent exist.");
        abort();
    }

    for (size_t i = 0; i < files.size(); i++) {
        if(file == files[i]) {
            return &files[i];
        }
    }
    
    apf::log::fatal("File: \"" + file.string() + "\", wasnt found in files deque.");
    abort();
}