#pragma once

#include <vector>
#include <optional>
#include "Models/Helpers/Date.h"
#include "Models/Deals/DealView.h"
namespace sql {
    class Connection;
}

//Entity
struct Deal {
    int deal_id;
    int brand_id;
    int creator_id;

    bool is_lead;
    bool is_connected;
    bool is_deal_done;
    bool post_uploaded;
    bool payment_cleared;

    std::string date;
    bool is_deleted;
};


//PATCH struct (only fields to update)
struct DealUpdate {
    std::optional<int> brand_id;
    std::optional<int> creator_id;

    std::optional<bool> is_lead;
    std::optional<bool> is_connected;
    std::optional<bool> is_deal_done;
    std::optional<bool> post_uploaded;
    std::optional<bool> payment_cleared;

    std::optional<std::string> date;
};

class DealRepository {
public:
    // CREATE
    int create(sql::Connection* conn, const Deal& deal);

    // READ
    Deal getById(sql::Connection* conn, int deal_id);

    std::vector<Deal> getByBrandId(sql::Connection* conn, int brand_id);
    std::vector<Deal> getByCreatorId(sql::Connection* conn, int creator_id);
    std::vector<DealView> getDealsWithNames(sql::Connection* conn);

    // UPDATE (PATCH)
    void update(sql::Connection* conn, int deal_id, const DealUpdate& updateData);

    // DELETE (SOFT)
    void softDelete(sql::Connection* conn, int deal_id);
    bool existsById(sql::Connection* conn, int deal_id);
};