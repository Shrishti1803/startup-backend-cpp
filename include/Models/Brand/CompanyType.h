#pragma once

#include <string>

class CompanyType {
private:
    int comp_type_id;
    std::string type;   // "Pvt" / "Public"

public:
    CompanyType() = default;

    CompanyType(int id, const std::string& t)
        : comp_type_id(id), type(t) {}

    int getId() const { return comp_type_id; }

    const std::string& getType() const { return type; }

    void setType(const std::string& t) {
        type = t;
    }
};