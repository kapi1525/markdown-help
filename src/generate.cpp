#include "project.hpp"



void project::generate_chm() {
    for (size_t i = 0; i < files.size(); i++) {
        if(files[i].extension().compare(".md") == 0) {
            _convert_to_html(files[i], std::filesystem::current_path() / temp_path / std::filesystem::relative(files[i]).replace_extension("html"));
        } else {
            std::cout << (std::filesystem::current_path() / temp_path / std::filesystem::relative(files[i])).string() << "\n";
            std::filesystem::copy_file(files[i], std::filesystem::current_path() / temp_path / std::filesystem::relative(files[i]), std::filesystem::copy_options::overwrite_existing);
        }
    }
}


void project::_convert_to_html(std::filesystem::path file_path, std::filesystem::path out_file_path) {
    std::cout << out_file_path.string() << "\n";

    std::filesystem::create_directories(out_file_path.parent_path());
    std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>();

    std::ifstream file(file_path);
    std::ofstream out(out_file_path);
    if(file.is_open() && out.is_open()) {
        std::stringstream ss;
        ss << file.rdbuf();
        out << parser->Parse(ss);
    } else {
        std::cerr << "Failed to open file!\n";
        std::cerr << "Since i programed this in this way you have no idea opening what file failed, yay!\n";
        exit(-1);
    }
}