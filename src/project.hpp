#pragma once
#include <string>
#include <deque>
#include <filesystem>
#include "apf/logs.hpp"
#include "apf/file.hpp"
#include "nlohmann/json.hpp"
#include "maddy/parser.h"



struct toc_item {
    std::string name;
    std::filesystem::path* file = nullptr;
    std::deque<toc_item>* contents = nullptr;
};

typedef std::deque<toc_item> toc;


class project {
public:
    project(std::filesystem::path file_path);
    ~project();

    bool build();

    std::string name;
    std::filesystem::path proj_file;
    std::filesystem::path toc_file;
    std::filesystem::path hhp_file;
    std::filesystem::path chm_file;

    std::filesystem::path* default_file;

    std::filesystem::path output_path = "out";
    std::filesystem::path temp_path   = "temp";

    bool create_gitignore = true;
    bool convert_md_to_html = true;

    toc table_of_contents;

    std::deque<std::filesystem::path> files;


private:
    void load_toc(nlohmann::json* json, toc_item* item);
    void unload_toc(toc_item* item);
    std::string export_toc(toc_item* item = nullptr);

    std::string md_to_html(std::string markdown);

    std::filesystem::path* get_file_pointer(std::filesystem::path file);

};

