#include "project.hpp"



void project::prepare_build() {
    _prepare_dirs();
    _convert_files_to_html();
    _create_hhp();
    _create_hhc();
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
    ss << "Contents File=" << name << ".hhc" << "\n";
    ss << "Default Window=main" << "\n";
    ss << "Full-text search=Yes" << "\n";
    ss << "Title=" << name << "\n";

    ss << "[WINDOWS]" << "\n";
    ss << "main=\""<< name <<"\",\"" << name << ".hhc" << "\",,\"" << std::filesystem::relative(*default_file, temp_path).string() << "\",\"" << std::filesystem::relative(*default_file, temp_path).string() << "\",,,,,0x23520,200,0x1046,,0xB0000,,,,,,0" << "\n";

    ss << "[FILES]" << "\n";
    // TODO: test this on windows.
    //ss << name << ".hhp\n"; idk if its needed chmcmd works without it.
    for (size_t i = 0; i < files.size(); i++) {
        ss << std::filesystem::relative(files[i], temp_path).string() << "\n";
    }

    std::filesystem::path path = temp_path / name;
    path.concat(".hhp");
    write(path, ss.str());
}



void project::_create_hhc() {
    std::stringstream ss;
    ss << "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\"><HTML><HEAD></HEAD><BODY><OBJECT type=\"text/site properties\"></OBJECT><UL>";
    ss << _get_menu_html();
    ss << "</UL></BODY></HTML>";

    std::filesystem::path path = temp_path / name;
    path.concat(".hhc");
    write(path, ss.str());
}


// <LI><OBJECT type="text/sitemap"><param name="Name" value="name"><param name="Local" value="file"></OBJECT></LI>
std::string project::_get_menu_html(menu_item* item) {
    std::stringstream ss;
    if(item == nullptr) {
        for (size_t i = 0; i < menu.size(); i++) {
            ss << _get_menu_html(&menu[i]);
        }
        return ss.str();
    }
    
    else {
        ss << "<LI><OBJECT type=\"text/sitemap\"><param name=\"Name\" value=\"" << item->name << "\"/>";
        if(item->file != nullptr) {
            ss << "<param name=\"Local\" value=\"" << std::filesystem::relative(*item->file, temp_path).string() << "\"/>";
        }
        ss << "</OBJECT>";
        if(item->contents->size() > 0) {
            ss << "<UL>";
            for (size_t i = 0; i < item->contents->size(); i++) {
                ss << _get_menu_html(&item->contents->at(i));
            }
            ss << "</UL>";
        }
        ss << "</LI>";

        return ss.str();
    }
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