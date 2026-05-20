#pragma once

#include <string>
#include <optional>
#include "Models/Helpers/Date.h"

class NewsFunding {
private:
    int news_id=0;
    std::optional<std::string> event_type;
    std::optional<std::string> title;
    std::optional<std::string> description;
    std::optional<Date> event_date;

public:
    NewsFunding() = default;

    NewsFunding(int id,
                const std::optional<std::string>& type = std::nullopt,
                const std::optional<std::string>& t = std::nullopt,
                const std::optional<std::string>& desc = std::nullopt,
                const std::optional<Date>& date = std::nullopt)
        : news_id(id),
          event_type(type),
          title(t),
          description(desc),
          event_date(date) {}

    int getId() const { return news_id; }

    const std::optional<std::string>& getEventType() const { return event_type; }

    const std::optional<std::string>& getTitle() const { return title; }

    const std::optional<std::string>& getDescription() const { return description; }

    const std::optional<Date>& getDate() const { return event_date; }

    void setId(int id) { news_id = id; }

    void setEventType(const std::optional<std::string>& val) { event_type = val; }

    void setTitle(const std::optional<std::string>& val) { title = val; }

    void setDescription(const std::optional<std::string>& val) { description = val; }
    
    void setDate(const std::optional<Date>& val) { event_date = val; }
};