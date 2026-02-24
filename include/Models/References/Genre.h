#pragma once

#include <string>
#include <optional>

class Genre {
private:
    int genre_id;
    std::string category;
    std::optional<std::string> description;

public:
    Genre() = default;

    Genre(int id, const std::string& cat,
          const std::optional<std::string>& desc = std::nullopt)
        : genre_id(id), category(cat), description(desc) {}

    // Getters
    int getId() const { return genre_id; }
    const std::string& getCategory() const { return category; }
    const std::optional<std::string>& getDescription() const { return description; }

    // Setter (if needed)
    void setDescription(const std::optional<std::string>& desc) {
        description = desc;
    }
};