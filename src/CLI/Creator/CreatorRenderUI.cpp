// CreatorRenderUI.cpp

#include "CLI/Creator/CreatorRenderUI.h"
#include "CLI/UI/TerminalLayout.h"

#include <iostream>

CreatorRenderUI::CreatorRenderUI(
    ReferencesService& refService,
    const Auth::Session& session
)
    : referencesService(refService),
      currentSession(session)
{
}

void CreatorRenderUI::printSectionHeader(
    const std::string& title
)
{
    UI::title(title);
}

void CreatorRenderUI::displayCompactHeader(
    const Creator& data
)
{
    std::cout
        << "\n==================================================\n";

    std::cout
        << " CREATOR : "
        << data.getName()
        << "\n";

    std::cout
        << "==================================================\n";

    std::cout
        << " ID         : "
        << data.getId()
        << "\n";

    std::cout
        << " Email      : "
        << data.getEmail()
               .value_or("-")
        << "\n";

    std::cout
        << " Rating     : ";

    if(data.getRating().has_value())
    {
        std::cout
            << data.getRating()
                   .value();
    }
    else
    {
        std::cout << "-";
    }

    std::cout << "\n";

    std::cout
        << " Reach      : ";

    if(data.getAudienceSize().has_value())
    {
        std::cout
            << data.getAudienceSize()
                   .value();
    }
    else
    {
        std::cout << "-";
    }

    std::cout << "\n";

    // ===== NICHES =====

    std::cout << " Niches     : ";

    if(data.getNiches().empty())
    {
        std::cout << "-";
    }
    else
    {
        for(size_t i = 0;
            i < data.getNiches().size();
            i++)
        {
            std::cout
                << data.getNiches()[i]
                       .getType();

            if(i != data.getNiches().size() - 1)
            {
                std::cout << ", ";
            }
        }
    }

    std::cout << "\n";

    // ===== TARGET AUDIENCE =====

    std::cout << " Audience   : ";

    if(data.getTargetAudience().empty())
    {
        std::cout << "-";
    }
    else
    {
        for(size_t i = 0;
            i < data.getTargetAudience().size();
            i++)
        {
            std::cout
                << data.getTargetAudience()[i]
                       .getName();

            if(i != data.getTargetAudience().size() - 1)
            {
                std::cout << ", ";
            }
        }
    }

    std::cout
        << "\n==================================================\n";
}

/* =====================================================
   CORE
===================================================== */

void CreatorRenderUI::displayCreatorCore(
    const Creator& data
)
{
    printSectionHeader(
        "CORE INFORMATION"
    );

    std::cout
        << "  Creator ID    : "
        << data.getId()
        << "\n";

    std::cout
        << "  Creator Name  : "
        << data.getName()
        << "\n";

    std::cout
        << "  Email         : "
        << data.getEmail()
               .value_or("-")
        << "\n";

    std::cout
        << "  Rating        : ";

    if(data.getRating().has_value())
    {
        std::cout
            << data.getRating()
                   .value();
    }
    else
    {
        std::cout << "-";
    }

    std::cout << "\n";

    std::cout
        << "  Audience Size : ";

    if(data.getAudienceSize().has_value())
    {
        std::cout
            << data.getAudienceSize()
                   .value();
    }
    else
    {
        std::cout << "-";
    }

    std::cout << "\n";
}



/* =====================================================
   COMMUNITY
===================================================== */

void CreatorRenderUI::displayCommunity(
    const Creator& data
)
{
    printSectionHeader(
        "COMMUNITY REACH"
    );

    if(!data.getCommunityReaches().has_value())
    {
        UI::warning("No community data available.");
        return;
    }

    const auto& community =
        data.getCommunityReaches()
            .value();

    std::cout
        << "  Sentiment   : "
        << community.getSentiment()
               .value_or("-")
        << "\n";

    std::cout
        << "  Reply Rate  : ";

    if(community.getReplyRate().has_value())
    {
        std::cout
            << community.getReplyRate()
                   .value();
    }
    else
    {
        std::cout << "-";
    }

    std::cout << "\n";
}



/* =====================================================
   NICHES
===================================================== */

void CreatorRenderUI::displayNiches(
    const Creator& data
)
{
    printSectionHeader(
        "NICHES"
    );

    if(data.getNiches().empty())
    {
        UI::warning("No niches linked.");
        return;
    }

    for(const auto& niche :
        data.getNiches())
    {
        std::cout
            << "  • "
            << niche.getType()
            << "\n";
    }
}



/* =====================================================
   TARGET AUDIENCE
===================================================== */

void CreatorRenderUI::displayTargetAudience(
    const Creator& data
)
{
    printSectionHeader(
        "TARGET AUDIENCE"
    );

    if(data.getTargetAudience().empty())
    {
        UI::warning("No target audience linked.");
        return;
    }

    for(const auto& audience :
        data.getTargetAudience())
    {
        std::cout
            << "  • "
            << audience.getName()
            << "\n";
    }
}



/* =====================================================
   PROFILE ACCOUNTS
===================================================== */

