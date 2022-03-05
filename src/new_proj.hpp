#include <string>
#include <deque>
#include <filesystem>
#include "apf/logs.hpp"
#include "nlohmann/json.hpp"



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

    std::filesystem::path output_path = "out";
    std::filesystem::path temp_path   = "temp";

    bool create_gitignore = true;

    toc table_of_contents;

    std::deque<std::filesystem::path> files;


private:
    void load_toc(nlohmann::json* json, toc_item* item);
    std::filesystem::path* get_file_pointer(std::filesystem::path file);

};

