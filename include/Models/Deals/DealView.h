#pragma once

#include <string>

struct DealView {

    int deal_id = 0;

    std::string brand_name;
    std::string creator_name;

    bool is_lead;
    std::string lead_date;

    bool is_connected;
    std::string connected_date;

    bool is_deal_done;
    std::string deal_done_date;

    bool post_uploaded;
    std::string post_uploaded_date;

    bool payment_cleared;
    std::string payment_cleared_date;
};