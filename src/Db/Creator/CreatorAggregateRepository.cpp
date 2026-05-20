#include "Db/Creator/CreatorAggregateRepository.h"
#include "Utils/ActivityDetails.h"
#include "Models/References/TargetAudience.h"

CreatorAggregateRepository::CreatorAggregateRepository(
    CreatorRepository& creator,
    CommunityReachesRepository& community,
    CreatorNicheRepository& niche,
    ProfileAccountRepository& profile,
    HPVAuditRepository& hpv,
    StrategicNoteRepository& strategic,
    VideoL10Repository& video,
    VerdictRepository& verdict,
    CreatorTargetAudienceRepository& targetRepo
)
:
creatorRepo(creator),
communityRepo(community),
nicheRepo(niche),
profileRepo(profile),
hpvRepo(hpv),
strategicRepo(strategic),
videoRepo(video),
verdictRepo(verdict),
creatorTargetRepo(targetRepo)
{
}


// CREATE
int CreatorAggregateRepository::create(
    sql::Connection* conn,
    int userId,
    const CreatorAggregate& data
)
{
    /* ===== CREATOR ===== */
    int creatorId = creatorRepo.insert(
        conn,
        data.creator.getName(),
        data.creator.getEmail(),
        data.creator.getRating(),
        data.creator.getAudienceSize()
    );

    ActivityDetails::logActivity(
        conn, userId, "CREATE", "CREATOR",
        creatorId, "creator_name", data.creator.getName()
    );

    /* ===== COMMUNITY ===== */
    if (data.community.has_value())
    {
        int commId = communityRepo.insert(conn,
            creatorId,
            data.community->getSentiment(),
            data.community->getReplyRate()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE",
            "COMMUNITY_REACHES", commId, "", "created"
        );
    }

    /* ===== NICHES ===== */
    for (const auto& n : data.niches)
    {
        nicheRepo.insert(
            creatorId,
            n.getNiche().getId()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE",
            "CREATOR_NICHE", creatorId, "", "linked"
        );
    }

    /* ===== PROFILE ACCOUNTS ===== */
    for (const auto& p : data.profileAccounts)
    {
        int id = profileRepo.insert(conn,
            creatorId,
            p.getPlatformId(),
            p.getLink(),
            p.getFollowers()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE",
            "PROFILE_ACCOUNT",
            id,
            "",
            "created"
        );
    }

    /* ===== HPV ===== */
    for (const auto& hpv : data.hpvAudits)
    {
        int id = hpvRepo.insert(conn,
            creatorId,
            hpv.getTopic(),
            hpv.getSpikeRate()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE",
            "HPV_AUDIT", id, "", "created"
        );
    }

    /* ===== NOTES ===== */
    for (const auto& note : data.strategicNotes)
    {
        int id = strategicRepo.insert(conn,
            creatorId,
            note.getDescription(),
            note.getAuthor()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE",
            "STRATEGIC_NOTE", id, "", "created"
        );
    }

    /* ===== VIDEO ===== */
    for (const auto& vid : data.videos)
    {
        int id = videoRepo.insert(conn,
            creatorId,
            vid.getConsistency(),
            vid.getAvgViews(),
            vid.getAvgER(),
            vid.getSharability(),
            vid.getVelocityTrend()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE",
            "VIDEO_L10", id, "", "created"
        );
    }

    /* ===== VERDICT ===== */
    if (data.verdict.has_value())
    {
        int id = verdictRepo.insert(conn,
            creatorId,
            data.verdict->getStrengths(),
            data.verdict->getWeaknesses(),
            data.verdict->getGrowthPotential(),
            data.verdict->getFinalVerdict()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE",
            "VERDICT", id, "", "created"
        );
    }

    /* ===== TARGET AUDIENCE ===== */
    for (const auto& aud : data.targetAudience)
    {
        creatorTargetRepo.insert(
            creatorId,
            aud.getId()
        );

        ActivityDetails::logActivity(
            conn, userId, "CREATE",
            "CREATOR_TARGET_AUDIENCE",
            creatorId, "", "linked"
        );
    }

    return creatorId;
}

