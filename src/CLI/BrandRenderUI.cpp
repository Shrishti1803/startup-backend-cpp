#include "CLI/BrandRenderUI.h"
#include "CLI/UI/TerminalLayout.h"

#include <iostream>
#include <iomanip>

void BrandRenderer::printSectionHeader(
    const std::string& title
)
{
    UI::title(title);
}

void BrandRenderer::displayCompactHeader(
    const BrandAggregate& data
)
{
    std::cout
        << "\n==================================================\n";

    std::cout
        << " BRAND : "
        << data.brand.getName()
        << "\n";

    std::cout
        << "==================================================\n";

    // ===== POSITION =====

    if(data.standard.has_value())
    {
        std::cout
            << " Position : "
            << data.standard->getPosition()
                   .value_or("-")
            << "\n";
    }

    // ===== GENRES =====

    std::cout
        << " Genres   : ";

    if(data.genres.empty())
    {
        std::cout << "-";
    }

    else
    {
        for(size_t i = 0;
            i < data.genres.size();
            i++)
        {
            std::cout
                << data.genres[i]
                       .getCategory();

            if(i != data.genres.size() - 1)
            {
                std::cout << ", ";
            }
        }
    }

    std::cout << "\n";

    // ===== AUDIENCE =====

    std::cout
        << " Audience : ";

    if(data.targetAudience.empty())
    {
        std::cout << "-";
    }

    else
    {
        for(size_t i = 0;
            i < data.targetAudience.size();
            i++)
        {
            std::cout
                << data.targetAudience[i]
                       .getName();

            if(i != data.targetAudience.size() - 1)
            {
                std::cout << ", ";
            }
        }
    }

    std::cout
        << "\n==================================================\n";
}

void BrandRenderer::displayGoals(
    const BrandAggregate& data
)
{
    printSectionHeader(
        "GOALS"
    );

    if(data.goals.empty())
    {
        UI::warning(
            "No goals available."
        );

        return;
    }

    for(const auto& goal :
        data.goals)
    {
        std::cout
            << "  ["
            << goal.getId()
            << "] "
            << goal.getDescription()
                   .value_or("-")
            << "\n";
    }
}

void BrandRenderer::displayHeads(
    const BrandAggregate& data
)
{
    printSectionHeader(
        "HEADS"
    );

    if(data.heads.empty())
    {
        UI::warning(
            "No heads available."
        );

        return;
    }

    for(const auto& head :
        data.heads)
    {
        std::cout
            << "\n----------------------------------\n";

        std::cout
            << "  ["
            << head.getId()
            << "] "
            << head.getName()
            << "\n";

        std::cout
            << "      Position : "
            << head.getPosition()
                   .getName()
            << "\n";

        if(head.getLinkedIn().has_value())
        {
            std::cout
                << "      LinkedIn : "
                << head.getLinkedIn()
                       .value()
                << "\n";
        }

        std::cout
            << "      Official : "
            << head.getOfficialMail()
            << "\n";

        std::cout
            << "      Personal : "
            << head.getPersonalMail()
            << "\n";

        // ===== CONTACTS =====

        if(!head.getContacts().empty())
        {
            std::cout
                << "      Contacts :\n";

            for(const auto& c :
                head.getContacts())
            {
                std::cout
                    << "         - "
                    << c.getType()
                           .value_or("Unknown")
                    << " : "
                    << c.getValue()
                    << "\n";
            }
        }
    }
}

void BrandRenderer::displayCompetitors(
    const BrandAggregate& data
)
{
    printSectionHeader(
        "COMPETITORS"
    );

    if(data.competitors.empty())
    {
        UI::warning(
            "No competitors available."
        );

        return;
    }

    for(const auto& comp :
        data.competitors)
    {
        std::cout
            << "  ["
            << comp.getId()
            << "] "
            << comp.getName();

        if(comp.getType().has_value())
        {
            std::cout
                << " ("
                << comp.getType()
                       .value()
                << ")";
        }

        std::cout << "\n";
    }
}

void BrandRenderer::displayRevenue(
    const BrandAggregate& data
)
{
    printSectionHeader(
        "REVENUE"
    );

    if(data.revenue.empty())
    {
        UI::warning(
            "No revenue data available."
        );

        return;
    }

    for(const auto& rev :
        data.revenue)
    {
        std::cout
            << "\n----------------------------------\n";

        std::cout
            << "  ["
            << rev.getId()
            << "] "
            << rev.getValue();

        if(rev.getCurrency().has_value())
        {
            std::cout
                << " "
                << rev.getCurrency()
                       .value();
        }

        std::cout << "\n";

        if(rev.getYear().has_value())
        {
            std::cout
                << "      FY       : "
                << rev.getYear()
                       .value()
                << "\n";
        }

        if(rev.getPeriod().has_value())
        {
            std::cout
                << "      Period   : "
                << rev.getPeriod()
                       .value()
                << "\n";
        }

        if(rev.getType().has_value())
        {
            std::cout
                << "      Type     : "
                << rev.getType()
                       .value()
                << "\n";
        }

        if(rev.getSource().has_value())
        {
            std::cout
                << "      Source   : "
                << rev.getSource()
                       .value()
                << "\n";
        }
    }
}

void BrandRenderer::displayGenres(
    const BrandAggregate& data
)
{
    printSectionHeader(
        "GENRES"
    );

    if(data.genres.empty())
    {
        UI::warning(
            "No genres linked."
        );

        return;
    }

    for(const auto& genre :
        data.genres)
    {
        std::cout
            << "  • "
            << genre.getCategory()
            << "\n";
    }
}

