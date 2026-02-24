#pragma once

#include <string>
#include <vector>
#include <optional>

// Child models
#include "Goal.h"
#include "Competitor.h"
#include "Revenue.h"
#include "Head.h"
#include "NewsFunding.h"
#include "Standard.h"
#include "CompanyType.h"
#include "Insights.h"

// Reference models
#include "../References/Genre.h"
#include "../References/TargetAudience.h"

class Brand {
private:
    // ===== Core Fields =====
    int brand_id;
    std::string brand_name;

    // 1:M RELATIONS : 
    std::vector<Goal> goals;
    std::vector<Head> heads;
    std::vector<Competitor> competitors;
    std::vector<Revenue> revenues;
    std::vector<NewsFunding> news_funding;

    //1:1 RELATIONS 
    std::optional<CompanyType> company_type;
    std::optional<BrandStandard> brand_standard;
    std::optional<Insights> insights;

    //M:M RELATIONS 
    std::vector<Genre> genres;
    std::vector<TargetAudience> target_audiences;



    public:
        Brand() = default;

        Brand(int id, const std::string& name)
            : brand_id(id), brand_name(name) {}

        // ===== Identity Getters =====
        int getId() const { return brand_id; }

        const std::string& getName() const { return brand_name; }

        void setName(const std::string& name) {
            brand_name = name;
        }

        // ===== Adders (1:M) =====
        void addGoal(const Goal& goal) {
            goals.push_back(goal);
        }

        void addHead(const Head& head) {
            heads.push_back(head);
        }

        void addCompetitor(const Competitor& comp) {
            competitors.push_back(comp);
        }

        void addRevenue(const Revenue& revenue) {
            revenues.push_back(revenue);
        }

        void addNewsFunding(const NewsFunding& news) {
            news_funding.push_back(news);
        }

        void addGenre(const Genre& genre) {
            genres.push_back(genre);
        }

        void addTargetAudience(const TargetAudience& audience) {
            target_audiences.push_back(audience);
        }

        // ===== Setters (1:1) =====
        void setCompanyType(const CompanyType& type) {
            company_type = type;
        }

        void setBrandStandard(const BrandStandard& standard) {
            brand_standard = standard;
        }

        void setInsights(const Insights& ins) {
            insights = ins;
        }

        // ===== Getters (Read-only access) =====
        const std::vector<Goal>& getGoals() const { return goals; }
        const std::vector<Head>& getHeads() const { return heads; }
        const std::vector<Competitor>& getCompetitors() const { return competitors; }
        const std::vector<Revenue>& getRevenues() const { return revenues; }
        const std::vector<NewsFunding>& getNewsFunding() const { return news_funding; }

        const std::vector<Genre>& getGenres() const { return genres; }
        const std::vector<TargetAudience>& getTargetAudiences() const { return target_audiences; }

        const std::optional<CompanyType>& getCompanyType() const { return company_type; }
        const std::optional<BrandStandard>& getBrandStandard() const { return brand_standard; }
        const std::optional<Insights>& getInsights() const { return insights; }


};