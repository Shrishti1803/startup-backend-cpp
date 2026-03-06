#pragma once

#include <vector>

// Models
#include "../../Models/Brand/Brand.h"
#include "../../Models/Brand/CompanyType.h"
#include "../../Models/Brand/Competitor.h"
#include "../../Models/Brand/Goal.h"
#include "../../Models/Brand/Head.h"
#include "../../Models/Brand/Insights.h"
#include "../../Models/Brand/NewsFunding.h"
#include "../../Models/Brand/Position.h"
#include "../../Models/Brand/Revenue.h"
#include "../../Models/Brand/Standard.h"

// Repositories
#include "BrandRepository.h"
#include "CompanyTypeRepository.h"
#include "CompetitorRepository.h"
#include "GoalRepository.h"
#include "HeadRepository.h"
#include "HeadContactRepository.h"
#include "InsightsRepository.h"
#include "NewsFundingRepository.h"
#include "PositionRepository.h"
#include "RevenueRepository.h"
#include "StandardRepository.h"



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

    std::vector<int> genreIds;
    std::vector<int> targetAudienceIds;
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

    int create(const BrandAggregate& data);
};