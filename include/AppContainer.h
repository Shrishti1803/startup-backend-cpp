#pragma once

#include "Db/DbManager.h"

/* =====================================================
   BRAND REPOSITORIES
===================================================== */

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



/* =====================================================
   BRAND REFERENCES
===================================================== */

#include "Db/Reference/Brand/GenreRepository.h"
#include "Db/Reference/Brand/TargetAudienceRepository.h"
#include "Db/Reference/Brand/BrandGenreRepository.h"
#include "Db/Reference/Brand/BrandTargetAudienceRepository.h"



/* =====================================================
   CREATOR REPOSITORIES
===================================================== */

#include "Db/Creator/CreatorRepository.h"
#include "Db/Creator/CommunityReachesRepository.h"
#include "Db/Creator/ProfileAccountRepository.h"
#include "Db/Creator/HPVAuditRepository.h"
#include "Db/Creator/StrategicNoteRepository.h"
#include "Db/Creator/VideoL10Repository.h"
#include "Db/Creator/VerdictRepository.h"



/* =====================================================
   CREATOR REFERENCES
===================================================== */

#include "Db/Reference/Creator/NicheRepository.h"
#include "Db/Reference/Creator/PlatformRepository.h"

#include "Db/Reference/Creator/CreatorNicheRepository.h"
#include "Db/Reference/Creator/CreatorTargetAudienceRepository.h"



/* =====================================================
   DEAL REPOSITORIES
===================================================== */

#include "Db/Deal/DealRepository.h"
#include "Db/Deal/DealQueryRepository.h"



/* =====================================================
   QUERY REPOSITORIES
===================================================== */

#include "Db/Brand/BrandQueryRepository.h"
#include "Db/Creator/CreatorQueryRepository.h"



/* =====================================================
   AGGREGATES
===================================================== */

#include "Db/Brand/BrandAggregateRepository.h"
#include "Db/Creator/CreatorAggregateRepository.h"



/* =====================================================
   SERVICES
===================================================== */

#include "Service/BrandService.h"
#include "Service/CreatorService.h"
#include "Service/DealService.h"
#include "Service/ReferencesService.h"
#include "Service/AnalyticsService.h"



struct AppContainer
{
    DbManager db;



    /* =====================================================
       BRAND
    ===================================================== */

    BrandRepository brandRepo;
    CompetitorRepository competitorRepo;
    GoalRepository goalRepo;
    HeadRepository headRepo;
    HeadContactRepository headContactRepo;
    InsightsRepository insightsRepo;
    NewsFundingRepository newsFundingRepo;
    PositionRepository positionRepo;
    RevenueRepository revenueRepo;
    BrandStandardRepository standardRepo;
    PastInfluencerCampRepository pastInfluencerCampRepo;



    /* =====================================================
       BRAND REFERENCES
    ===================================================== */

    GenreRepository genreRepo;
    TargetAudienceRepository targetRepo;

    BrandGenreRepository brandGenreRepo;
    BrandTargetAudienceRepository brandTargetRepo;



    /* =====================================================
       CREATOR
    ===================================================== */

    CreatorRepository creatorRepo;
    CommunityReachesRepository communityRepo;
    ProfileAccountRepository profileRepo;
    HPVAuditRepository hpvRepo;
    StrategicNoteRepository strategicRepo;
    VideoL10Repository videoRepo;
    VerdictRepository verdictRepo;



    /* =====================================================
       CREATOR REFERENCES
    ===================================================== */

    NicheRepository nicheRepo;
    PlatformRepository platformRepo;

    CreatorNicheRepository nicheLinkRepo;
    CreatorTargetAudienceRepository creatorTargetAudienceRepo;



    /* =====================================================
       DEAL
    ===================================================== */

    DealRepository dealRepo;
    DealQueryRepository dealQueryRepo;



    /* =====================================================
       QUERY
    ===================================================== */

    BrandQueryRepository brandQueryRepo;
    CreatorQueryRepository creatorQueryRepo;



    /* =====================================================
       AGGREGATES
    ===================================================== */

    BrandAggregateRepository brandAggregateRepo;
    CreatorAggregateRepository creatorAggregateRepo;



    /* =====================================================
       SERVICES
    ===================================================== */

    BrandService brandService;
    CreatorService creatorService;
    DealService dealService;
    ReferencesService referencesService;
    AnalyticsService analyticsService;



    AppContainer()

        :

        /* =====================================================
           BRAND REPOS
        ===================================================== */

        brandRepo(db),
        competitorRepo(db),
        goalRepo(db),
        headRepo(db),
        headContactRepo(db),
        insightsRepo(db),
        newsFundingRepo(db),
        positionRepo(db),
        revenueRepo(db),
        standardRepo(db),
        pastInfluencerCampRepo(db),



        /* =====================================================
           BRAND REFERENCES
        ===================================================== */

        genreRepo(db),
        targetRepo(db),

        brandGenreRepo(db),
        brandTargetRepo(db),



        /* =====================================================
           CREATOR REPOS
        ===================================================== */

        creatorRepo(db),
        communityRepo(db),
        profileRepo(db),
        hpvRepo(db),
        strategicRepo(db),
        videoRepo(db),
        verdictRepo(db),



        /* =====================================================
           CREATOR REFERENCES
        ===================================================== */

        nicheRepo(db),
        platformRepo(db),

        nicheLinkRepo(db),
        creatorTargetAudienceRepo(db),



        /* =====================================================
           DEAL
        ===================================================== */

        dealRepo(),
        dealQueryRepo(),



        /* =====================================================
           QUERY
        ===================================================== */

        brandQueryRepo(db),
        creatorQueryRepo(),



        /* =====================================================
           BRAND AGGREGATE
        ===================================================== */

        brandAggregateRepo(
            brandRepo,
            competitorRepo,
            goalRepo,
            headRepo,
            headContactRepo,
            insightsRepo,
            newsFundingRepo,
            positionRepo,
            revenueRepo,
            standardRepo,
            genreRepo,
            targetRepo,
            brandGenreRepo,
            brandTargetRepo,
            pastInfluencerCampRepo
        ),



        /* =====================================================
           CREATOR AGGREGATE
        ===================================================== */

        creatorAggregateRepo(
            creatorRepo,
            communityRepo,
            nicheLinkRepo,
            profileRepo,
            hpvRepo,
            strategicRepo,
            videoRepo,
            verdictRepo,
            creatorTargetAudienceRepo
        ),



        /* =====================================================
           SERVICES
        ===================================================== */

        brandService(
            db,
            brandAggregateRepo,
            brandRepo,
            brandQueryRepo,
            creatorQueryRepo,
            brandGenreRepo,
            brandTargetRepo
        ),

        creatorService(
            db,
            creatorAggregateRepo,
            creatorRepo,
            creatorQueryRepo
        ),

        dealService(
            db,
            dealRepo,
            dealQueryRepo,
            brandRepo,
            creatorRepo
        ),

        referencesService(
            genreRepo,
            targetRepo,
            nicheRepo,
            platformRepo,
            positionRepo
        ),

         analyticsService(
            db
         )
    {
    }
};