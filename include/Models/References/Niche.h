#pragma once

#include <string>

class Niche {

private:

    int niche_id=0;
    std::string niche_type;

public:

    Niche() = default;

    Niche(int id, const std::string& type)
        : niche_id(id),
          niche_type(type) {}

    // ===== Getters =====

    int getId() const {
        return niche_id;
    }

    const std::string& getType() const {
        return niche_type;
    }

    // ===== Setter =====

    void setId(int id){
        niche_id = id;
    }
    void setType(const std::string& type) {
        niche_type = type;
    }
};