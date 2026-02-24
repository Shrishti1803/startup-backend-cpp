#pragma once

#include <string>
#include <optional>

class Competitor {
private:
    int competitor_id;
    std::string name;
    std::optional<std::string> type;   // descriptive type

public:
    Competitor() = default;

    Competitor(int id,
               const std::string& comp_name,
               const std::optional<std::string>& comp_type = std::nullopt)
        : competitor_id(id),
          name(comp_name),
          type(comp_type) {}

    // Getters
    int getId() const { return competitor_id; }

    const std::string& getName() const { return name; }

    const std::optional<std::string>& getType() const {
        return type;
    }

    // Setters
    void setName(const std::string& comp_name) {
        name = comp_name;
    }

    void setType(const std::optional<std::string>& comp_type) {
        type = comp_type;
    }
};