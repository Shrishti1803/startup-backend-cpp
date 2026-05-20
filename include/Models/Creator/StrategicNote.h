#pragma once

#include <string>
#include <optional>

class StrategicNote {

private:

    int strategic_id=0;
    std::optional<std::string> description;
    std::string author_name;

public:

    StrategicNote() = default;

    StrategicNote(
        int id,
        const std::string& author,
        const std::optional<std::string>& desc = std::nullopt)
        : strategic_id(id),
          description(desc),
          author_name(author) {}

    int getId() const { return strategic_id; }

    const std::optional<std::string>& getDescription() const {
        return description;
    }

    const std::string& getAuthor() const {
        return author_name;
    }

    void setId(int id)
    {
        strategic_id = id;
    }
    void setDescription(const std::optional<std::string>& desc) {
        description = desc;
    }
    void setAuthor(
        const std::string& author
    )
    {
        author_name = author;
    }
};