#pragma once

#include <string>
#include <optional>

class Goal {
private:
    int goal_id = 0;
    std::optional<std::string> description;

public:
    Goal() = default;

    Goal(int id,
         const std::optional<std::string>& desc = std::nullopt)
        : goal_id(id), description(desc) {}

    // Getters
    int getId() const { return goal_id; }

    const std::optional<std::string>& getDescription() const {
        return description;
    }

    // Setter
    void setId(int i){goal_id = i;}

    void setDescription(const std::optional<std::string>& desc) {
        description = desc;
    }
};