void BrandRenderer::displayAudience(
    const BrandAggregate& data
)
{
    printSectionHeader(
        "TARGET AUDIENCE"
    );

    if(data.targetAudience.empty())
    {
        UI::warning(
            "No target audience linked."
        );

        return;
    }

    for(const auto& aud :
        data.targetAudience)
    {
        std::cout
            << "  • "
            << aud.getName()
            << "\n";
    }
}

void BrandRenderer::displayBrandCore(
    const BrandAggregate& data
)
{
    printSectionHeader(
        "CORE INFORMATION"
    );

    std::cout
        << "  Brand ID      : "
        << data.brand.getId()
        << "\n";

    std::cout
        << "  Brand Name    : "
        << data.brand.getName()
        << "\n";

    std::cout
        << "  Company Type  : "
        << data.brand.getCompanyType()
               .value_or("-")
        << "\n";

    std::cout
        << "  Psychographics:\n"
        << "      "
        << data.brand.getPsychographics()
               .value_or("-")
        << "\n";

    std::cout
        << "  Genre Desc:\n"
        << "      "
        << data.brand.getGenreDescription()
               .value_or("-")
        << "\n";
}

void BrandRenderer::displayInsights(
    const BrandAggregate& data
)
{
    printSectionHeader(
        "INSIGHTS"
    );

    if(!data.insights.has_value())
    {
        UI::warning(
            "No insights available."
        );

        return;
    }

    const auto& insight =
        data.insights.value();

    std::cout
        << "  Pitch Angle:\n"
        << "      "
        << insight.getPitchAngle()
               .value_or("-")
        << "\n\n";

    std::cout
        << "  Leverage:\n"
        << "      "
        << insight.getLeverage()
               .value_or("-")
        << "\n\n";

    std::cout
        << "  Gap:\n"
        << "      "
        << insight.getGap()
               .value_or("-")
        << "\n\n";

    std::cout
        << "  Hook:\n"
        << "      "
        << insight.getHook()
               .value_or("-")
        << "\n";
}

void BrandRenderer::displayStandard(
    const BrandAggregate& data
)
{
    printSectionHeader(
        "BRAND STANDARD"
    );

    if(!data.standard.has_value())
    {
        UI::warning(
            "No standard data available."
        );

        return;
    }

    const auto& standard =
        data.standard.value();

    std::cout
        << "  Position:\n"
        << "      "
        << standard.getPosition()
               .value_or("-")
        << "\n\n";

    std::cout
        << "  Identity:\n"
        << "      "
        << standard.getIdentity()
               .value_or("-")
        << "\n\n";

    std::cout
        << "  CSR:\n"
        << "      "
        << standard.getCSR()
               .value_or("-")
        << "\n\n";

    std::cout
        << "  Quality:\n"
        << "      "
        << standard.getQuality()
               .value_or("-")
        << "\n\n";

    std::cout
        << "  Perception:\n"
        << "      "
        << standard.getPerception()
               .value_or("-")
        << "\n";
}

void BrandRenderer::displayNewsFunding(
    const BrandAggregate& data
)
{
    printSectionHeader(
        "NEWS & FUNDING"
    );

    if(data.newsFundings.empty())
    {
        UI::warning(
            "No news or funding records available."
        );

        return;
    }

    for(const auto& news :
        data.newsFundings)
    {
        std::cout
            << "\n----------------------------------\n";

        std::cout
            << "  ["
            << news.getId()
            << "] ";

        if(news.getEventType().has_value())
        {
            std::cout
                << "("
                << news.getEventType()
                       .value()
                << ") ";
        }

        std::cout
            << news.getTitle()
                   .value_or("-")
            << "\n";

        std::cout
            << "      Description : "
            << news.getDescription()
                   .value_or("-")
            << "\n";

        if(news.getDate().has_value())
        {
            std::cout
                << "      Date        : "
                << news.getDate()
                       ->toString()
                << "\n";
        }
    }
}

void BrandRenderer::displayPastInfluencerCamps(
    const BrandAggregate& data
)
{
    printSectionHeader(
        "PAST INFLUENCER CAMPAIGNS"
    );

    if(data.pastInfluencerCamps.empty())
    {
        UI::warning(
            "No influencer campaigns available."
        );

        return;
    }

    for(const auto& camp :
        data.pastInfluencerCamps)
    {
        std::cout
            << "\n----------------------------------\n";

        std::cout
            << "  ["
            << camp.getId()
            << "]\n";

        std::cout
            << "      Ambassadors : "
            << camp.getAmbassadors()
                   .value_or("-")
            << "\n";

        std::cout
            << "      Campaigns   : "
            << camp.getCampaigns()
                   .value_or("-")
            << "\n";

        std::cout
            << "      Strategy    : "
            << camp.getStrategy()
                   .value_or("-")
            << "\n";
    }
}

void BrandRenderer::displayBrandHeader(
    const BrandAggregate& data
)
{
    UI::title(
        "BRAND DETAILS"
    );

    displayBrandCore(data);

    displayGenres(data);

    displayAudience(data);

    // ================= GOALS =================

    displayGoals(data);

    // ================= HEADS =================

    displayHeads(data);

    // ================= COMPETITORS =================

    displayCompetitors(data);

    // ================= REVENUE =================

    displayRevenue(data);

    // ================= INSIGHTS =================

    displayInsights(data);

    // ================= STANDARD =================

    displayStandard(data);

    // ================= NEWS FUNDING =================

    displayNewsFunding(data);

    // ================= PAST CAMPAIGNS =================

    displayPastInfluencerCamps(data);
}