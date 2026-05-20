// BrandAggregateRepository : AKA the ORCHESTRA OF THE BRAND RELATED TABLES
// Responsible for coordinating multi-table operations related to the Brand domain.
// Individual repositories handle table-level persistence, while this class
// orchestrates operations that span across multiple entities.
#include <unordered_set>

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
    CompetitorRepository& competitorRepo,
    GoalRepository& goalRepo,
    HeadRepository& headRepo,
    HeadContactRepository& headContactRepo,
    InsightsRepository& insightsRepo,
    NewsFundingRepository& newsFundingRepo,
    PositionRepository& positionRepo,
    RevenueRepository& revenueRepo,
    BrandStandardRepository& standardRepo,
    GenreRepository& genreRepo,
    TargetAudienceRepository& targetRepo,
    BrandGenreRepository& brandGenreRepo,
    BrandTargetAudienceRepository& brandTargetRepo,
    PastInfluencerCampRepository& pastInfluencerRepo
)
: brandRepo(brandRepo),
  competitorRepo(competitorRepo),
  goalRepo(goalRepo),
  headRepo(headRepo),
  headContactRepo(headContactRepo),
  insightsRepo(insightsRepo),
  newsFundingRepo(newsFundingRepo),
  positionRepo(positionRepo),
  revenueRepo(revenueRepo),
  standardRepo(standardRepo),
  genreRepo(genreRepo),
  targetRepo(targetRepo),
  brandGenreRepo(brandGenreRepo),
  brandTargetRepo(brandTargetRepo),
  pastInfluencerRepo(pastInfluencerRepo)
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
    int brandId = brandRepo.insertBrand(conn,data.brand.getName(), data.brand.getPsychographics(), data.brand.getGenreDescription(), data.brand.getCompanyType());

    ActivityDetails::logActivity(
        conn, userId, "CREATE", "BRAND", brandId, "", data.brand.getName()
    );

    // ===== Standard =====
    if (data.standard.has_value())
    {
        standardRepo.insert(
            conn,
            brandId,
            data.standard->getPosition(),
            data.standard->getIdentity(),
            data.standard->getCSR(),
            data.standard->getQuality(),
            data.standard->getPerception()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE", "BRAND_STANDARD", brandId, "", "created"
        );
    }


    if (data.insights.has_value())
    {
        insightsRepo.insert(
            conn,
            brandId,
            data.insights->getPitchAngle(),
            data.insights->getLeverage(),
            data.insights->getGap(),
            data.insights->getHook()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE", "INSIGHTS", brandId, "", "created"
        );
    }


    // ===== Revenue =====
