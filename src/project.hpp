#include <deque>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
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
    void generate_chm();

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

    void _convert_to_html(std::filesystem::path file_path, std::filesystem::path out_file_path);

    void _prepare_dirs();
    void _cleanup_dirs();

    // I made those only cause i hate std::ifstream and ofstream
    std::string read(std::filesystem::path file_path);
    void write(std::filesystem::path file_path, std::string_view text);
};