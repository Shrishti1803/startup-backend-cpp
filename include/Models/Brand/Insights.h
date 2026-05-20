#pragma once

#include <string>
#include <optional>

class Insights {
private:
    int insights_id=0;
    std::optional<std::string> pitch_angle;
    std::optional<std::string> leverage;
    std::optional<std::string> gap;
    std::optional<std::string> hook;

public:
    Insights() = default;

    Insights(int id,
             const std::optional<std::string>& pitch = std::nullopt,
             const std::optional<std::string>& lev = std::nullopt,
             const std::optional<std::string>& g = std::nullopt,
             const std::optional<std::string>& h = std::nullopt)
        : insights_id(id),
          pitch_angle(pitch),
          leverage(lev),
          gap(g),
          hook(h) {}

    int getId() const { return insights_id; }

    const std::optional<std::string>& getPitchAngle() const { return pitch_angle; }

    const std::optional<std::string>& getLeverage() const { return leverage; }

    const std::optional<std::string>& getGap() const { return gap; }

    const std::optional<std::string>& getHook() const { return hook; }
    
    void setPitchAngle(const std::optional<std::string>& val) { pitch_angle = val; }

    void setLeverage(const std::optional<std::string>& val) { leverage = val; }

    void setGap(const std::optional<std::string>& val) { gap = val; }

    void setHook(const std::optional<std::string>& val) { hook = val; }

    void setId(int id) { insights_id = id; }
};