for (const auto& rev : data.revenue)
{
    revenueRepo.insert(
        conn,
        brandId,
        rev.getYear().value_or(0),
        rev.getPeriod(),
        rev.getSource(),
        rev.getType(),
        rev.getValue(),
        rev.getCurrency()
    );

    ActivityDetails::logActivity(
        conn, userId, "CREATE", "REVENUE", brandId, "", "created"
    );
}


    // ===== Competitors =====
    for(const auto& competitor : data.competitors)
    {
        int competitorId = competitorRepo.insert(
            conn,
            brandId,
            competitor.getName(),
            competitor.getType()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE", "COMPETITORS", competitorId, "", competitor.getName()
        );
    }


    // ===== Goals =====
    for(const auto& goal : data.goals)
    {
        int goalId = goalRepo.insert(
            conn,
            brandId,
            goal.getDescription()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE", "GOALS", goalId, "", goal.getDescription().value_or("")
        );
    }


    // ===== News Funding =====
    for(const auto& news : data.newsFundings)
    {
        int newsId = newsFundingRepo.insert(
            conn,
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
            conn,
            brandId,
            head.getPosition().getId(),
            head.getName(),
            head.getLinkedIn(),
            head.getPersonalMail(),
            head.getOfficialMail()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE", "HEADS", headId, "", head.getName()
        );


        for(const auto& contact : head.getContacts())
        {
            int contactId = headContactRepo.insert(
                conn,
                headId,
                contact.getType(),
                contact.getValue()
            );

            ActivityDetails::logActivity(
                conn, userId, "CREATE", "HEAD_CONTACT", contactId, "", contact.getValue()
            );
        }
    }

        // ===== Brand Genre (M:M) =====
    for(const auto& genre : data.genres)
    {
        brandGenreRepo.insert(
            brandId,
            genre.getId()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE", "BRAND_GENRE", brandId, "", "linked"
        );
    }

    // ===== Brand Target Audience (M:M) =====
    for(const auto& aud : data.targetAudience)
    {
        brandTargetRepo.insert(
            brandId,
            aud.getId()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE", "BRAND_TARGET_AUDIENCE", brandId, "", "linked"
        );
    }

        // ===== Past Influencer Campaign =====
    for (const auto& pic : data.pastInfluencerCamps)
    {
        int picId = pastInfluencerRepo.insert(
            conn,
            brandId,
            pic.getAmbassadors(),
            pic.getCampaigns(),
            pic.getStrategy()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE", "PAST_INFLUENCER_CAMP", picId, "", "created"
        );
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
    if (!data.brand.getName().empty())
    {
        brandRepo.updateName(
            conn,brandId,
            std::optional<std::string>(data.brand.getName())
        );
    }

    if (data.brand.getPsychographics().has_value())
    {
        brandRepo.updatePsychographics(
            conn,brandId,
            data.brand.getPsychographics()
        );
    }

    if (data.brand.getGenreDescription().has_value())
    {
        brandRepo.updateGenreDescription(
            conn,brandId,
            data.brand.getGenreDescription()
        );
    }

    if (data.brand.getCompanyType().has_value())
    {
        brandRepo.updateCompanyType(
            conn,brandId,
            data.brand.getCompanyType()
        );
    }

    ActivityDetails::logActivity(
        conn,
        userId,
        "UPDATE",
        "BRAND",
        brandId,
        "",
        (!data.brand.getName().empty()
            ? data.brand.getName()
            : "PATCH UPDATE")
    );

    // ===== Standard =====
    if (data.standard.has_value())
    {
        auto existingStandard =
            standardRepo.getByBrandId(brandId);

        if(existingStandard.has_value())
        {
            Standard merged =
                existingStandard.value();

            // ===== POSITION =====
            if(data.standard->getPosition().has_value())
            {
                merged.setPosition(
                    data.standard->getPosition()
                );
            }
            else if(
                data.clearFields.count("standard.position")
            )
            {
                merged.setPosition(std::nullopt);
            }

            // ===== IDENTITY =====
            if(data.standard->getIdentity().has_value())
            {
                merged.setIdentity(
                    data.standard->getIdentity()
                );
            }
            else if(
                data.clearFields.count("standard.identity")
            )
            {
                merged.setIdentity(std::nullopt);
            }

            // ===== CSR =====
            if(data.standard->getCSR().has_value())
            {
                merged.setCSR(
                    data.standard->getCSR()
                );
            }
            else if(
                data.clearFields.count("standard.csr")
            )
            {
                merged.setCSR(std::nullopt);
            }

            // ===== QUALITY =====
            if(data.standard->getQuality().has_value())
            {
                merged.setQuality(
                    data.standard->getQuality()
                );
            }
            else if(
                data.clearFields.count("standard.quality")
            )
            {
                merged.setQuality(std::nullopt);
            }

            // ===== PERCEPTION =====
            if(data.standard->getPerception().has_value())
            {
                merged.setPerception(
                    data.standard->getPerception()
                );
            }
            else if(
                data.clearFields.count("standard.perception")
            )
            {
                merged.setPerception(std::nullopt);
            }

            standardRepo.update(
                conn,
                merged.getId(),
                merged.getPosition(),
                merged.getIdentity(),
                merged.getCSR(),
                merged.getQuality(),
                merged.getPerception()
            );

            ActivityDetails::logActivity(
                conn,
                userId,
                "UPDATE",
                "STANDARD",
                merged.getId(),
                "",
                "updated"
            );
        }
        else
        {
            int standardId =
                standardRepo.insert(
                    conn,
                    brandId,
                    data.standard->getPosition(),
                    data.standard->getIdentity(),
                    data.standard->getCSR(),
                    data.standard->getQuality(),
                    data.standard->getPerception()
                );

            ActivityDetails::logActivity(
                conn,
                userId,
                "CREATE",
                "STANDARD",
                standardId,
                "",
                "created"
            );
        }
    }



    // ===== Insights =====
    if (data.insights.has_value())
    {
        auto existingInsights =
            insightsRepo.getByBrandId(brandId);

        if(existingInsights.has_value())
        {
            Insights merged =
                existingInsights.value();

            // ===== PITCH ANGLE =====
            if(data.insights->getPitchAngle().has_value())
            {
                merged.setPitchAngle(
                    data.insights->getPitchAngle()
                );
            }
            else if(
                data.clearFields.count("insights.pitch_angle")
            )
            {
                merged.setPitchAngle(std::nullopt);
            }

            // ===== LEVERAGE =====
            if(data.insights->getLeverage().has_value())
            {
                merged.setLeverage(
                    data.insights->getLeverage()
                );
            }
            else if(
                data.clearFields.count("insights.leverage")
            )
            {
                merged.setLeverage(std::nullopt);
            }

            // ===== GAP =====
            if(data.insights->getGap().has_value())
            {
                merged.setGap(
                    data.insights->getGap()
                );
            }
            else if(
                data.clearFields.count("insights.gap")
            )
            {
                merged.setGap(std::nullopt);
            }

            // ===== HOOK =====
            if(data.insights->getHook().has_value())
            {
                merged.setHook(
                    data.insights->getHook()
                );
            }
            else if(
                data.clearFields.count("insights.hook")
            )
            {
                merged.setHook(std::nullopt);
            }

            insightsRepo.update(
                conn,
                merged.getId(),
                merged.getPitchAngle(),
                merged.getLeverage(),
                merged.getGap(),
                merged.getHook()
            );

            ActivityDetails::logActivity(
                conn,
                userId,
                "UPDATE",
                "INSIGHTS",
                merged.getId(),
                "",
                "updated"
            );
        }
        else
        {
            int insightsId =
                insightsRepo.insert(
                    conn,
                    brandId,
                    data.insights->getPitchAngle(),
                    data.insights->getLeverage(),
                    data.insights->getGap(),
                    data.insights->getHook()
                );

            ActivityDetails::logActivity(
                conn,
                userId,
                "CREATE",
                "INSIGHTS",
                insightsId,
                "",
                "created"
            );
        }
    }


    // ===== Revenue =====
    for (const auto& revenue : data.revenue)
    {
        if (revenue.getId() != 0)
        {
            auto existingRevenue =
                revenueRepo.getById(
                    revenue.getId()
                );

            if(existingRevenue.has_value())
            {
                Revenue merged =
                    existingRevenue.value();

                // ===== YEAR =====
                if(revenue.getYear().has_value())
                {
                    merged.setYear(
                        revenue.getYear()
                    );
                }

                // ===== PERIOD =====
                if(revenue.getPeriod().has_value())
                {
                    merged.setPeriod(
                        revenue.getPeriod()
                    );
                }

                // ===== SOURCE =====
                if(revenue.getSource().has_value())
                {
                    merged.setSource(
                        revenue.getSource()
                    );
                }

                // ===== TYPE =====
                if(revenue.getType().has_value())
                {
                    merged.setType(
                        revenue.getType()
                    );
                }

                // ===== VALUE =====
                if(revenue.getValue() > 0)
                {
                    merged.setValue(
                        revenue.getValue()
                    );
                }

                // ===== CURRENCY =====
                if(revenue.getCurrency().has_value())
                {
                    merged.setCurrency(
                        revenue.getCurrency()
                    );
                }

                revenueRepo.update(
                    conn,
                    merged.getId(),
                    merged.getYear(),
                    merged.getPeriod(),
                    merged.getSource(),
                    merged.getType(),
                    std::optional<double>(
                        merged.getValue()
                    ),
                    merged.getCurrency()
                );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "UPDATE",
                    "REVENUE",
                    merged.getId(),
                    "",
                    "updated"
                );
            }
        }
        else
        {
            int revenueId =
                revenueRepo.insert(
                    conn,
                    brandId,
                    revenue.getYear().value_or(0),
                    revenue.getPeriod(),
                    revenue.getSource(),
                    revenue.getType(),
                    revenue.getValue(),
                    revenue.getCurrency()
                );

            ActivityDetails::logActivity(
                conn,
                userId,
                "CREATE",
                "REVENUE",
                revenueId,
                "",
                "created"
            );
            continue;
        }
    }
    

    // ===== Competitors =====
    for (const auto& competitor : data.competitors)
    {
        if (competitor.getId() != 0)
        {
            auto existingCompetitor =
                competitorRepo.getById(
                    competitor.getId()
                );

            if(existingCompetitor.has_value())
            {
                Competitor merged =
                    existingCompetitor.value();

                // ===== NAME =====
                if(
                    !competitor.getName().empty()
                )
                {
                    merged.setName(
                        competitor.getName()
                    );
                }

                // ===== TYPE =====
                if(
                    competitor.getType().has_value()
                )
                {
                    merged.setType(
                        competitor.getType()
                    );
                }

                competitorRepo.update(
                    conn,
                    merged.getId(),
                    merged.getName(),
                    merged.getType()
                );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "UPDATE",
                    "COMPETITOR",
                    merged.getId(),
                    "",
                    "updated"
                );
                
            }
        }
        else
        {
            int competitorId =
                competitorRepo.insert(
                    conn,
                    brandId,
                    competitor.getName(),
                    competitor.getType()
                );

            ActivityDetails::logActivity(
                conn,
                userId,
                "CREATE",
                "COMPETITOR",
                competitorId,
                "",
                competitor.getName()
            );
            continue;
        }
    }

    // ===== Goals =====
    for (const auto& goal : data.goals)
    {
        if (goal.getId() != 0)
        {
            auto existingGoal =
                goalRepo.getById(
                    goal.getId()
                );

            if(existingGoal.has_value())
            {
                Goal merged =
                    existingGoal.value();

                // ===== DESCRIPTION =====
                if(
                    goal.getDescription().has_value()
                )
                {
                    merged.setDescription(
                        goal.getDescription()
                    );
                }

                goalRepo.update(
                    conn,
                    merged.getId(),
                    merged.getDescription()
                );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "UPDATE",
                    "GOAL",
                    merged.getId(),
                    "",
                    merged.getDescription()
                        .value_or("")
                );
            }
        }
        else
        {
            int goalId =
                goalRepo.insert(
                    conn,
                    brandId,
                    goal.getDescription()
                );

            ActivityDetails::logActivity(
                conn,
                userId,
                "CREATE",
                "GOALS",
                goalId,
                "",
                goal.getDescription().value_or("")
            );
            continue;
        }
    }


    // ===== News Funding =====
    for (const auto& news : data.newsFundings)
    {
        if (news.getId() != 0)
        {
            auto existingNews =
                newsFundingRepo.getById(
                    news.getId()
                );

            if(existingNews.has_value())
            {
                NewsFunding merged =
                    existingNews.value();

                // ===== EVENT TYPE =====
                if(
                    news.getEventType().has_value()
                )
                {
                    merged.setEventType(
                        news.getEventType()
                    );
                }

                // ===== TITLE =====
                if(
                    news.getTitle().has_value()
                )
                {
                    merged.setTitle(
                        news.getTitle()
                    );
                }

                // ===== DESCRIPTION =====
                if(
                    news.getDescription().has_value()
                )
                {
                    merged.setDescription(
                        news.getDescription()
                    );
                }

                // ===== DATE =====
                if(
                    news.getDate().has_value()
                )
                {
                    merged.setDate(
                        news.getDate().value()
                    );
                }

                newsFundingRepo.update(
                    conn,
                    merged.getId(),
                    merged.getEventType(),
                    merged.getTitle(),
                    merged.getDescription(),
                    merged.getDate()
                );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "UPDATE",
                    "NEWS_FUNDING",
                    merged.getId(),
                    "",
                    merged.getTitle()
                        .value_or("")
                );
            }
        }
        else
        {
            int newsId =
                newsFundingRepo.insert(
                    conn,
                    brandId,
                    news.getEventType(),
                    news.getTitle(),
                    news.getDescription(),
                    news.getDate()
                );

            ActivityDetails::logActivity(
                conn,
                userId,
                "CREATE",
                "NEWS_FUNDING",
                newsId,
                "",
                news.getTitle().value_or("")
            );
            continue;
        }
    }

    // ===== Heads =====
    for (const auto& head : data.heads)
    {
        if (head.getId() != 0)
        {
            auto existingHead =
                headRepo.getById(
                    head.getId()
                );

            if(existingHead.has_value())
            {
                BrandHead merged =
                    existingHead.value();

                // ===== POSITION =====
                if(
                    head.getPosition().getId() != 0
                )
                {
                    merged.setPosition(
                        head.getPosition()
                    );
                }

                // ===== NAME =====
                if(!head.getName().empty())
                {
                    merged.setName(
                        head.getName()
                    );
                }

                // ===== LINKEDIN =====
                if(
                    head.getLinkedIn().has_value()
                )
                {
                    merged.setLinkedIn(
                        head.getLinkedIn()
                    );
                }

                // ===== PERSONAL MAIL =====
                if(
                    !head.getPersonalMail().empty()
                )
                {
                    merged.setPersonalMail(
                        head.getPersonalMail()
                    );
                }

                // ===== OFFICIAL MAIL =====
                if(
                    !head.getOfficialMail().empty()
                )
                {
                    merged.setOfficialMail(
                        head.getOfficialMail()
                    );
                }

                headRepo.update(
                    conn,
                    merged.getId(),

                    (merged.getPosition().getId() != 0)
                        ? std::optional<int>(
                            merged.getPosition().getId()
                        )
                        : std::nullopt,

                    (!merged.getName().empty())
                        ? std::optional<std::string>(
                            merged.getName()
                        )
                        : std::nullopt,

                    merged.getLinkedIn(),

                    (!merged.getPersonalMail().empty())
                        ? std::optional<std::string>(
                            merged.getPersonalMail()
                        )
                        : std::nullopt,

                    (!merged.getOfficialMail().empty())
                        ? std::optional<std::string>(
                            merged.getOfficialMail()
                        )
                        : std::nullopt
                );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "UPDATE",
                    "HEAD",
                    merged.getId(),
                    "",
                    merged.getName()
                );

                // ===== Head Contacts =====
                for (const auto& contact : head.getContacts())
                {
                    if (contact.getId() != 0)
                    {
                        auto existingContact =
                            headContactRepo.getById(
                                contact.getId()
                            );

                        if(existingContact.has_value())
                        {
                            HeadContact mergedContact =
                                existingContact.value();

                            // ===== TYPE =====
                            if(
                                contact.getType().has_value()
                            )
                            {
                                mergedContact.setType(
                                    contact.getType()
                                );
                            }

                            // ===== VALUE =====
                            if(
                                !contact.getValue().empty()
                            )
                            {
                                mergedContact.setValue(
                                    contact.getValue()
                                );
                            }

                            headContactRepo.update(
                                conn,
                                mergedContact.getId(),

                                mergedContact.getType(),

                                (!mergedContact.getValue().empty())
                                    ? std::optional<std::string>(
                                        mergedContact.getValue()
                                    )
                                    : std::nullopt
                            );

                            ActivityDetails::logActivity(
                                conn,
                                userId,
                                "UPDATE",
                                "HEAD_CONTACT",
                                mergedContact.getId(),
                                "",
                                mergedContact.getValue()
                            );
                        }
                    }
                }
            }
        }
        else
        {
            int headId =
                headRepo.insert(
                    conn,
                    brandId,
                    head.getPosition().getId(),
                    head.getName(),
                    head.getLinkedIn(),
                    head.getPersonalMail(),
                    head.getOfficialMail()
                );

            ActivityDetails::logActivity(
                conn,
                userId,
                "CREATE",
                "HEAD",
                headId,
                "",
                head.getName()
            );

            for(const auto& contact : head.getContacts())
            {
                int contactId =
                    headContactRepo.insert(
                        conn,
                        headId,
                        contact.getType(),
                        contact.getValue()
                    );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "CREATE",
                    "HEAD_CONTACT",
                    contactId,
                    "",
                    contact.getValue()
                );
            }
            continue;
        }
    }

    // ===== Brand Genre (M:M SYNC) =====
    if(!data.genres.empty())
    {
        auto existingGenres =
            brandGenreRepo.getByBrandId(brandId);

        std::unordered_set<int> existingIds;

        for(const auto& g : existingGenres)
        {
            existingIds.insert(g.getId());
        }

        std::unordered_set<int> incomingIds;

        for(const auto& g : data.genres)
        {
            incomingIds.insert(g.getId());
        }

        // DELETE removed
        for(int id : existingIds)
        {
            if(incomingIds.find(id) == incomingIds.end())
            {
                brandGenreRepo.softDelete(
                    brandId,
                    id
                );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "DELETE",
                    "BRAND_GENRE",
                    brandId,
                    "",
                    ""
                );
            }
        }

        // INSERT new
        for(int id : incomingIds)
        {
            if(existingIds.find(id) == existingIds.end())
            {
                brandGenreRepo.insert(
                    brandId,
                    id
                );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "CREATE",
                    "BRAND_GENRE",
                    brandId,
                    "",
                    "linked"
                );
            }
        }
    }

    // ===== Brand Target Audience (M:M SYNC) =====
    if(!data.targetAudience.empty())
    {
        auto existingAud =
            brandTargetRepo.getByBrandId(brandId);

        std::unordered_set<int> existingIds;

        for(const auto& a : existingAud)
        {
            existingIds.insert(a.getId());
        }

        std::unordered_set<int> incomingIds;

        for(const auto& a : data.targetAudience)
        {
            incomingIds.insert(a.getId());
        }

        // DELETE removed
        for(int id : existingIds)
        {
            if(incomingIds.find(id) == incomingIds.end())
            {
                brandTargetRepo.softDelete(
                    brandId,
                    id
                );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "DELETE",
                    "BRAND_TARGET_AUDIENCE",
                    brandId,
                    "",
                    ""
                );
            }
        }

        // INSERT new
        for(int id : incomingIds)
        {
            if(existingIds.find(id) == existingIds.end())
            {
                brandTargetRepo.insert(
                    brandId,
                    id
                );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "CREATE",
                    "BRAND_TARGET_AUDIENCE",
                    brandId,
                    "",
                    "linked"
                );
            }
        }
    }

    // ===== Past Influencer Campaign =====
    for (const auto& pic : data.pastInfluencerCamps)
    {
        if (pic.getId() != 0)
        {
            auto existingPIC =
                pastInfluencerRepo.getById(
                    pic.getId()
                );

            if(existingPIC.has_value())
            {
                PastInfluencerCamp merged =
                    existingPIC.value();

                // ===== AMBASSADORS =====
                if(
                    pic.getAmbassadors().has_value()
                )
                {
                    merged.setAmbassadors(
                        pic.getAmbassadors()
                    );
                }

                // ===== CAMPAIGNS =====
                if(
                    pic.getCampaigns().has_value()
                )
                {
                    merged.setCampaigns(
                        pic.getCampaigns()
                    );
                }

                // ===== STRATEGY =====
                if(
                    pic.getStrategy().has_value()
                )
                {
                    merged.setStrategy(
                        pic.getStrategy()
                    );
                }

                pastInfluencerRepo.update(
                    conn,
                    merged.getId(),
                    merged.getAmbassadors(),
                    merged.getCampaigns(),
                    merged.getStrategy()
                );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "UPDATE",
                    "PAST_INFLUENCER_CAMP",
                    merged.getId(),
                    "",
                    "updated"
                );
            }
        }
        else
        {
            int picId =
                pastInfluencerRepo.insert(
                    conn,
                    brandId,
                    pic.getAmbassadors(),
                    pic.getCampaigns(),
                    pic.getStrategy()
                );

            ActivityDetails::logActivity(
                conn,
                userId,
                "CREATE",
                "PAST_INFLUENCER_CAMP",
                picId,
                "",
                "created"
            );
        }
        continue;
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

    auto standard = standardRepo.getByBrandId(brandId);
    if (standard.has_value())
        aggregate.standard = standard.value();

    auto insights = insightsRepo.getByBrandId(brandId);
    if (insights.has_value())
        aggregate.insights = insights.value();

    auto revenues = revenueRepo.getByBrandId(brandId);
    if (!revenues.empty())
        aggregate.revenue = revenues;

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

    aggregate.genres = brandGenreRepo.getByBrandId(brandId);
    aggregate.targetAudience = brandTargetRepo.getByBrandId(brandId);

    aggregate.pastInfluencerCamps =
    pastInfluencerRepo.getByBrandId(brandId);

    // =========================
    // 🔥 FIX: SYNC DATA INTO BRAND
    // =========================

    // 1:M
    for (const auto& g : aggregate.goals)
        aggregate.brand.addGoal(g);

    for (const auto& c : aggregate.competitors)
        aggregate.brand.addCompetitor(c);

    for (const auto& r : aggregate.revenue)
        aggregate.brand.addRevenue(r);

    for (const auto& n : aggregate.newsFundings)
        aggregate.brand.addNewsFunding(n);

    for (const auto& h : aggregate.heads)
        aggregate.brand.addHead(h);

    if (aggregate.standard.has_value())
    {
        aggregate.brand.setBrandStandard(aggregate.standard.value());
    }

    if (aggregate.insights.has_value())
    {
        aggregate.brand.setInsights(aggregate.insights.value());
    }

    // M:M
    for (const auto& g : aggregate.genres)
        aggregate.brand.addGenre(g);

    for (const auto& a : aggregate.targetAudience)
        aggregate.brand.addTargetAudience(a);
    return aggregate;
}