// UPDATE
void CreatorAggregateRepository::update(
    sql::Connection* conn,
    int userId,
    int creatorId,
    const CreatorAggregate& data
)
{
    /* =====================================================
       CREATOR
    ===================================================== */

    if (!data.creator.getName().empty())
    {
        creatorRepo.updateName(
            conn,creatorId,
            std::optional<std::string>(
                data.creator.getName()
            )
        );
    }

    if (
        data.creator.getEmail().has_value()
        ||
        data.clearFields.count(
            "creator.email_id"
        )
    )
    {
        creatorRepo.updateEmail(
            conn,creatorId,
            data.clearFields.count(
                "creator.email_id"
            )
            ? std::nullopt
            : data.creator.getEmail()
        );
    }

    if (
        data.creator.getRating().has_value()
        ||
        data.clearFields.count(
            "creator.rating"
        )
    )
    {
        creatorRepo.updateRating(
            conn,creatorId,
            data.clearFields.count(
                "creator.rating"
            )
            ? std::nullopt
            : data.creator.getRating()
        );
    }

    if (
        data.creator.getAudienceSize()
            .has_value()
        ||
        data.clearFields.count(
            "creator.audience_size"
        )
    )
    {
        creatorRepo.updateAudienceSize(
            conn,creatorId,
            data.clearFields.count(
                "creator.audience_size"
            )
            ? std::nullopt
            : data.creator.getAudienceSize()
        );
    }

    ActivityDetails::logActivity(
        conn,
        userId,
        "UPDATE",
        "CREATOR",
        creatorId,
        "",
        "updated"
    );



    /* =====================================================
       COMMUNITY
    ===================================================== */

    if (data.community.has_value())
    {
        auto existing =
            communityRepo.getByCreatorId(
                creatorId
            );

        if (existing.has_value())
        {
            CommunityReaches merged =
                existing.value();

            if (
                data.community->getSentiment()
                    .has_value()
            )
            {
                merged.setSentiment(
                    data.community
                        ->getSentiment()
                );
            }
            else if (
                data.clearFields.count(
                    "community_reaches.sentiment"
                )
            )
            {
                merged.setSentiment(
                    std::nullopt
                );
            }

            if (
                data.community->getReplyRate()
                    .has_value()
            )
            {
                merged.setReplyRate(
                    data.community
                        ->getReplyRate()
                );
            }
            else if (
                data.clearFields.count(
                    "community_reaches.reply_rate"
                )
            )
            {
                merged.setReplyRate(
                    std::nullopt
                );
            }

            communityRepo.update(conn,
                merged.getId(),
                merged.getSentiment(),
                merged.getReplyRate()
            );

            ActivityDetails::logActivity(
                conn,
                userId,
                "UPDATE",
                "COMMUNITY_REACHES",
                merged.getId(),
                "",
                "updated"
            );
        }
        else
        {
            int id =
                communityRepo.insert(conn,
                    creatorId,
                    data.community
                        ->getSentiment(),
                    data.community
                        ->getReplyRate()
                );

            ActivityDetails::logActivity(
                conn,
                userId,
                "CREATE",
                "COMMUNITY_REACHES",
                id,
                "",
                "created"
            );
        }
    }



    /* =====================================================
       NICHES (M:M SYNC)
    ===================================================== */

    if (!data.niches.empty())
    {
        auto existing =
            nicheRepo.getByCreatorId(
                creatorId
            );

        std::unordered_set<int>
            existingIds;

        for (const auto& n : existing)
        {
            existingIds.insert(
                n.getId()
            );
        }

        std::unordered_set<int>
            incomingIds;

        for (const auto& n :
             data.niches)
        {
            incomingIds.insert(
                n.getNiche().getId()
            );
        }

        // DELETE REMOVED
        for (int id : existingIds)
        {
            if (
                incomingIds.find(id)
                == incomingIds.end()
            )
            {
                nicheRepo.softDelete(
                    creatorId,
                    id
                );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "DELETE",
                    "CREATOR_NICHE",
                    creatorId,
                    "",
                    "removed"
                );
            }
        }

        // INSERT NEW
        for (int id : incomingIds)
        {
            if (
                existingIds.find(id)
                == existingIds.end()
            )
            {
                nicheRepo.insert(
                    creatorId,
                    id
                );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "CREATE",
                    "CREATOR_NICHE",
                    creatorId,
                    "",
                    "linked"
                );
            }
        }
    }



    /* =====================================================
       PROFILE ACCOUNT
    ===================================================== */

    for (const auto& acc :
         data.profileAccounts)
    {
        // ===== UPDATE =====
        if (acc.getId() != 0)
        {
            auto existing =
                profileRepo.getById(
                    acc.getId()
                );

            ProfileAccount merged =
                existing;

            merged.setPlatformId(
                acc.getPlatformId()
            );

            if (!acc.getLink().empty())
            {
                merged.setLink(
                    acc.getLink()
                );
            }

            if (
                acc.getFollowers()
                    .has_value()
            )
            {
                merged.setFollowers(
                    acc.getFollowers()
                );
            }
            else if (
                data.clearFields.count(
                    "profile_account.followers"
                )
            )
            {
                merged.setFollowers(
                    std::nullopt
                );
            }

            profileRepo.update(conn,
                merged.getId(),
                merged.getPlatformId(),
                merged.getLink(),
                merged.getFollowers()
            );

            ActivityDetails::logActivity(
                conn,
                userId,
                "UPDATE",
                "PROFILE_ACCOUNT",
                merged.getId(),
                "",
                "updated"
            );
        }

        // ===== INSERT =====
        else
        {
            int id =
                profileRepo.insert(conn,
                    creatorId,
                    acc.getPlatformId(),
                    acc.getLink(),
                    acc.getFollowers()
                );

            ActivityDetails::logActivity(
                conn,
                userId,
                "CREATE",
                "PROFILE_ACCOUNT",
                id,
                "",
                "created"
            );
        }
    }



    /* =====================================================
       TARGET AUDIENCE (M:M SYNC)
    ===================================================== */

    if (!data.targetAudience.empty())
    {
        auto existing =
            creatorTargetRepo
                .getByCreatorId(
                    creatorId
                );

        std::unordered_set<int>
            existingIds(
                existing.begin(),
                existing.end()
            );

        std::unordered_set<int>
            incomingIds;

        for (const auto& a :
             data.targetAudience)
        {
            incomingIds.insert(
                a.getId()
            );
        }

        // DELETE REMOVED
        for (int id : existingIds)
        {
            if (
                incomingIds.find(id)
                == incomingIds.end()
            )
            {
                creatorTargetRepo
                    .softDelete(
                        creatorId,
                        id
                    );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "DELETE",
                    "CREATOR_TARGET_AUDIENCE",
                    creatorId,
                    "",
                    "removed"
                );
            }
        }

        // INSERT NEW
        for (int id : incomingIds)
        {
            if (
                existingIds.find(id)
                == existingIds.end()
            )
            {
                creatorTargetRepo
                    .insert(
                        creatorId,
                        id
                    );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "CREATE",
                    "CREATOR_TARGET_AUDIENCE",
                    creatorId,
                    "",
                    "linked"
                );
            }
        }
    }



    /* =====================================================
       HPV AUDIT
    ===================================================== */

    for (const auto& hpv :
         data.hpvAudits)
    {
        if (hpv.getId() != 0)
        {
            auto existing =
                hpvRepo.getById(
                    hpv.getId()
                );

            if (existing.has_value())
            {
                HPVAudit merged =
                    existing.value();

                if (
                    hpv.getTopic()
                        .has_value()
                )
                {
                    merged.setTopic(
                        hpv.getTopic()
                    );
                }

                if (
                    hpv.getSpikeRate()
                        .has_value()
                )
                {
                    merged.setSpikeRate(
                        hpv.getSpikeRate()
                    );
                }

                hpvRepo.update(conn,
                    merged.getId(),
                    merged.getTopic(),
                    merged.getSpikeRate()
                );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "UPDATE",
                    "HPV_AUDIT",
                    merged.getId(),
                    "",
                    "updated"
                );
            }
        }
        else
        {
            int id =
                hpvRepo.insert(conn,
                    creatorId,
                    hpv.getTopic(),
                    hpv.getSpikeRate()
                );

            ActivityDetails::logActivity(
                conn,
                userId,
                "CREATE",
                "HPV_AUDIT",
                id,
                "",
                "created"
            );
            continue;
        }
    }



    /* =====================================================
       STRATEGIC NOTES
    ===================================================== */

    for (const auto& note :
         data.strategicNotes)
    {
        if (note.getId() != 0)
        {
            auto existing =
                strategicRepo.getById(
                    note.getId()
                );

            if (existing.has_value())
            {
                StrategicNote merged =
                    existing.value();

                if (
                    note.getDescription()
                        .has_value()
                )
                {
                    merged.setDescription(
                        note.getDescription()
                    );
                }

                if (
                    !note.getAuthor()
                        .empty()
                )
                {
                    merged.setAuthor(
                        note.getAuthor()
                    );
                }

                strategicRepo.update(conn,
                    merged.getId(),
                    merged.getDescription(),
                    merged.getAuthor()
                );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "UPDATE",
                    "STRATEGIC_NOTE",
                    merged.getId(),
                    "",
                    "updated"
                );
            }
        }
        else
        {
            int id =
                strategicRepo.insert(conn,
                    creatorId,
                    note.getDescription(),
                    note.getAuthor()
                );

            ActivityDetails::logActivity(
                conn,
                userId,
                "CREATE",
                "STRATEGIC_NOTE",
                id,
                "",
                "created"
            );
            continue;
        }
    }



    /* =====================================================
       VIDEO
    ===================================================== */

    for (const auto& vid :
         data.videos)
    {
        if (vid.getId() != 0)
        {
            auto existing =
                videoRepo.getById(
                    vid.getId()
                );

            if (existing.has_value())
            {
                VideoL10 merged =
                    existing.value();

                if (
                    vid.getConsistency()
                        .has_value()
                )
                {
                    merged.setConsistency(
                        vid.getConsistency()
                    );
                }

                if (
                    vid.getAvgViews()
                        .has_value()
                )
                {
                    merged.setAvgViews(
                        vid.getAvgViews()
                    );
                }

                if (
                    vid.getAvgER()
                        .has_value()
                )
                {
                    merged.setAvgER(
                        vid.getAvgER()
                    );
                }

                if (
                    vid.getSharability()
                        .has_value()
                )
                {
                    merged.setSharability(
                        vid.getSharability()
                    );
                }

                if (
                    vid.getVelocityTrend()
                        .has_value()
                )
                {
                    merged.setVelocityTrend(
                        vid.getVelocityTrend()
                    );
                }

                videoRepo.update(conn,
                    merged.getId(),
                    merged.getConsistency(),
                    merged.getAvgViews(),
                    merged.getAvgER(),
                    merged.getSharability(),
                    merged.getVelocityTrend()
                );

                ActivityDetails::logActivity(
                    conn,
                    userId,
                    "UPDATE",
                    "VIDEO_L10",
                    merged.getId(),
                    "",
                    "updated"
                );
            }
        }
        else
        {
            int id =
                videoRepo.insert(conn,
                    creatorId,
                    vid.getConsistency(),
                    vid.getAvgViews(),
                    vid.getAvgER(),
                    vid.getSharability(),
                    vid.getVelocityTrend()
                );

            ActivityDetails::logActivity(
                conn,
                userId,
                "CREATE",
                "VIDEO_L10",
                id,
                "",
                "created"
            );
            continue;
        }
    }



    /* =====================================================
       VERDICT
    ===================================================== */

    if (data.verdict.has_value())
    {
        auto existing =
            verdictRepo.getByCreatorId(
                creatorId
            );

        if (existing.has_value())
        {
            Verdict merged =
                existing.value();

            if (
                data.verdict
                    ->getStrengths()
                    .has_value()
            )
            {
                merged.setStrengths(
                    data.verdict
                        ->getStrengths()
                );
            }
            else if (
                data.clearFields.count(
                    "verdict.strengths"
                )
            )
            {
                merged.setStrengths(
                    std::nullopt
                );
            }

            if (
                data.verdict
                    ->getWeaknesses()
                    .has_value()
            )
            {
                merged.setWeaknesses(
                    data.verdict
                        ->getWeaknesses()
                );
            }
            else if (
                data.clearFields.count(
                    "verdict.weaknesses"
                )
            )
            {
                merged.setWeaknesses(
                    std::nullopt
                );
            }

            if (
                data.verdict
                    ->getGrowthPotential()
                    .has_value()
            )
            {
                merged.setGrowthPotential(
                    data.verdict
                        ->getGrowthPotential()
                );
            }
            else if (
                data.clearFields.count(
                    "verdict.growth_potential"
                )
            )
            {
                merged.setGrowthPotential(
                    std::nullopt
                );
            }

            if (
                data.verdict
                    ->getFinalVerdict()
                    .has_value()
            )
            {
                merged.setFinalVerdict(
                    data.verdict
                        ->getFinalVerdict()
                );
            }
            else if (
                data.clearFields.count(
                    "verdict.final_verdict"
                )
            )
            {
                merged.setFinalVerdict(
                    std::nullopt
                );
            }

            verdictRepo.update(conn,
                merged.getId(),
                merged.getStrengths(),
                merged.getWeaknesses(),
                merged.getGrowthPotential(),
                merged.getFinalVerdict()
            );

            ActivityDetails::logActivity(
                conn,
                userId,
                "UPDATE",
                "VERDICT",
                merged.getId(),
                "",
                "updated"
            );
        }
        else
        {
            int id =
                verdictRepo.insert(conn,
                    creatorId,
                    data.verdict
                        ->getStrengths(),
                    data.verdict
                        ->getWeaknesses(),
                    data.verdict
                        ->getGrowthPotential(),
                    data.verdict
                        ->getFinalVerdict()
                );

            ActivityDetails::logActivity(
                conn,
                userId,
                "CREATE",
                "VERDICT",
                id,
                "",
                "created"
            );
        }
    }
}

