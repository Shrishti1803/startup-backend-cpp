#pragma once

#include <string>
#include <optional>

class Revenue {
private:
    int revenue_id;
    std::optional<int> financial_year;
    std::optional<std::string> revenue_period;
    std::optional<std::string> revenue_source;
    std::optional<std::string> revenue_type;
    double revenue_value;
    std::optional<std::string> currency;

public:
    Revenue() = default;

    Revenue(int id,
            double value,
            const std::optional<int>& year = std::nullopt,
            const std::optional<std::string>& period = std::nullopt,
            const std::optional<std::string>& source = std::nullopt,
            const std::optional<std::string>& type = std::nullopt,
            const std::optional<std::string>& curr = std::nullopt)
        : revenue_id(id),
          financial_year(year),
          revenue_period(period),
          revenue_source(source),
          revenue_type(type),
          revenue_value(value),
          currency(curr) {}

    int getId() const { return revenue_id; }

    double getValue() const { return revenue_value; }

    const std::optional<int>& getYear() const { return financial_year; }

    const std::optional<std::string>& getPeriod() const { return revenue_period; }

    const std::optional<std::string>& getSource() const { return revenue_source; }

    const std::optional<std::string>& getType() const { return revenue_type; }

    const std::optional<std::string>& getCurrency() const { return currency; }
};