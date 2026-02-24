#pragma once

#include <string>

class TargetAudience {
private:
    int aud_id;
    std::string aud_name;

public:
    TargetAudience() = default;

    TargetAudience(int id, const std::string& name)
        : aud_id(id), aud_name(name) {}

    // Getters
    int getId() const { return aud_id; }
    const std::string& getName() const { return aud_name; }

    // Setter
    void setName(const std::string& name) {
        aud_name = name;
    }
};