// DELETE
void CreatorAggregateRepository::softDelete(
    sql::Connection* conn,
    int userId,
    int creatorId
)
{
    /* =====================================================
       COMMUNITY
    ===================================================== */

    auto community =
        communityRepo.getByCreatorId(
            creatorId
        );

    if (community.has_value())
    {
        communityRepo.softDelete(
            conn,community->getId()
        );

        ActivityDetails::logActivity(
            conn,
            userId,
            "DELETE",
            "COMMUNITY_REACHES",
            community->getId(),
            "",
            "deleted"
        );
    }



    /* =====================================================
       NICHES
    ===================================================== */

    auto niches =
        nicheRepo.getByCreatorId(
            creatorId
        );

    for (const auto& n : niches)
    {
        nicheRepo.softDelete(
            creatorId,
            n.getId()
        );

        ActivityDetails::logActivity(
            conn,
            userId,
            "DELETE",
            "CREATOR_NICHE",
            creatorId,
            "",
            "removed"
        );
    }



    /* =====================================================
       PROFILE ACCOUNT
    ===================================================== */

    auto profiles =
        profileRepo.getByCreatorId(
            creatorId
        );

    for (const auto& p : profiles)
    {
        profileRepo.softDelete(
           conn, p.getId()
        );

        ActivityDetails::logActivity(
            conn,
            userId,
            "DELETE",
            "PROFILE_ACCOUNT",
            p.getId(),
            "",
            "deleted"
        );
    }



    /* =====================================================
       HPV AUDIT
    ===================================================== */

    auto hpvs =
        hpvRepo.getByCreatorId(
            creatorId
        );

    for (const auto& h : hpvs)
    {
        hpvRepo.softDelete(
           conn, h.getId()
        );

        ActivityDetails::logActivity(
            conn,
            userId,
            "DELETE",
            "HPV_AUDIT",
            h.getId(),
            "",
            "deleted"
        );
    }



    /* =====================================================
       STRATEGIC NOTE
    ===================================================== */

    auto notes =
        strategicRepo.getByCreatorId(
            creatorId
        );

    for (const auto& n : notes)
    {
        strategicRepo.softDelete(
            conn,n.getId()
        );

        ActivityDetails::logActivity(
            conn,
            userId,
            "DELETE",
            "STRATEGIC_NOTE",
            n.getId(),
            "",
            "deleted"
        );
    }



    /* =====================================================
       VIDEO L10
    ===================================================== */

    auto videos =
        videoRepo.getByCreatorId(
            creatorId
        );

    for (const auto& v : videos)
    {
        videoRepo.softDelete(
            conn,v.getId()
        );

        ActivityDetails::logActivity(
            conn,
            userId,
            "DELETE",
            "VIDEO_L10",
            v.getId(),
            "",
            "deleted"
        );
    }



    /* =====================================================
       VERDICT
    ===================================================== */

    auto verdict =
        verdictRepo.getByCreatorId(
            creatorId
        );

    if (verdict.has_value())
    {
        verdictRepo.softDelete(
            conn,verdict->getId()
        );

        ActivityDetails::logActivity(
            conn,
            userId,
            "DELETE",
            "VERDICT",
            verdict->getId(),
            "",
            "deleted"
        );
    }



    /* =====================================================
       TARGET AUDIENCE
    ===================================================== */

    auto audiences =
        creatorTargetRepo
            .getByCreatorId(
                creatorId
            );

    for (int id : audiences)
    {
        creatorTargetRepo.softDelete(
            creatorId,
            id
        );

        ActivityDetails::logActivity(
            conn,
            userId,
            "DELETE",
            "CREATOR_TARGET_AUDIENCE",
            creatorId,
            "",
            "removed"
        );
    }



    /* =====================================================
       CREATOR
    ===================================================== */

    creatorRepo.softDelete(
       conn, creatorId
    );

    ActivityDetails::logActivity(
        conn,
        userId,
        "DELETE",
        "CREATOR",
        creatorId,
        "",
        "deleted"
    );
}



