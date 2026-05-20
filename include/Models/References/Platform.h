#pragma once

#include <string>

class Platform {

private:

    int platform_id=0;
    std::string platform_name;

public:

    Platform() = default;

    Platform(int id, const std::string& name)
        : platform_id(id),
          platform_name(name) {}

    // ===== Getters =====

    int getId() const {
        return platform_id;
    }

    const std::string& getName() const {
        return platform_name;
    }

    // ===== Setter =====

    void setName(const std::string& name) {
        platform_name = name;
    }
};