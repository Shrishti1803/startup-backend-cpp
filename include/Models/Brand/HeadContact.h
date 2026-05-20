#pragma once

#include <string>
#include <optional>

class HeadContact {
private:
    int contact_id = 0;
    std::optional<std::string> contact_type;   // e.g., WhatsApp / Phone
    std::string contact_value;

public:
    HeadContact() = default;

    HeadContact(int id,
                const std::string& value,
                const std::optional<std::string>& type = std::nullopt)
        : contact_id(id),
          contact_type(type),
          contact_value(value) {}

    // ===== Getters =====
    int getId() const { return contact_id; }

    const std::optional<std::string>& getType() const {
        return contact_type;
    }

    const std::string& getValue() const {
        return contact_value;
    }

    // ===== Setters =====
    void setId(int id) { contact_id = id; }

    void setType(const std::optional<std::string>& type) {
        contact_type = type;
    }

    void setValue(const std::string& value) {
        contact_value = value;
    }

    // 🔥 Convenience overload (important for UI usage)
    void setType(const std::string& type) {
        contact_type = type;
    }
};