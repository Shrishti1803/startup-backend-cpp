#pragma once

#include <string>
#include <vector>
#include <optional>

#include "HeadContact.h"
#include "Position.h"

class Head {
private:
    int head_id;
    Position position;
    std::string name;
    std::optional<std::string> linkedin_id;
    std::string personal_mail;
    std::string official_mail;

    std::vector<HeadContact> contacts;

public:
    Head() = default;

    Head(int id,
         const Position& pos,
         const std::string& head_name,
         const std::string& personalMail,
         const std::string& officialMail,
         const std::optional<std::string>& linkedin = std::nullopt)
        : head_id(id),
          position(pos),
          name(head_name),
          linkedin_id(linkedin),
          personal_mail(personalMail),
          official_mail(officialMail) {}

    // ===== Getters =====
    int getId() const { return head_id; }

    const Position& getPosition() const { return position; }

    const std::string& getName() const { return name; }

    const std::optional<std::string>& getLinkedIn() const {
        return linkedin_id;
    }

    const std::string& getPersonalMail() const {
        return personal_mail;
    }

    const std::string& getOfficialMail() const {
        return official_mail;
    }

    const std::vector<HeadContact>& getContacts() const {
        return contacts;
    }

    // ===== Modifiers =====
    void setPosition(const Position& pos) {
        position = pos;
    }

    void setLinkedIn(const std::optional<std::string>& linkedin) {
        linkedin_id = linkedin;
    }

    void addContact(const HeadContact& contact) {
        contacts.push_back(contact);
    }
};