void BrandAggregateRepository::softDelete(
    sql::Connection* conn,
    int userId,
    int brandId
)
{
    // ===== Competitors =====
    auto competitors = competitorRepo.getByBrandId(brandId);
    for (const auto& comp : competitors)
    {
        competitorRepo.softDelete(conn,comp.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "COMPETITOR", comp.getId(), "", ""
        );
    }

    // ===== Goals =====
    auto goals = goalRepo.getByBrandId(brandId);
    for (const auto& goal : goals)
    {
        goalRepo.softDelete(conn,goal.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "GOALS", goal.getId(), "", ""
        );
    }

    // ===== News Funding =====
    auto newsList = newsFundingRepo.getByBrandId(brandId);
    for (const auto& news : newsList)
    {
        newsFundingRepo.softDelete(conn,news.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "NEWS_FUNDING", news.getId(), "", ""
        );
    }

    // ===== Heads + Contacts =====
    auto heads = headRepo.getByBrandId(brandId);
    for (const auto& head : heads)
    {
        auto contacts = headContactRepo.getByHeadId(head.getId());

        for (const auto& contact : contacts)
        {
            headContactRepo.softDelete(conn,contact.getId());

            ActivityDetails::logActivity(
                conn, userId, "DELETE", "HEAD_CONTACT", contact.getId(), "", ""
            );
        }

        headRepo.softDelete(conn,head.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "HEAD", head.getId(), "", ""
        );
    }

    // ===== Revenue =====
    auto revenues = revenueRepo.getByBrandId(brandId);
    for (const auto& rev : revenues)
    {
        revenueRepo.softDelete(conn,rev.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "REVENUE", rev.getId(), "", ""
        );
    }

    // ===== Insights =====
    auto insights = insightsRepo.getByBrandId(brandId);
    if (insights.has_value())
    {
        insightsRepo.softDelete(conn,insights->getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "INSIGHTS", insights->getId(), "", ""
        );
    }

    // ===== Standard =====
    auto standard = standardRepo.getByBrandId(brandId);
    if (standard.has_value())
    {
        standardRepo.softDelete(conn,standard->getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "STANDARD", standard->getId(), "", ""
        );
    }

    auto genres = brandGenreRepo.getByBrandId(brandId);
    for(const auto& genre : genres){
        brandGenreRepo.softDelete(brandId, genre.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "BRAND_GENRE", brandId, "", "" 
        );
    }

    auto audiences = brandTargetRepo.getByBrandId(brandId);
    for(const auto& aud : audiences){
        brandTargetRepo.softDelete(brandId, aud.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "BRAND_TARGET_AUDIENCE", brandId, "", ""
        );
    }

        // ===== Past Influencer Campaign =====
    auto pics = pastInfluencerRepo.getByBrandId(brandId);

    for (const auto& pic : pics)
    {
        pastInfluencerRepo.softDelete(conn,pic.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "PAST_INFLUENCER_CAMP", pic.getId(), "", ""
        );
    }
    // ===== Brand =====
    brandRepo.softDelete(conn,brandId);

    ActivityDetails::logActivity(
        conn, userId, "DELETE", "BRAND", brandId, "", ""
    );
}

