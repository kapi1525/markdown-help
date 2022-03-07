#include "project.hpp"



bool project::build() {
    // Remove old temp and out folders
    if(std::filesystem::remove_all(temp_path) == (uintmax_t)-1 || std::filesystem::remove_all(output_path) == (uintmax_t)-1) {
        apf::log::fatal("Failed to remove temp or output paths.");
        return false;
    }


    // Create temp and out folders
    if(!std::filesystem::create_directories(temp_path) || !std::filesystem::create_directories(output_path)) {
        apf::log::fatal("Failed to create temp or output paths.");
        return false;
    }


    // Create gitignores
    if(create_gitignore) {
        apf::file::write(temp_path / ".gitignore", "*");
        apf::file::write(output_path / ".gitignore", "*");
    }


    // Copy files to temp (to be 200% sure it wont override or corrupt existing files)
    {
        apf::log::info("Copying files...");
        std::filesystem::path from, to;
        for (size_t i = 0; i < files.size(); i++) {
            apf::log::info(files[i].string());
            from = files[i];
            to = temp_path / std::filesystem::relative(from, proj_file.parent_path());
            std::filesystem::create_directories(to.parent_path());
            std::filesystem::copy_file(from, to);
            files[i] = to;
        }
    }


    // Convert md to html
    if(convert_md_to_html) {
        apf::log::info("Converting markdown to html...");
        for (size_t i = 0; i < files.size(); i++) {
            if(files[i].extension() == ".md") {
                apf::file::write(files[i].replace_extension(".html"), md_to_html(apf::file::read(files[i])));
                apf::log::info(files[i].string());
            }
        }
    }
    
    toc_file /= name;
    toc_file += ".hhc";

    hhp_file /= name;
    hhp_file += ".hhp";

    chm_file /= name;
    chm_file += ".chm";


    // Create table of contents (.hhc)
    apf::log::info("Generating table of contents file: \"" + (temp_path / toc_file).string() + "\"...");
    apf::file::write(temp_path / toc_file, export_toc());


    // Create html help project (.hhp)
    apf::log::info("Generating html help project file: \"" + (temp_path / hhp_file).string() + "\"...");

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
    for (size_t i = 0; i < files.size(); i++) {
        ss << std::filesystem::relative(files[i], temp_path).string() << "\n";
    }

    apf::file::write(temp_path / hhp_file, ss.str());


    // Run compiler (chmcmd or hhc)
    std::filesystem::current_path(temp_path);   // chmcmd creates chm file in current path.
    apf::log::custom("CMD", ("chmcmd \"" + hhp_file.string() + "\""));
    if(system(("chmcmd \"" + hhp_file.string() + "\"").c_str())) {
        apf::log::fatal("chmcmd didnt return 0.");
        return false;
    }


    // Copy chm to out
    if(!std::filesystem::copy_file(temp_path / chm_file, output_path / chm_file)) {
        apf::log::fatal("Failed to copy: \"" + chm_file.string() + "\" from: \"" + (temp_path / chm_file).string() + "\" to \"" + (output_path / chm_file).string() + "\".");
        return false;
    }


    return true;
}



std::string project::export_toc(toc_item* item) {
    std::stringstream ss;
    if(item == nullptr) {
        for (size_t i = 0; i < table_of_contents.size(); i++) {
            ss << "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\"><HTML><HEAD></HEAD><BODY><OBJECT type=\"text/site properties\"></OBJECT><UL>";
            ss << export_toc(&table_of_contents[i]);
            ss << "</UL></BODY></HTML>";
        }
        return ss.str();
    }
    
    else {
        ss << "<LI><OBJECT type=\"text/sitemap\"><param name=\"Name\" value=\"" << item->name << "\"/>";
        if(item->file != nullptr) {
            ss << "<param name=\"Local\" value=\"" << std::filesystem::relative(*item->file, temp_path).string() << "\"/>";
        }
        ss << "</OBJECT>";
        if(item->contents != nullptr && item->contents->size() > 0) {
            ss << "<UL>";
            for (size_t i = 0; i < item->contents->size(); i++) {
                ss << export_toc(&item->contents->at(i));
            }
            ss << "</UL>";
        }
        ss << "</LI>";

        return ss.str();
    }
}



std::string project::md_to_html(std::string markdown) {
    maddy::Parser* parser = new maddy::Parser();
    std::stringstream ss(markdown);
    std::string html = parser->Parse(ss);
    delete parser;
    return "<!DOCTYPE html><html><head></head><body>" + html + "</body></html>";
}