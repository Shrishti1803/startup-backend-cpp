// BrandAggregateRepository : AKA the ORCHESTRA OF THE BRAND RELATED TABLES
// Responsible for coordinating multi-table operations related to the Brand domain.
// Individual repositories handle table-level persistence, while this class
// orchestrates operations that span across multiple entities.

#include "Db/Brand/BrandAggregateRepository.h"
#include "Utils/ActivityDetails.h"

/*
Constructor 
Initializes the aggregate repo with references to all 
The separate repos responsible for all the different brand-related tables
Allows the Aggregate repo to coordinate multi table operations 
*/
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

// CREATE OPERATION
// Creates a complete Brand record across multiple related tables.
//
// This function acts as an orchestration layer that inserts data into:
//  - BRAND
//  - COMPANY_TYPE
//  - BRAND_STANDARD
//  - INSIGHTS
//  - REVENUE
//  - COMPETITORS
//  - GOALS
//  - NEWS_FUNDING
//  - HEADS
//  - HEAD_CONTACTS
//
// Each step also records an entry in the Activity Log for auditing purposes.
// The transaction boundary is controlled by the Service layer.
int BrandAggregateRepository::create(
    sql::Connection* conn,
    int userId,
    const BrandAggregate& data
)
{
    // ===== Brand =====
    int brandId = brandRepo.insertBrand(data.brand.getName());

    ActivityDetails::logActivity(
        conn, userId, "CREATE", "BRAND", brandId, "", data.brand.getName()
    );


    // ===== Company Type =====
    companyTypeRepo.insert(
        brandId,
        data.companyType.getType()
    );

    ActivityDetails::logActivity(
        conn, userId, "CREATE", "COMPANY_TYPE", brandId, "", data.companyType.getType()
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

    ActivityDetails::logActivity(
        conn, userId, "CREATE", "STANDARD", brandId, "", "created"
    );


    // ===== Insights =====
    insightsRepo.insert(
        brandId,
        data.insights.getPitchAngle(),
        data.insights.getLeverage(),
        data.insights.getGap(),
        data.insights.getHook()
    );

    ActivityDetails::logActivity(
        conn, userId, "CREATE", "INSIGHTS", brandId, "", "created"
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

    ActivityDetails::logActivity(
        conn, userId, "CREATE", "REVENUE", brandId, "", "created"
    );


    // ===== Competitors =====
    for(const auto& competitor : data.competitors)
    {
        int competitorId = competitorRepo.insert(
            brandId,
            competitor.getName(),
            competitor.getType()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE", "COMPETITOR", competitorId, "", competitor.getName()
        );
    }


    // ===== Goals =====
    for(const auto& goal : data.goals)
    {
        int goalId = goalRepo.insert(
            brandId,
            goal.getDescription()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE", "GOAL", goalId, "", goal.getDescription().value_or("")
        );
    }


    // ===== News Funding =====
    for(const auto& news : data.newsFundings)
    {
        int newsId = newsFundingRepo.insert(
            brandId,
            news.getEventType(),
            news.getTitle(),
            news.getDescription(),
            news.getDate()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE", "NEWS_FUNDING", newsId, "", news.getTitle().value_or("")
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

        ActivityDetails::logActivity(
            conn, userId, "CREATE", "HEAD", headId, "", head.getName()
        );


        for(const auto& contact : head.getContacts())
        {
            int contactId = headContactRepo.insert(
                headId,
                contact.getType(),
                contact.getValue()
            );

            ActivityDetails::logActivity(
                conn, userId, "CREATE", "HEAD_CONTACT", contactId, "", contact.getValue()
            );
        }
    }

    return brandId;
}


// UPDATE OPERATION
// Updates existing brand-related data using PATCH-style updates.
//
// Only the fields provided in the BrandAggregate object are updated.
// Each modification is logged in the Activity Log for auditing.
void BrandAggregateRepository::update(
    sql::Connection* conn,
    int userId,
    int brandId,
    const BrandAggregate& data
)
{
    // ===== Brand =====
    brandRepo.updateName(
        brandId,
        std::optional<std::string>(data.brand.getName())
    );

    ActivityDetails::logActivity(
        conn, userId, "UPDATE", "BRAND", brandId, "", data.brand.getName()
    );


    // ===== Company Type =====
    companyTypeRepo.update(
        brandId,
        std::optional<std::string>(data.companyType.getType())
    );

    ActivityDetails::logActivity(
        conn, userId, "UPDATE", "COMPANY_TYPE", brandId, "", data.companyType.getType()
    );


    // ===== Competitors =====
    for(const auto& competitor : data.competitors)
    {
        competitorRepo.update(
            competitor.getId(),
            std::optional<std::string>(competitor.getName()),
            competitor.getType()
        );

        ActivityDetails::logActivity(
            conn, userId, "UPDATE", "COMPETITOR", competitor.getId(), "", competitor.getName()
        );
    }


    // ===== Goals =====
    for(const auto& goal : data.goals)
    {
        goalRepo.update(
            goal.getId(),
            goal.getDescription()
        );

        ActivityDetails::logActivity(
            conn, userId, "UPDATE", "GOAL", goal.getId(), "", goal.getDescription().value_or("")
        );
    }
}


// READ OPERATION
// Fetches a complete BrandAggregate by assembling data from all
// related tables including brand details, competitors, goals,
// revenues, insights, leadership, and contacts.
BrandAggregate BrandAggregateRepository::getById(int brandId)
{
    BrandAggregate aggregate;

    aggregate.brand = brandRepo.getById(brandId);

    auto companyType = companyTypeRepo.getByBrandId(brandId);
    if (companyType.has_value())
        aggregate.companyType = companyType.value();

    auto standard = standardRepo.getByBrandId(brandId);
    if (standard.has_value())
        aggregate.standard = standard.value();

    auto insights = insightsRepo.getByBrandId(brandId);
    if (insights.has_value())
        aggregate.insights = insights.value();

    auto revenues = revenueRepo.getByBrandId(brandId);
    if (!revenues.empty())
        aggregate.revenue = revenues.front();

    aggregate.competitors = competitorRepo.getByBrandId(brandId);
    aggregate.goals = goalRepo.getByBrandId(brandId);
    aggregate.newsFundings = newsFundingRepo.getByBrandId(brandId);
    aggregate.heads = headRepo.getByBrandId(brandId);

    for(auto& head : aggregate.heads)
    {
        auto contacts = headContactRepo.getByHeadId(head.getId());

        for(const auto& contact : contacts)
            head.addContact(contact);
    }

    return aggregate;
}


// READ OPERATION
// Fetches a complete BrandAggregate by assembling data from all
// related tables including brand details, competitors, goals,
// revenues, insights, leadership, and contacts.
void BrandAggregateRepository::softDelete(
    sql::Connection* conn,
    int userId,
    int brandId
)
{
    auto competitors = competitorRepo.getByBrandId(brandId);

    for(const auto& competitor : competitors)
    {
        competitorRepo.softDelete(competitor.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "COMPETITOR", competitor.getId(), "", ""
        );
    }

    auto goals = goalRepo.getByBrandId(brandId);

    for(const auto& goal : goals)
    {
        goalRepo.softDelete(goal.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "GOAL", goal.getId(), "", ""
        );
    }

    brandRepo.softDelete(brandId);

    ActivityDetails::logActivity(
        conn, userId, "DELETE", "BRAND", brandId, "", ""
    );
}