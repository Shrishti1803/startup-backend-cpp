#pragma once

#include <string>
#include <optional>
#include "Helpers/Date.h"

class NewsFunding {
private:
    int news_id;
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
};