#include "project.hpp"



void project::generate_chm() {
    _prepare_dirs();

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

    std::stringstream ss;
    ss << read(file_path);
    write(out_file_path, parser->Parse(ss));
}



void project::_prepare_dirs() {
    if(output_path != std::filesystem::current_path()) {
        std::filesystem::create_directories(output_path);
        write(output_path / ".gitignore", "*");
    }
    if(temp_path != std::filesystem::current_path()){
        std::filesystem::create_directories(temp_path);
        write(temp_path / ".gitignore", "*");
    }
}

void project::_cleanup_dirs() {
    std::cout << "Cleaning up...";
    std::cout << "Removed " << std::filesystem::remove_all(temp_path) << " files.";
}



std::string project::read(std::filesystem::path file_path) {
    std::ifstream file(file_path);
    if(file.is_open()) {
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    } else {
        std::cerr << "Failed to read file: " << file_path << ", make sure that file exists!";
        exit(-1);
    }
}

void project::write(std::filesystem::path file_path, std::string_view text) {
    std::ofstream file(file_path);
    if(file.is_open()) {
        file << text;
    } else {
        std::cerr << "Failed to write/create file: " << file_path << "!";
        exit(-1);
    }
}