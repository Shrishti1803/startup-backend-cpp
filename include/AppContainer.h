#pragma once

#include "Db/DbManager.h"

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

// Aggregate
#include "Db/Brand/BrandAggregateRepository.h"

// Services
#include "Service/BrandService.h"

struct AppContainer {

    DbManager db;

    // repositories
    BrandRepository brandRepo;
    CompanyTypeRepository companyTypeRepo;
    CompetitorRepository competitorRepo;
    GoalRepository goalRepo;
    HeadRepository headRepo;
    HeadContactRepository headContactRepo;
    InsightsRepository insightsRepo;
    NewsFundingRepository newsFundingRepo;
    PositionRepository positionRepo;
    RevenueRepository revenueRepo;
    BrandStandardRepository standardRepo;

    // aggregate
    BrandAggregateRepository brandAggregateRepo;

    // services
    BrandService brandService;

    AppContainer()
        : brandRepo(db),
          companyTypeRepo(db),
          competitorRepo(db),
          goalRepo(db),
          headRepo(db),
          headContactRepo(db),
          insightsRepo(db),
          newsFundingRepo(db),
          positionRepo(db),
          revenueRepo(db),
          standardRepo(db),

          brandAggregateRepo(
              brandRepo,
              companyTypeRepo,
              competitorRepo,
              goalRepo,
              headRepo,
              headContactRepo,
              insightsRepo,
              newsFundingRepo,
              positionRepo,
              revenueRepo,
              standardRepo
          ),

          brandService(
              db,
              brandAggregateRepo,
              brandRepo
          )
    {}
};