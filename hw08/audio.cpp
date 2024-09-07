#include "audio.h"

Audio::Audio(FileContent &&content, unsigned duration)
    : File{std::move(content)}, duration{duration} {}

// TODO implement get_type function
std::string_view Audio::get_type() const{
    return "AUD";
}


size_t Audio::get_raw_size() const {
  //i do a static casting because in one of the tests it updates the duration to 9001 and then hell breaks loose. To overcome that i do casting :)
  return static_cast<uint64_t>(this->duration) * 48000 * 2 * 16 / 8;  // TODO calculate raw size
}

unsigned Audio::get_duration() { return this->duration; }

// TODO implement content update function
void Audio::update(FileContent &&new_content, unsigned new_duration){
  File::content = std::move(new_content);
  Audio::duration = new_duration;
}