//This repo serves as the Aggregate of all the BRAND related files
//Will make the insert/update/read/delete(soft) easy as it is the aggregate of all the Brand aspect files
#pragma once

#include <vector>
#include <optional>

#include <mysql_connection.h>

// Models
#include "Models/Brand/Brand.h"
#include "Models/Brand/CompanyType.h"
#include "Models/Brand/Competitor.h"
#include "Models/Brand/Goal.h"
#include "Models/Brand/Head.h"
#include "Models/Brand/Insights.h"
#include "Models/Brand/NewsFunding.h"
#include "Models/Brand/Position.h"
#include "Models/Brand/Revenue.h"
#include "Models/Brand/Standard.h"

// Repositories
#include "Db/Brand/BrandRepository.h"
#include "Db/Brand/CompanyTypeRepository.h"
#include "Db/Brand/CompetitorRepository.h"
#include "Db/Brand/GoalRepository.h"
#include "Db/Brand/HeadRepository.h"
#include "Db/Brand/HeadContactRepository.h"
#include "Db/Brand/InsightsRepository.h"
#include "Db/Brand/NewsFundingRepository.h"
#include "Db/Brand/PositionRepository.h"
#include "Db/Brand/RevenueRepository.h"
#include "Db/Brand/StandardRepository.h"



struct BrandAggregate {

    Brand brand;
    CompanyType companyType;
    Revenue revenue;
    Position position;
    Standard standard;
    Insights insights;

    std::vector<Head> heads;
    std::vector<Competitor> competitors;
    std::vector<Goal> goals;
    std::vector<NewsFunding> newsFundings;

};



class BrandAggregateRepository {

private:

    BrandRepository& brandRepo;
    CompanyTypeRepository& companyTypeRepo;
    CompetitorRepository& competitorRepo;
    GoalRepository& goalRepo;
    HeadRepository& headRepo;
    HeadContactRepository& headContactRepo;
    InsightsRepository& insightsRepo;
    NewsFundingRepository& newsFundingRepo;
    PositionRepository& positionRepo;
    RevenueRepository& revenueRepo;
    BrandStandardRepository& standardRepo;

public:

    BrandAggregateRepository(
        BrandRepository& brandRepo,
        CompanyTypeRepository& companyTypeRepo,
        CompetitorRepository& competitorRepo,
        GoalRepository& goalRepo,
        HeadRepository& headRepo,
        HeadContactRepository& headContactRepo,
        InsightsRepository& insightsRepo,
        NewsFundingRepository& newsFundingRepo,
        PositionRepository& positionRepo,
        RevenueRepository& revenueRepo,
        BrandStandardRepository& standardRepo
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

};