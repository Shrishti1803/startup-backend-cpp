#pragma once

#include <string>

#include "Db/Brand/BrandAggregateRepository.h"
#include "CLI/UI/TerminalLayout.h"
class BrandRenderer
{
public:

    static void printSectionHeader(
        const std::string& title
    );

    void displayCompactHeader(const BrandAggregate& data);

    static void displayBrandHeader(
        const BrandAggregate& data
    );

    static void displayBrandCore(
        const BrandAggregate& data
    );

    static void displayGoals(
        const BrandAggregate& data
    );

    static void displayHeads(
        const BrandAggregate& data
    );

    static void displayCompetitors(
        const BrandAggregate& data
    );

    static void displayRevenue(
        const BrandAggregate& data
    );

    static void displayGenres(
        const BrandAggregate& data
    );

    static void displayAudience(
        const BrandAggregate& data
    );

    static void displayInsights(
        const BrandAggregate& data
    );

    static void displayStandard(
        const BrandAggregate& data
    );

    static void displayNewsFunding(
        const BrandAggregate& data
    );
    
    static void displayPastInfluencerCamps(
        const BrandAggregate& data
    );
};