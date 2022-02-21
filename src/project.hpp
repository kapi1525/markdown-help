#include <deque>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include "maddy/parser.h"



struct menu_item {
    std::string name;
    std::filesystem::path* file;
    std::deque<menu_item>* contents = new std::deque<menu_item>;
};


class project {
public:
    project();
    ~project();

    void load_project(std::filesystem::path path);

    void debug_print_menu(menu_item* item = nullptr, size_t level = 0);

    std::string name;

    std::filesystem::path output_path = "out";
    std::filesystem::path temp_path   = "temp";

    std::deque<menu_item> menu;

    std::deque<std::filesystem::path> files;

private:
    void _load_project_json_project(nlohmann::json* json);
    void _load_project_json_files(nlohmann::json* json);
    void _load_project_json_menu(nlohmann::json* json, menu_item* item = nullptr);
};