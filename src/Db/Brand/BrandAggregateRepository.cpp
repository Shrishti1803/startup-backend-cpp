#include "BrandAggregateRepository.h"

BrandAggregateRepository::BrandAggregateRepository(
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
)
: brandRepo(brandRepo),
  companyTypeRepo(companyTypeRepo),
  competitorRepo(competitorRepo),
  goalRepo(goalRepo),
  headRepo(headRepo),
  headContactRepo(headContactRepo),
  insightsRepo(insightsRepo),
  newsFundingRepo(newsFundingRepo),
  positionRepo(positionRepo),
  revenueRepo(revenueRepo),
  standardRepo(standardRepo)
{
}


int BrandAggregateRepository::create(const BrandAggregate& data)
{
    // ===== Brand =====
    int brandId = brandRepo.insertBrand(data.brand.getName());

    // ===== Company Type =====
    companyTypeRepo.insert(
        brandId,
        data.companyType.getType()
    );

    // ===== Standard =====
    standardRepo.insert(
        brandId,
        data.standard.getPosition(),
        data.standard.getIdentity(),
        data.standard.getCSR(),
        data.standard.getQuality(),
        data.standard.getPerception()
    );

    // ===== Insights =====
    insightsRepo.insert(
        brandId,
        data.insights.getPitchAngle(),
        data.insights.getLeverage(),
        data.insights.getGap(),
        data.insights.getHook()
    );

    // ===== Revenue =====
    revenueRepo.insert(
        brandId,
        data.revenue.getYear().value_or(0),
        data.revenue.getPeriod(),
        data.revenue.getSource(),
        data.revenue.getType(),
        data.revenue.getValue(),
        data.revenue.getCurrency()
    );

    // ===== Competitors =====
    for(const auto& competitor : data.competitors)
    {
        competitorRepo.insert(
            brandId,
            competitor.getName(),
            competitor.getType()
        );
    }

    // ===== Goals =====
    for(const auto& goal : data.goals)
    {
        goalRepo.insert(
            brandId,
            goal.getDescription()
        );
    }

    // ===== News Funding =====
    for(const auto& news : data.newsFundings)
    {
        newsFundingRepo.insert(
            brandId,
            news.getEventType(),
            news.getTitle(),
            news.getDescription(),
            news.getDate()
        );
    }

    // ===== Heads =====
    for(const auto& head : data.heads)
    {
        int headId = headRepo.insert(
            brandId,
            head.getPosition().getId(),
            head.getName(),
            head.getLinkedIn(),
            head.getPersonalMail(),
            head.getOfficialMail()
        );

        for(const auto& contact : head.getContacts())
        {
            headContactRepo.insert(
                headId,
                contact.getType(),
                contact.getValue()
            );
        }
    }

    return brandId;
}