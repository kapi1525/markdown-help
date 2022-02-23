#include "project.hpp"



void project::compile() {
    // FIXME: repleace system with something else.
    std::filesystem::path in = std::filesystem::absolute(temp_path) / name;
    std::filesystem::path out = std::filesystem::absolute(output_path) / name;
    in.concat(".chm");
    out.concat(".chm");
    std::filesystem::current_path(temp_path);
    std::stringstream cmd;
    cmd << "chmcmd \"" << name << ".hhp\"";
    system(cmd.str().c_str());
    std::filesystem::copy_file(in, out, std::filesystem::copy_options::overwrite_existing);
}