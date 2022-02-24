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


enum compiler {
    // hhc,     // Microsoft html help workshop compiler (windows only)
    chmcmd      // Free pascal html help compiler        (multiplatform)
};


class project {
public:
    project();
    ~project();

    void load_project(std::filesystem::path file);
    void build();

    void prepare_build();
    void compile();

    void debug_print_menu(menu_item* item = nullptr, size_t level = 0);

    compiler c = chmcmd;


    std::filesystem::path project_file;


    std::string name;
    std::filesystem::path* default_file;

    std::filesystem::path output_path = "out";
    std::filesystem::path temp_path   = "temp";

    // deques everywhere!
    std::deque<menu_item> menu;

    // List of all files used in this project, other parts of projects point to thiese so if file path changes here it changes in entire project.
    std::deque<std::filesystem::path> files;


private:
    // Find pointer to specified file in files array.
    std::filesystem::path* get_file_pointer(std::filesystem::path file);

    void load_toc(nlohmann::json* json, menu_item* item);

    void _convert_files_to_html();
    std::string _markdown_to_html(std::string_view input);
    void _create_hhp(); // HTML Help project
    void _create_hhc(); // HTML Help contents (menu)
    std::string _get_menu_html(menu_item* item = nullptr);

    void _prepare_dirs();
    void _cleanup_dirs();

    // I made those only cause i hate std::ifstream and ofstream
    std::string read(std::filesystem::path file_path);
    void write(std::filesystem::path file_path, std::string_view text);
};