CreatorAggregate
CreatorAggregateRepository::getById(
    int creatorId
)
{
    CreatorAggregate aggregate;

    /* ===== ROOT ===== */
    aggregate.creator =
        creatorRepo.getById(creatorId);

    /* ===== COMMUNITY ===== */
    auto community =
        communityRepo.getByCreatorId(
            creatorId
        );

    if (community.has_value())
    {
        aggregate.community = community;
    }

    /* ===== VERDICT ===== */
    auto verdict =
        verdictRepo.getByCreatorId(
            creatorId
        );

    if (verdict.has_value())
    {
        aggregate.verdict = verdict;
    }

    /* ===== HPV ===== */
    aggregate.hpvAudits =
        hpvRepo.getByCreatorId(
            creatorId
        );

    /* ===== NOTES ===== */
    aggregate.strategicNotes =
        strategicRepo.getByCreatorId(
            creatorId
        );

    /* ===== VIDEO ===== */
    aggregate.videos =
        videoRepo.getByCreatorId(
            creatorId
        );

    /* ===== PROFILE ACCOUNTS ===== */
    aggregate.profileAccounts =
        profileRepo.getByCreatorId(
            creatorId
        );

    /* ===== NICHES ===== */
    {
        auto niches =
            nicheRepo.getByCreatorId(
                creatorId
            );

        for (const auto& n : niches)
        {
            CreatorNiche cn;

            cn.setNiche(n);

            aggregate.niches.push_back(cn);
        }
    }

    /* ===== TARGET AUDIENCE ===== */
    {
        auto audIds =
            creatorTargetRepo.getByCreatorId(
                creatorId
            );

        for (int id : audIds)
        {
            TargetAudience t;

            t.setId(id);

            aggregate.targetAudience
                .push_back(t);
        }
    }

    // =========================
    // 🔥 SYNC INTO CREATOR
    // =========================

    // ===== 1:M =====

    for (const auto& hpv : aggregate.hpvAudits)
    {
        aggregate.creator.addHPVAudit(
            hpv
        );
    }

    for (const auto& note : aggregate.strategicNotes)
    {
        aggregate.creator.addStrategicNote(
            note
        );
    }

    for (const auto& vid : aggregate.videos)
    {
        aggregate.creator.addVideoPerformance(
            vid
        );
    }

    for (const auto& acc : aggregate.profileAccounts)
    {
        aggregate.creator.addProfileAccount(
            acc
        );
    }

    // ===== 1:1 =====

    if (aggregate.community.has_value())
    {
        aggregate.creator.setCommunityReaches(
            aggregate.community.value()
        );
    }

    if (aggregate.verdict.has_value())
    {
        aggregate.creator.setVerdict(
            aggregate.verdict.value()
        );
    }

    // ===== M:M =====

    for (const auto& n : aggregate.niches)
    {
        aggregate.creator.addNiche(
            n.getNiche()
        );
    }
    for (const auto& a : aggregate.targetAudience)
    {
        aggregate.creator.addTargetAudience(
            a
        );
    }

    return aggregate;
}


