#pragma once

#include <string>
#include <optional>

class PastInfluencerCamp {
private:
    int p_inf_id=0;
    int brand_id;

    std::optional<std::string> ambassadors;
    std::optional<std::string> campaigns;
    std::optional<std::string> strategy;

    bool is_deleted;

public:
    PastInfluencerCamp() = default;

    PastInfluencerCamp(
        int id,
        int brandId,
        const std::optional<std::string>& amb = std::nullopt,
        const std::optional<std::string>& camp = std::nullopt,
        const std::optional<std::string>& strat = std::nullopt,
        bool deleted = false
    )
        : p_inf_id(id),
          brand_id(brandId),
          ambassadors(amb),
          campaigns(camp),
          strategy(strat),
          is_deleted(deleted) {}

    // Getters
    int getId() const { return p_inf_id; }

    int getBrandId() const { return brand_id; }

    const std::optional<std::string>& getAmbassadors() const {
        return ambassadors;
    }

    const std::optional<std::string>& getCampaigns() const {
        return campaigns;
    }

    const std::optional<std::string>& getStrategy() const {
        return strategy;
    }

    bool getIsDeleted() const { return is_deleted; }

    // Setters
    void setId(int id) {p_inf_id = id;}

    void setBrandId(int id){brand_id = id;}

    void setAmbassadors(const std::optional<std::string>& amb) {
        ambassadors = amb;
    }

    void setCampaigns(const std::optional<std::string>& camp) {
        campaigns = camp;
    }

    void setStrategy(const std::optional<std::string>& strat) {
        strategy = strat;
    }

    void setIsDeleted(bool deleted) {
        is_deleted = deleted;
    }
};