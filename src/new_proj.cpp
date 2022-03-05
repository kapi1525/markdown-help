#include "new_proj.hpp"



project::project(std::filesystem::path file_path) {

}


project::~project() {

}


bool project::build() {

}



void project::load_toc(nlohmann::json* json, toc_item* item) {

}


std::filesystem::path* project::get_file_pointer(std::filesystem::path file) {
    file = std::filesystem::absolute(file).make_preferred();

    if(!std::filesystem::exists(file)) {
        apf::log::fatal("File: \"" + file.string() + "\", dosent exist.");
    }

    for (size_t i = 0; i < files.size(); i++) {
        /* code */
    }
    
}