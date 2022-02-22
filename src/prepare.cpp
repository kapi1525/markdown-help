#include "project.hpp"



void project::prepare_build() {
    _prepare_dirs();
    _convert_files_to_html();
    debug_print_menu();
    _create_hhp();
}


void project::_convert_files_to_html() {
    std::cout << "Converting files to html format...\n";
    for (size_t i = 0; i < files.size(); i++) {
        if(files[i].extension().compare(".md") == 0) {
            std::cout << files[i] << "\n";
            std::filesystem::path newpath = std::filesystem::absolute(temp_path / std::filesystem::relative(files[i]).replace_extension("html"));
            write(newpath, _markdown_to_html(read(files[i])));
            files[i] = newpath;
        }
    }
}


std::string project::_markdown_to_html(std::string_view input) {
    std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>();
    std::stringstream ss;
    ss << input;
    return "<!DOCTYPE html><html><head></head><body>" + parser->Parse(ss) + "</body></html>";
}



void project::_create_hhp() {
    std::stringstream ss;
    ss << "[OPTIONS]" << "\n";
    ss << "Auto Index=Yes" << "\n";
    ss << "Binary TOC=Yes" << "\n";
    ss << "Compiled File=" << name << ".chm" << "\n";
    ss << "Contents File=" << "\n";
    ss << "Default Window=main" << "\n";
    ss << "Full-text search=Yes" << "\n";
    ss << "Title=" << name << "\n";

    ss << "[WINDOWS]" << "\n";
    ss << "main=\""<< name <<"\",\"" << "\",,\"" << default_file << "\",\"" << default_file << "\",,,,,0x23520,200,0x1046,,0xB0000,,,,,,0" << "\n";

    ss << "[FILES]" << "\n";
    //ss << name << ".hhp\n";
    for (size_t i = 0; i < files.size(); i++) {
        ss << std::filesystem::relative(files[i], temp_path).string() << "\n";
    }

    write(temp_path /= name + ".hhp", ss.str());
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