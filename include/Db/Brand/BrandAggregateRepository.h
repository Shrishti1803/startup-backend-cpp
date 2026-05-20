//This repo serves as the Aggregate of all the BRAND related files
//Will make the insert/update/read/delete(soft) easy as it is the aggregate of all the Brand aspect files
#pragma once

#include <vector>
#include <optional>

#include <mysql_connection.h>
#include <unordered_set>
// Models
#include "Models/Brand/Brand.h"
#include "Models/Brand/Competitor.h"
#include "Models/Brand/Goal.h"
#include "Models/Brand/BrandHead.h"
#include "Models/Brand/Insights.h"
#include "Models/Brand/NewsFunding.h"
#include "Models/Brand/Position.h"
#include "Models/Brand/Revenue.h"
#include "Models/Brand/Standard.h"

// Repositories
#include "Db/Brand/BrandRepository.h"
#include "Db/Brand/CompetitorRepository.h"
#include "Db/Brand/GoalRepository.h"
#include "Db/Brand/HeadRepository.h"
#include "Db/Brand/HeadContactRepository.h"
#include "Db/Brand/InsightsRepository.h"
#include "Db/Brand/NewsFundingRepository.h"
#include "Db/Brand/PositionRepository.h"
#include "Db/Brand/RevenueRepository.h"
#include "Db/Brand/StandardRepository.h"
#include "Db/Brand/PastInfluencerCampRepository.h"
#include "Models/Brand/PastInfluencerCamp.h"

//M:M repos
#include "Db/Reference/Brand/BrandGenreRepository.h"
#include "Db/Reference/Brand/GenreRepository.h"
#include "Db/Reference/Brand/BrandTargetAudienceRepository.h"
#include "Db/Reference/Brand/TargetAudienceRepository.h"



struct BrandAggregate {

    // Core
    Brand brand;

    // Classification (UI early fields)
    std::vector<Genre> genres;
    std::vector<NewsFunding> newsFundings;
    std::vector<TargetAudience> targetAudience;

    // Campaigns
    std::vector<PastInfluencerCamp> pastInfluencerCamps;

    // Market context
    std::vector<Competitor> competitors;
    std::vector<Goal> goals;

    // Financial
    std::vector<Revenue> revenue;

    // Positioning
    std::optional<Standard> standard;

    // People
    Position position;
    std::vector<BrandHead> heads;

    std::optional<Insights> insights;
    std::unordered_set<std::string> clearFields;
};



class BrandAggregateRepository {

private:

    BrandRepository& brandRepo;
    CompetitorRepository& competitorRepo;
    GoalRepository& goalRepo;
    HeadRepository& headRepo;
    HeadContactRepository& headContactRepo;
    InsightsRepository& insightsRepo;
    NewsFundingRepository& newsFundingRepo;
    PositionRepository& positionRepo;
    RevenueRepository& revenueRepo;
    BrandStandardRepository& standardRepo;
    GenreRepository& genreRepo;
    TargetAudienceRepository& targetRepo;
    BrandGenreRepository& brandGenreRepo;
    BrandTargetAudienceRepository& brandTargetRepo;
    PastInfluencerCampRepository& pastInfluencerRepo;


public:

    BrandAggregateRepository(
        BrandRepository& brandRepo,
        CompetitorRepository& competitorRepo,
        GoalRepository& goalRepo,
        HeadRepository& headRepo,
        HeadContactRepository& headContactRepo,
        InsightsRepository& insightsRepo,
        NewsFundingRepository& newsFundingRepo,
        PositionRepository& positionRepo,
        RevenueRepository& revenueRepo,
        BrandStandardRepository& standardRepo,
        GenreRepository& genRepo,
        TargetAudienceRepository& targetRepo,
        BrandGenreRepository& brandGenreRepo,
        BrandTargetAudienceRepository& brandTargetRepo,
        PastInfluencerCampRepository& pastInfluencerRepo
    );



    // CREATE
    int create(
        sql::Connection* conn,
        int userId,
        const BrandAggregate& data
    );


    // UPDATE
    void update(
        sql::Connection* conn,
        int userId,
        int brandId,
        const BrandAggregate& data
    );


    // READ
    BrandAggregate getById(int brandId);


    // DELETE
    void softDelete(
        sql::Connection* conn,
        int userId,
        int brandId
    );
    void deletePartial(
        sql::Connection* conn,
        int userId,
        int brandId,
        const BrandAggregate& data
    );

};