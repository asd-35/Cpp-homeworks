#include "contact_list.h"

#include <algorithm>
#include <numeric>
#include <sstream>

// TODO create implementation here!

bool contact_list::add(storage& contacts, std::string_view name, number_t number){
    if(name.empty()){
        return false;
    }
    
    if(get_number_by_name(contacts,name) == number){
        return false;
    }
    if(get_name_by_number(contacts,number) == name){
        return false;
    }
    
    contacts.names.push_back(name.data());
    contacts.numbers.push_back(number);

    return true;
};

size_t contact_list::size(const storage& contacts){
    return contacts.names.size();
};

contact_list::number_t contact_list::get_number_by_name(storage& contacts, std::string_view name){
    contact_list::number_t result{-1};

    for (size_t i = 0; i < size(contacts); ++i)
    {
        if(contacts.names.at(i) == name.data()){
            result = contacts.numbers.at(i);
            i = contacts.names.size();
        }
    }
    return result;
};

std::string contact_list::to_string(const storage& contacts){
    std::string result{""};
    
    for (size_t i = 0; i < size(contacts); ++i)
    {
        result += contacts.names.at(i) + " - " + std::to_string(contacts.numbers.at(i)) + "\n";
    }
    return result;
};

bool contact_list::remove(storage& contacts, std::string_view name){
    if(!size(contacts)){
        return false;
    }else{
        for (size_t i = 0; i < size(contacts); ++i)
        {
            if(contacts.names.at(i) == name.data()){
                contacts.names.erase(contacts.names.begin()+i);
                contacts.numbers.erase(contacts.numbers.begin()+i);
                i = size(contacts);
            }
        }
        
        return true;
    }
};

void contact_list::sort(storage& contacts){
    std::vector<number_t> vec_num{contacts.numbers};
    std::vector<std::string> vec_names{contacts.names};
    std::sort(vec_names.begin(),vec_names.end());

    for (size_t i = 0; i < size(contacts); ++i)
    {
        contact_list::number_t num{(get_number_by_name(contacts,vec_names.at(i)))};
        vec_num.at(i) = num;
    }
    contacts.names.swap(vec_names);
    contacts.numbers.swap(vec_num);
};

std::string contact_list::get_name_by_number(storage& contacts, number_t number){
    std::string result{""};
    for (size_t i = 0; i < size(contacts); ++i)
    {
        if(contacts.numbers.at(i) == number){
            result = contacts.names.at(i);
            i = contacts.names.size();
        }
    }
    return result;
};