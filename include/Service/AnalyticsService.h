#pragma once

#include "Db/DbManager.h"
#include "Db/Analytics/AnalyticsRepository.h"

class AnalyticsService
{
private:
    DbManager& db;
    AnalyticsRepository analyticsRepository;

public:
    AnalyticsService(DbManager& db);

    void showMonthlyDealsGraph();
};