void BrandAggregateRepository::deletePartial(
    sql::Connection* conn,
    int userId,
    int brandId,
    const BrandAggregate& data
)
{
    // ===== Goals =====
    for (const auto& g : data.goals)
    {
        goalRepo.softDelete(conn,g.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "GOALS", g.getId(), "", ""
        );
    }

    // ===== Competitors =====
    for (const auto& c : data.competitors)
    {
        competitorRepo.softDelete(conn,c.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "COMPETITOR", c.getId(), "", ""
        );
    }

    // ===== Revenue =====
    for (const auto& r : data.revenue)
    {
        revenueRepo.softDelete(conn,r.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "REVENUE", r.getId(), "", ""
        );
    }

    // ===== News =====
    for (const auto& n : data.newsFundings)
    {
        newsFundingRepo.softDelete(conn,n.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "NEWS_FUNDING", n.getId(), "", ""
        );
    }

    // ===== Heads =====
    for (const auto& h : data.heads)
    {
        // contacts first
        for (const auto& c : h.getContacts())
        {
            headContactRepo.softDelete(conn,c.getId());

            ActivityDetails::logActivity(
                conn, userId, "DELETE", "HEAD_CONTACT", c.getId(), "", ""
            );
        }

        headRepo.softDelete(conn,h.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "HEAD", h.getId(), "", ""
        );
    }

    // ===== Insights =====
    if (data.insights.has_value())
    {
        auto existingInsights =
            insightsRepo.getByBrandId(brandId);

        if(existingInsights.has_value())
        {
            insightsRepo.softDelete(
                conn,existingInsights->getId()
            );

            ActivityDetails::logActivity(
                conn,
                userId,
                "DELETE",
                "INSIGHTS",
                existingInsights->getId(),
                "",
                ""
            );
        }
    }

    // ===== Standard =====
    if (data.standard.has_value())
    {
        auto existingStandard =
            standardRepo.getByBrandId(brandId);

        if(existingStandard.has_value())
        {
            standardRepo.softDelete(
                conn,existingStandard->getId()
            );

            ActivityDetails::logActivity(
                conn,
                userId,
                "DELETE",
                "STANDARD",
                existingStandard->getId(),
                "",
                ""
            );
        }
    }

    // ===== Genre unlink =====
    for (const auto& g : data.genres)
    {
        brandGenreRepo.softDelete(brandId, g.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "BRAND_GENRE", brandId, "", ""
        );
    }

    // ===== Audience unlink =====
    for (const auto& a : data.targetAudience)
    {
        brandTargetRepo.softDelete(brandId, a.getId());

        ActivityDetails::logActivity(
            conn, userId, "DELETE", "BRAND_TARGET_AUDIENCE", brandId, "", ""
        );
    }
        // ===== Past Influencer Campaign =====
    for (const auto& pic : data.pastInfluencerCamps)
    {
        pastInfluencerRepo.softDelete(
            conn,pic.getId()
        );

        ActivityDetails::logActivity(
            conn,
            userId,
            "DELETE",
            "PAST_INFLUENCER_CAMP",
            pic.getId(),
            "",
            ""
        );
    }
}