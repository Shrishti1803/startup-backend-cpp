#pragma once

#include <string>
#include <optional>

class CommunityReaches {
private:
    int comm_id =0 ;
    std::optional<std::string> sentiment;
    std::optional<std::string> reply_rate;

public:

    CommunityReaches() = default;

    CommunityReaches(
        int id,
        const std::optional<std::string>& sent = std::nullopt,
        const std::optional<std::string>& reply = std::nullopt)
        : comm_id(id),
          sentiment(sent),
          reply_rate(reply) {}

    int getId() const { return comm_id; }

    const std::optional<std::string>& getSentiment() const {
        return sentiment;
    }

    const std::optional<std::string>& getReplyRate() const {
        return reply_rate;
    }

    void setId(int id) {
        comm_id = id;
    }
    
    void setSentiment(const std::optional<std::string>& sent) {
        sentiment = sent;
    }

    void setReplyRate(const std::optional<std::string>& reply) {
        reply_rate = reply;
    }
};