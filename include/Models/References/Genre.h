#pragma once

#include <string>
#include <optional>

class Genre {
private:
    int genre_id=0;
    std::string category;

public:
    Genre() = default;

    Genre(int id, const std::string& cat)
        : genre_id(id), category(cat){}

    // Getters
    int getId() const { return genre_id; }
    const std::string& getCategory() const { return category; }

    // Setter 
    void setId(int id){genre_id = id;}
};