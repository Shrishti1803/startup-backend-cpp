#pragma once

#include <string>
#include <optional>

class Verdict {

private:

    int verdict_id=0;

    std::optional<std::string> strengths;
    std::optional<std::string> weaknesses;
    std::optional<std::string> growth_potential;
    std::optional<std::string> final_verdict;

public:

    Verdict() = default;

    Verdict(
        int id,
        const std::optional<std::string>& s = std::nullopt,
        const std::optional<std::string>& w = std::nullopt,
        const std::optional<std::string>& g = std::nullopt,
        const std::optional<std::string>& f = std::nullopt)
        : verdict_id(id),
          strengths(s),
          weaknesses(w),
          growth_potential(g),
          final_verdict(f) {}

    int getId() const { return verdict_id; }

    const std::optional<std::string>& getStrengths() const {
        return strengths;
    }

    const std::optional<std::string>& getWeaknesses() const {
        return weaknesses;
    }

    const std::optional<std::string>& getGrowthPotential() const {
        return growth_potential;
    }

    const std::optional<std::string>& getFinalVerdict() const {
        return final_verdict;
    }

    //Setters 
    void setId(int id)
    {
        verdict_id = id;
    }

    void setStrengths(
        const std::optional<std::string>& s
    )
    {
        strengths = s;
    }

    void setWeaknesses(
        const std::optional<std::string>& w
    )
    {
        weaknesses = w;
    }

    void setGrowthPotential(
        const std::optional<std::string>& g
    )
    {
        growth_potential = g;
    }

    void setFinalVerdict(
        const std::optional<std::string>& f
    )
    {
        final_verdict = f;
    }
};