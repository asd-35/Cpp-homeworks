#include "filesystem.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>

Filesystem::Filesystem() {}

bool Filesystem::register_file(const std::string &name,
                               std::shared_ptr<File> file) {
  // TODO: Do sanity checks first! Is that actually a good name, is the pointer set,
  //       does a file with that name already exists? Anything else to check here?
  auto it = files.find(name);
  if(name.empty() || !file || it != files.end()){
    return false;
  }
  // Creater a shared pointer to the this object
  auto thisptr = this->shared_from_this();

  // TODO: Check if the file already has a filesystem, if so, it may only be
  //       the same as this one, not another one!
  if(file->filesystem.lock()){
    return false;
  }

  file->filesystem = std::move(thisptr);

  // TODO: Now actually store it in the filesystem
  file->name = name;
  files.insert(std::make_pair(name,std::move(file)));
  
  return true;
}

bool Filesystem::remove_file(std::string_view name) {
  // TODO: Remove file from the filesystem, return true if successful, false otherwhise.
  //       false may indicate multple different reasons for failure, which is not impotant
  for(auto x: files){
    if(x.first == name.data()){
      files.erase(name.data());
      return true;
    }
  }
  return false;
}

bool Filesystem::rename_file(std::string_view source, std::string_view dest) {
  // TODO: Check filesystem for the source and dest files, if it's possible to rename
  //       the source file, then update the filesystem
  
  //is it possible to pull this of with  a for each loop ?
  auto itSource = files.find(source.data());
  auto itDest = files.find(dest.data());

  if (itSource != files.end() && itDest == files.end() && !source.empty() && !dest.empty()) {
      itSource->second->name = dest.data();
      files.emplace(dest.data(), std::move(itSource->second));
      files.erase(itSource);
      return true;
  }

  return false;
}

std::shared_ptr<File> Filesystem::get_file(std::string_view name) const {
  // TODO: Check the filesystem for the given file, return it if you found it,
  //       else just return nothing
  for(auto x: files){
    if(name.data() == x.first){
      return x.second;
    }
  }
  return nullptr;
}

size_t Filesystem::get_file_count() const {
  // TODO: how many files have you got?
  return files.size();
}

size_t Filesystem::in_use() const {
  // TODO: sum up all real file sizes
  size_t result = 0;
  for(auto& x: files){
    result+= x.second->get_size();
  }
  return result;
}

// convenience function so you can see what files are stored
std::string Filesystem::file_overview(bool sort_by_size) {
  std::ostringstream output;
  // this function is not tested, but it may help you when debugging.

  output << "files in filesystem: " << std::endl;

  // for (auto&& entry : this->files) {
  //     // TODO: fix printing name, type and size
  //     output << entry.get_name << std::endl;
  // }
  return std::move(output).str();
}

std::vector<std::shared_ptr<File>>
Filesystem::files_in_size_range(size_t max, size_t min) const {
  std::vector<std::shared_ptr<File>> result;

  for(auto& x: files){
    if(min <= x.second->get_size() && max >= x.second->get_size()){
      result.push_back(x.second);
    }
  }
  
  // TODO: find the matching files and return them
  return result;
}
