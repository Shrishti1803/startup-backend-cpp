/*
 
  Represents the core Brand entity in the system.
  This class models the complete in-memory structure of a brand and
  maintains relationships with all associated brand components.
 
  Relationships modeled in this class:
   - 1:M  → Goals, Heads, Competitors, Revenues, News/Funding updates
   - 1:1  → CompanyType, BrandStandard, Insights
   - M:M  → Genres and Target Audience
 
  The Brand object is used as the root entity for constructing a
  BrandAggregate, allowing the system to create, update, and retrieve
  complete brand data across multiple database tables in a coordinated way.
 
  Note:
  This class only represents the domain structure and contains no
  database logic. Persistence is handled by repository classes.
 */
#pragma once

#include <string>
#include <vector>
#include <optional>

// Child models
#include "Goal.h"
#include "Competitor.h"
#include "Revenue.h"
#include "BrandHead.h"
#include "NewsFunding.h"
#include "Standard.h"
#include "Insights.h"

// Reference models
#include "../References/Genre.h"
#include "../References/TargetAudience.h"

class Brand {
private:
    // ===== Core Fields =====
    int brand_id=0;
    std::string brand_name;
    std::optional<std::string> psychographics;
    std::optional<std::string> genre_description;
    std::optional<std::string> company_type;

    // 1:M RELATIONS : 
    std::vector<Goal> goals;
    std::vector<BrandHead> heads;
    std::vector<Competitor> competitors;
    std::vector<Revenue> revenues;
    std::vector<NewsFunding> news_funding;

    //1:1 RELATIONS 
    std::optional<Standard> brand_standard;
    std::optional<Insights> insights;

    //M:M RELATIONS 
    std::vector<Genre> genres;
    std::vector<TargetAudience> target_audiences;



    public:
        Brand() = default;

        Brand(int id, const std::string& name,
            const std::optional<std::string>& psych = std::nullopt,
            const std::optional<std::string>& gen_desc = std::nullopt,
            const std::optional<std::string>& comp_type = std::nullopt)
            : brand_id(id), brand_name(name), psychographics(psych), genre_description(gen_desc), company_type(comp_type){}

        // ===== Identity Getters =====
        int getId() const { return brand_id; }

        const std::string& getName() const { return brand_name; }

        const std::optional<std::string>& getPsychographics() const {
            return psychographics;
        }

        std::optional<std::string> getGenreDescription() const{
            return genre_description;
        }
        std::optional<std::string> getCompanyType() const{
            return company_type;
        }

        void setName(const std::string& name) {
            brand_name = name;
        }

        void setPsychographics(const std::optional<std::string>& psych) {
            psychographics = psych;
        }

        void setGenreDescription(const std::optional<std::string>& gen_desc){
            genre_description = gen_desc;
        }

        void setCompanyType(const std::optional<std::string>& comp_type){
            company_type = comp_type;
        }

        // ===== Adders (1:M) =====
        void addGoal(const Goal& goal) {
            goals.push_back(goal);
        }

        void addHead(const BrandHead& head) {
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



        void setBrandStandard(const Standard& standard) {
            brand_standard = standard;
        }

        void setInsights(const Insights& ins) {
            insights = ins;
        }

        // ===== Getters (Read-only access) =====
        const std::vector<Goal>& getGoals() const { return goals; }
        const std::vector<BrandHead>& getHeads() const { return heads; }
        const std::vector<Competitor>& getCompetitors() const { return competitors; }
        const std::vector<Revenue>& getRevenues() const { return revenues; }
        const std::vector<NewsFunding>& getNewsFunding() const { return news_funding; }

        const std::vector<Genre>& getGenres() const { return genres; }
        const std::vector<TargetAudience>& getTargetAudiences() const { return target_audiences; }

        const std::optional<Standard>& getBrandStandard() const { return brand_standard; }
        const std::optional<Insights>& getInsights() const { return insights; }


};