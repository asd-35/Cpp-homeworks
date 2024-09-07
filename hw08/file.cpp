#include "file.h"

#include "filesystem.h"

size_t File::get_size() const { return this->content.get_size(); }

bool File::rename(std::string_view new_name) {
  // TODO: Check that a filesystem actually exists, then rename it in the filesystem 
    // i know not my best block of code :)
    auto fs = filesystem.lock();
    if (!fs) {
        return false;
    }
    
    if (fs->get_file(new_name) != nullptr) {
        return false;  
    }

    if(fs->rename_file(name,new_name)){
        return true;
    }
    
}

const std::string &File::get_name() const { return name; }

const FileContent &File::get_content() const { return content; }

// TODO file constructor
File::File(FileContent&& content,std::string_view name) : content(std::move(content)), name(name.data()) {}

