#pragma once
#include <string>

struct DealView {
    int deal_id=0;
    std::string brand_name;
    std::string creator_name;

    bool is_lead;
    bool is_connected;
    bool is_deal_done;
    bool post_uploaded;
    bool payment_cleared;

    std::string date;
};