void CreatorRenderUI::displayProfileAccounts(
    const Creator& data,
    const std::vector<Platform>& platforms
)
{
    printSectionHeader(
        "PROFILE ACCOUNTS"
    );

    if(data.getProfileAccounts().empty())
    {
        UI::warning("No profile accounts available.");
        return;
    }

    for(const auto& acc :
        data.getProfileAccounts())
    {
        std::string platformName = "-";

        for(const auto& p : platforms)
        {
            if(
                p.getId() ==
                acc.getPlatformId()
            )
            {
                platformName =
                    p.getName();

                break;
            }
        }

        std::cout
            << "\n----------------------------------\n";

        std::cout
            << "  ["
            << acc.getId()
            << "]\n";

        std::cout
            << "      Platform   : "
            << platformName
            << "\n";

        std::cout
            << "      Link       : "
            << acc.getLink()
            << "\n";

        std::cout
            << "      Followers  : ";

        if(acc.getFollowers().has_value())
        {
            std::cout
                << acc.getFollowers()
                       .value();
        }
        else
        {
            std::cout << "-";
        }

        std::cout << "\n";
    }
}



/* =====================================================
   HPV AUDITS
===================================================== */

void CreatorRenderUI::displayHPVAudits(
    const Creator& data
)
{
    printSectionHeader(
        "HPV AUDITS"
    );

    if(data.getHPVAudits().empty())
    {
        UI::warning("No HPV audits available.");
        return;
    }

    for(const auto& hpv :
        data.getHPVAudits())
    {
        std::cout
            << "\n----------------------------------\n";

        std::cout
            << "  ["
            << hpv.getId()
            << "]\n";

        std::cout
            << "      Topic      : "
            << hpv.getTopic()
                   .value_or("-")
            << "\n";

        std::cout
            << "      Spike Rate : ";

        if(hpv.getSpikeRate().has_value())
        {
            std::cout
                << hpv.getSpikeRate()
                       .value();
        }
        else
        {
            std::cout << "-";
        }

        std::cout << "\n";
    }
}



/* =====================================================
   STRATEGIC NOTES
===================================================== */

void CreatorRenderUI::displayStrategicNotes(
    const Creator& data
)
{
    printSectionHeader(
        "STRATEGIC NOTES"
    );

    if(data.getStrategicNotes().empty())
    {
        UI::warning("No strategic notes available.");
        return;
    }

    for(const auto& note :
        data.getStrategicNotes())
    {
        std::cout
            << "\n----------------------------------\n";

        std::cout
            << "  ["
            << note.getId()
            << "]\n";

        std::cout
            << "      Description : "
            << note.getDescription()
                   .value_or("-")
            << "\n";

        std::cout
            << "      Author      : "
            << note.getAuthor()
            << "\n";
    }
}



/* =====================================================
   VIDEO L10
===================================================== */

void CreatorRenderUI::displayVideos(
    const Creator& data
)
{
    printSectionHeader(
        "VIDEO PERFORMANCE"
    );

    if(data.getVideoPerformance().empty())
    {
        UI::warning("No video performance data available.");
        return;
    }

    for(const auto& vid :
        data.getVideoPerformance())
    {
        std::cout
            << "\n----------------------------------\n";

        std::cout
            << "  ["
            << vid.getId()
            << "]\n";

        std::cout
            << "      Consistency : "
            << vid.getConsistency()
                   .value_or("-")
            << "\n";

        std::cout
            << "      Avg Views   : ";

        if(vid.getAvgViews().has_value())
        {
            std::cout
                << vid.getAvgViews()
                       .value();
        }
        else
        {
            std::cout << "-";
        }

        std::cout << "\n";

        std::cout
            << "      Avg ER      : ";

        if(vid.getAvgER().has_value())
        {
            std::cout
                << vid.getAvgER()
                       .value();
        }
        else
        {
            std::cout << "-";
        }

        std::cout << "\n";

        std::cout
            << "      Sharability : "
            << vid.getSharability()
                   .value_or("-")
            << "\n";

        std::cout
            << "      Velocity    : "
            << vid.getVelocityTrend()
                   .value_or("-")
            << "\n";
    }
}



/* =====================================================
   VERDICT
===================================================== */

void CreatorRenderUI::displayVerdict(
    const Creator& data
)
{
    printSectionHeader(
        "VERDICT"
    );

    if(!data.getVerdict().has_value())
    {
        UI::warning("No verdict available.");
        return;
    }

    const auto& verdict =
        data.getVerdict()
            .value();

    std::cout
        << "  Final Verdict : "
        << verdict.getFinalVerdict()
               .value_or("-")
        << "\n";

    std::cout
        << "  Strengths     : "
        << verdict.getStrengths()
               .value_or("-")
        << "\n";

    std::cout
        << "  Weaknesses    : "
        << verdict.getWeaknesses()
               .value_or("-")
        << "\n";

    std::cout
        << "  Growth        : "
        << verdict.getGrowthPotential()
               .value_or("-")
        << "\n";
}



/* =====================================================
   MAIN RENDER
===================================================== */

void CreatorRenderUI::render(
    const Creator& data
)
{
    auto platforms =
        referencesService.getAllPlatforms(
            currentSession
        );

    UI::title("CREATOR DETAILS");

    displayCreatorCore(data);

    displayCommunity(data);

    displayNiches(data);

    displayTargetAudience(data);

    displayProfileAccounts(data, platforms);

    displayHPVAudits(data);

    displayStrategicNotes(data);

    displayVideos(data);

    displayVerdict(data);
}