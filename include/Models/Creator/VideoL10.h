#pragma once

#include <string>
#include <optional>

class VideoL10 {

private:

    int performance_id=0;

    std::optional<std::string> L10_consistency;
    std::optional<std::string> L10_avg_views;
    std::optional<std::string> L10_avg_ER;
    std::optional<std::string> L10_sharability;
    std::optional<std::string> velocity_trend;

public:

    VideoL10() = default;

    VideoL10(
        int id,
        const std::optional<std::string>& c = std::nullopt,
        const std::optional<std::string>& views = std::nullopt,
        const std::optional<std::string>& er = std::nullopt,
        const std::optional<std::string>& share = std::nullopt,
        const std::optional<std::string>& trend = std::nullopt)
        : performance_id(id),
          L10_consistency(c),
          L10_avg_views(views),
          L10_avg_ER(er),
          L10_sharability(share),
          velocity_trend(trend) {}

    int getId() const { return performance_id; }

    const std::optional<std::string>& getConsistency() const {
        return L10_consistency;
    }

    const std::optional<std::string>& getAvgViews() const {
        return L10_avg_views;
    }

    const std::optional<std::string>& getAvgER() const {
        return L10_avg_ER;
    }

    const std::optional<std::string>& getSharability() const {
        return L10_sharability;
    }

    const std::optional<std::string>& getVelocityTrend() const {
        return velocity_trend;
    }

    //SETTERS 
    void setId(int id)
    {
        performance_id = id;
    }
    void setConsistency(
        const std::optional<std::string>& c
    )
    {
        L10_consistency = c;
    }

    void setAvgViews(
        const std::optional<std::string>& views
    )
    {
        L10_avg_views = views;
    }

    void setAvgER(
        const std::optional<std::string>& er
    )
    {
        L10_avg_ER = er;
    }

    void setSharability(
        const std::optional<std::string>& share
    )
    {
        L10_sharability = share;
    }

    void setVelocityTrend(
        const std::optional<std::string>& trend
    )
    {
        velocity_trend = trend;
    }
};