void CreatorAggregateRepository::deletePartial(
    sql::Connection* conn,
    int userId,
    int creatorId,
    const CreatorAggregate& data
)
{
    /* =====================================================
       COMMUNITY
    ===================================================== */

    if (data.community.has_value())
    {
        auto existing =
            communityRepo.getByCreatorId(
                creatorId
            );

        if (existing.has_value())
        {
            communityRepo.softDelete(
                conn,existing->getId()
            );

            ActivityDetails::logActivity(
                conn,
                userId,
                "DELETE",
                "COMMUNITY_REACHES",
                existing->getId(),
                "",
                ""
            );
        }
    }



    /* =====================================================
       HPV AUDIT
    ===================================================== */

    for (const auto& hpv :
         data.hpvAudits)
    {
        hpvRepo.softDelete(
            conn,hpv.getId()
        );

        ActivityDetails::logActivity(
            conn,
            userId,
            "DELETE",
            "HPV_AUDIT",
            hpv.getId(),
            "",
            ""
        );
    }



    /* =====================================================
       STRATEGIC NOTES
    ===================================================== */

    for (const auto& note :
         data.strategicNotes)
    {
        strategicRepo.softDelete(
            conn,note.getId()
        );

        ActivityDetails::logActivity(
            conn,
            userId,
            "DELETE",
            "STRATEGIC_NOTE",
            note.getId(),
            "",
            ""
        );
    }



    /* =====================================================
       VIDEO L10
    ===================================================== */

    for (const auto& vid :
         data.videos)
    {
        videoRepo.softDelete(
            conn,vid.getId()
        );

        ActivityDetails::logActivity(
            conn,
            userId,
            "DELETE",
            "VIDEO_L10",
            vid.getId(),
            "",
            ""
        );
    }



    /* =====================================================
       VERDICT
    ===================================================== */

    if (data.verdict.has_value())
    {
        auto existing =
            verdictRepo.getByCreatorId(
                creatorId
            );

        if (existing.has_value())
        {
            verdictRepo.softDelete(
                conn,existing->getId()
            );

            ActivityDetails::logActivity(
                conn,
                userId,
                "DELETE",
                "VERDICT",
                existing->getId(),
                "",
                ""
            );
        }
    }



    /* =====================================================
       PROFILE ACCOUNT
    ===================================================== */

    for (const auto& p :
         data.profileAccounts)
    {
        profileRepo.softDelete(
            conn,p.getId()
        );

        ActivityDetails::logActivity(
            conn,
            userId,
            "DELETE",
            "PROFILE_ACCOUNT",
            p.getId(),
            "",
            ""
        );
    }



    /* =====================================================
       NICHE UNLINK
    ===================================================== */

    for (const auto& n :
         data.niches)
    {
        nicheRepo.softDelete(
            creatorId,
            n.getNiche().getId()
        );

        ActivityDetails::logActivity(
            conn,
            userId,
            "DELETE",
            "CREATOR_NICHE",
            creatorId,
            "",
            ""
        );
    }



    /* =====================================================
       AUDIENCE UNLINK
    ===================================================== */

    for (const auto& a :
         data.targetAudience)
    {
        creatorTargetRepo.softDelete(
            creatorId,
            a.getId()
        );

        ActivityDetails::logActivity(
            conn,
            userId,
            "DELETE",
            "CREATOR_TARGET_AUDIENCE",
            creatorId,
            "",
            ""
        );
    }
}