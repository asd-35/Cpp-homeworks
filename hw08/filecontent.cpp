#include "filecontent.h"

// TODO implement constructors

// TODO implement member functions


FileContent::FileContent(const std::string& content): s{std::make_shared<std::string>(content)} {}
FileContent::FileContent(std::string&& content): s{std::make_shared<std::string>(std::move(content))}{};
FileContent::FileContent(const char* content) : s(std::make_shared<std::string>(content)) {}

size_t FileContent::get_size() const{
    return s->size();
}

std::shared_ptr<const std::string> FileContent::get() const{
    return s;
};

