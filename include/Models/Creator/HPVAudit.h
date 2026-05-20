#pragma once

#include <string>
#include <optional>

class HPVAudit {

private:

    int hpv_id=0;
    std::optional<std::string> topic;
    std::optional<std::string> spike_rate;

public:

    HPVAudit() = default;

    HPVAudit(
        int id,
        const std::optional<std::string>& t = std::nullopt,
        const std::optional<std::string>& rate = std::nullopt)
        : hpv_id(id),
          topic(t),
          spike_rate(rate) {}

    int getId() const { return hpv_id; }

    const std::optional<std::string>& getTopic() const {
        return topic;
    }

    const std::optional<std::string>& getSpikeRate() const {
        return spike_rate;
    }
    void setId(int id)
    {
        hpv_id = id;
    }

    void setTopic(const std::optional<std::string>& t) {
        topic = t;
    }

    void setSpikeRate(const std::optional<std::string>& rate) {
        spike_rate = rate;
    }
};