#pragma once

#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>

class Date {
private:
    int year;
    int month;
    int day;

    void validate() const {
        if (year < 0)
            throw std::invalid_argument("Invalid year");

        if (month < 1 || month > 12)
            throw std::invalid_argument("Invalid month");

        if (day < 1 || day > 31)
            throw std::invalid_argument("Invalid day");
    }

public:
    Date() = default;

    Date(int y, int m, int d)
        : year(y), month(m), day(d) {
        validate();
    }

    int getYear() const { return year; }
    int getMonth() const { return month; }
    int getDay() const { return day; }

        // ===== Convert to "YYYY-MM-DD" =====
    std::string toString() const {
        std::ostringstream oss;
        oss << std::setw(4) << std::setfill('0') << year << "-"
            << std::setw(2) << std::setfill('0') << month << "-"
            << std::setw(2) << std::setfill('0') << day;
        return oss.str();
    }

        // ===== Parse from "YYYY-MM-DD" =====
    static Date fromString(const std::string& dateStr) {
        if (dateStr.size() != 10 || dateStr[4] != '-' || dateStr[7] != '-') {
            throw std::invalid_argument("Invalid date format. Expected YYYY-MM-DD");
        }

        int y = std::stoi(dateStr.substr(0, 4));
        int m = std::stoi(dateStr.substr(5, 2));
        int d = std::stoi(dateStr.substr(8, 2));

        return Date(y, m, d);  // validation happens in constructor
    }
};