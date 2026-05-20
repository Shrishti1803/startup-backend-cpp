#pragma once

#include <string>
#include <vector>
#include <optional>

// ================= CHILD MODELS =================

#include "CommunityReaches.h"
#include "ProfileAccount.h"
#include "HPVAudit.h"
#include "StrategicNote.h"
#include "VideoL10.h"
#include "Verdict.h"

// ================= REFERENCE MODELS =================

#include "../References/Niche.h"
#include "../References/TargetAudience.h"

class Creator {

private:

    // ================= CORE =================

    int creator_id = 0;

    std::string creator_name;

    std::optional<std::string> email_id;

    std::optional<std::string> rating;

    std::optional<long long> audience_size;

    // ================= 1:1 =================

    std::optional<CommunityReaches> community_reaches;

    std::optional<Verdict> verdict;

    // ================= 1:M =================

    std::vector<HPVAudit> hpv_audits;

    std::vector<StrategicNote> strategic_notes;

    std::vector<VideoL10> video_performance;

    std::vector<ProfileAccount> profile_accounts;

    // ================= M:M =================

    std::vector<Niche> niches;
    std::vector<TargetAudience> target_audience;

public:

    Creator() = default;

    Creator(
        int id,
        const std::string& name,
        const std::optional<std::string>& email = std::nullopt,
        const std::optional<std::string>& rate = std::nullopt,
        const std::optional<long long>& audience = std::nullopt
    )
        : creator_id(id),
          creator_name(name),
          email_id(email),
          rating(rate),
          audience_size(audience)
    {
    }

    // ================= CORE GETTERS =================

    int getId() const
    {
        return creator_id;
    }

    const std::string& getName() const
    {
        return creator_name;
    }

    const std::optional<std::string>& getEmail() const
    {
        return email_id;
    }

    const std::optional<std::string>& getRating() const
    {
        return rating;
    }

    const std::optional<long long>& getAudienceSize() const
    {
        return audience_size;
    }

    // ================= CORE SETTERS =================

    void setId(int id)
    {
        creator_id = id;
    }

    void setName(const std::string& name)
    {
        creator_name = name;
    }

    void setEmail(
        const std::optional<std::string>& email
    )
    {
        email_id = email;
    }

    void setRating(
        const std::optional<std::string>& rate
    )
    {
        rating = rate;
    }

    void setAudienceSize(
        const std::optional<long long>& audience
    )
    {
        audience_size = audience;
    }

    // ================= 1:1 SETTERS =================

    void setCommunityReaches(
        const CommunityReaches& reach
    )
    {
        community_reaches = reach;
    }

    void setVerdict(
        const Verdict& v
    )
    {
        verdict = v;
    }

    // ================= 1:M ADDERS =================

    void addHPVAudit(
        const HPVAudit& audit
    )
    {
        hpv_audits.push_back(audit);
    }

    void addStrategicNote(
        const StrategicNote& note
    )
    {
        strategic_notes.push_back(note);
    }

    void addVideoPerformance(
        const VideoL10& video
    )
    {
        video_performance.push_back(video);
    }

    void addProfileAccount(
        const ProfileAccount& acc
    )
    {
        profile_accounts.push_back(acc);
    }

    // ================= M:M ADDERS =================

    void addNiche(
        const Niche& niche
    )
    {
        niches.push_back(niche);
    }
    void addTargetAudience(
        const TargetAudience& audience
    )
    {
        target_audience.push_back(
            audience
        );
    }

    // ================= 1:M GETTERS =================

    const std::vector<HPVAudit>&
    getHPVAudits() const
    {
        return hpv_audits;
    }

    const std::vector<StrategicNote>&
    getStrategicNotes() const
    {
        return strategic_notes;
    }

    const std::vector<VideoL10>&
    getVideoPerformance() const
    {
        return video_performance;
    }

    const std::vector<ProfileAccount>&
    getProfileAccounts() const
    {
        return profile_accounts;
    }

    // ================= M:M GETTERS =================

    const std::vector<Niche>&
    getNiches() const
    {
        return niches;
    }
    const std::vector<TargetAudience>&
    getTargetAudience() const
    {
        return target_audience;
    }

    // ================= 1:1 GETTERS =================

    const std::optional<CommunityReaches>&
    getCommunityReaches() const
    {
        return community_reaches;
    }

    const std::optional<Verdict>&
    getVerdict() const
    {
        return verdict;
    }
};