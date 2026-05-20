#include "Db/Brand/PastInfluencerCampRepository.h"
#include "Utils/Logger.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

PastInfluencerCampRepository::PastInfluencerCampRepository(DbManager& db)
    : dbManager(db) {}

int PastInfluencerCampRepository::insert(
    sql::Connection* conn,
    int brandId,
    const std::optional<std::string>& ambassadors,
    const std::optional<std::string>& campaigns,
    const std::optional<std::string>& strategy)
{
    auto db_logger = Logger::db();
    db_logger->info("Inserting PastInfluencerCamp for brand_id={}", brandId);

    try {

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "INSERT INTO PAST_INFLUENCER_CAMP "
                "(brand_id, ambassadors, campaigns, strategy) "
                "VALUES (?, ?, ?, ?)"
            )
        );

        pstmt->setInt(1, brandId);

        ambassadors.has_value() ? pstmt->setString(2, ambassadors.value())
                                : pstmt->setNull(2, sql::DataType::VARCHAR);

        campaigns.has_value() ? pstmt->setString(3, campaigns.value())
                              : pstmt->setNull(3, sql::DataType::VARCHAR);

        strategy.has_value() ? pstmt->setString(4, strategy.value())
                             : pstmt->setNull(4, sql::DataType::VARCHAR);

        pstmt->executeUpdate();

        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> rs(
            stmt->executeQuery("SELECT LAST_INSERT_ID()")
        );

        if (rs->next())
            return rs->getInt(1);

        return -1;
    }
    catch (sql::SQLException& e) {
        db_logger->error("PastInfluencerCamp insert failed: {}", e.what());
        throw;
    }
}

std::vector<PastInfluencerCamp> 
PastInfluencerCampRepository::getByBrandId(int brandId)
{
    auto db_logger = Logger::db();
    db_logger->info("Fetching PastInfluencerCamp for brand_id={}", brandId);

    std::vector<PastInfluencerCamp> records;

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT p_inf_id, brand_id, ambassadors, campaigns, strategy, is_deleted "
                "FROM PAST_INFLUENCER_CAMP "
                "WHERE brand_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, brandId);

        std::unique_ptr<sql::ResultSet> rs(
            pstmt->executeQuery()
        );

        while (rs->next()) {

            std::optional<std::string> amb, camp, strat;

            if (!rs->isNull("ambassadors"))
                amb = std::string(rs->getString("ambassadors"));

            if (!rs->isNull("campaigns"))
                camp = std::string(rs->getString("campaigns"));

            if (!rs->isNull("strategy"))
                strat = std::string(rs->getString("strategy"));

            PastInfluencerCamp record(
                rs->getInt("p_inf_id"),
                rs->getInt("brand_id"),
                amb,
                camp,
                strat,
                rs->getBoolean("is_deleted")
            );

            records.push_back(record);
        }
    }
    catch (sql::SQLException& e) {
        db_logger->error("PastInfluencerCamp fetch failed: {}", e.what());
        throw;
    }

    return records;
}

void PastInfluencerCampRepository::update(
    sql::Connection* conn,
    int id,
    const std::optional<std::string>& ambassadors,
    const std::optional<std::string>& campaigns,
    const std::optional<std::string>& strategy)
{
    auto db_logger = Logger::db();
    db_logger->info("Updating PastInfluencerCamp id={}", id);

    try {

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE PAST_INFLUENCER_CAMP "
                "SET ambassadors = ?, campaigns = ?, strategy = ? "
                "WHERE p_inf_id = ? AND is_deleted = 0"
            )
        );

        ambassadors.has_value() ? pstmt->setString(1, ambassadors.value())
                                : pstmt->setNull(1, sql::DataType::VARCHAR);

        campaigns.has_value() ? pstmt->setString(2, campaigns.value())
                              : pstmt->setNull(2, sql::DataType::VARCHAR);

        strategy.has_value() ? pstmt->setString(3, strategy.value())
                             : pstmt->setNull(3, sql::DataType::VARCHAR);

        pstmt->setInt(4, id);

        int rowsAffected = pstmt->executeUpdate();

        if (rowsAffected == 0) {
            db_logger->warn("No record found to update with ID {}", id);
            throw std::runtime_error("PastInfluencerCamp not found");
        }

        db_logger->info("PastInfluencerCamp ID {} updated successfully", id);
    }
    catch (sql::SQLException& e) {
        db_logger->error("PastInfluencerCamp update failed: {}", e.what());
        throw;
    }
}

void PastInfluencerCampRepository::softDelete(
    sql::Connection* conn,
    int id)
{
    auto db_logger = Logger::db();
    db_logger->info("Soft deleting PastInfluencerCamp id={}", id);

    try {

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE PAST_INFLUENCER_CAMP "
                "SET is_deleted = 1 "
                "WHERE p_inf_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, id);

        int rowsAffected = pstmt->executeUpdate();

        if (rowsAffected == 0) {
            db_logger->warn("No record found to delete with ID {}", id);
            throw std::runtime_error("PastInfluencerCamp not found");
        }

        db_logger->info("PastInfluencerCamp ID {} soft deleted successfully", id);
    }
    catch (sql::SQLException& e) {
        db_logger->error("PastInfluencerCamp delete failed: {}", e.what());
        throw;
    }
}

std::optional<PastInfluencerCamp>
PastInfluencerCampRepository::getById(
    int picId
)
{
    auto db_logger = Logger::db();

    db_logger->info(
        "Fetching PIC by id={}",
        picId
    );

    try
    {
        auto conn =
            dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                R"(
                    SELECT
                        past_campaign_id,
                        ambassadors,
                        campaigns,
                        strategy
                    FROM PAST_INFLUENCER_CAMP
                    WHERE past_campaign_id = ?
                    AND is_deleted = 0
                )"
            )
        );

        pstmt->setInt(1, picId);

        std::unique_ptr<sql::ResultSet> res(
            pstmt->executeQuery()
        );

        if(res->next())
        {
            PastInfluencerCamp p;

            p.setId(
                res->getInt(
                    "past_campaign_id"
                )
            );

            if(!res->isNull("ambassadors"))
            {
                p.setAmbassadors(
                    std::string(
                        res->getString(
                            "ambassadors"
                        )
                    )
                );
            }

            if(!res->isNull("campaigns"))
            {
                p.setCampaigns(
                    std::string(
                        res->getString(
                            "campaigns"
                        )
                    )
                );
            }

            if(!res->isNull("strategy"))
            {
                p.setStrategy(
                    std::string(
                        res->getString(
                            "strategy"
                        )
                    )
                );
            }

            return p;
        }

        return std::nullopt;
    }
    catch(sql::SQLException& e)
    {
        db_logger->error(
            "PIC getById failed: {}",
            e.what()
        );

        throw;
    }
}