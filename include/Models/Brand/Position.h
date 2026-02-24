#pragma once

#include <string>

class Position {
private:
    int position_id;
    std::string position_name;

public:
    Position() = default;

    Position(int id, const std::string& name)
        : position_id(id), position_name(name) {}

    // Getters
    int getId() const { return position_id; }

    const std::string& getName() const {
        return position_name;
    }

    // Setter
    void setName(const std::string& name) {
        position_name = name;
    }
};