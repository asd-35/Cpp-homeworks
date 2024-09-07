#include "document.h"


Document::Document(FileContent &&content) : File{std::move(content)} {}

// TODO implement get_type function
std::string_view Document::get_type() const{
  return "DOC";
}


size_t Document::get_raw_size() const {
  // TODO get the document size
  return get_size();
}

unsigned Document::get_character_count() const {
  // TODO count non whitespace characters
  std::string data = *content.get();

  unsigned count = 0;
  
  for(auto x: data){
    if(x == ' ' || x == '\n'|| x == '\t')
      continue;
    else
      ++count;
  }
  return count;
}

// TODO implement content update function
void Document::update(FileContent &&new_content){
    File::content = std::move(new_content);
};