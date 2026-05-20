#pragma once

#include <vector>
#include <optional>
#include <unordered_set>

#include <cppconn/connection.h>

// MODELS

#include "Models/Creator/Creator.h"
#include "Models/Creator/CommunityReaches.h"
#include "Models/Creator/Verdict.h"
#include "Models/Creator/HPVAudit.h"
#include "Models/Creator/StrategicNote.h"
#include "Models/Creator/VideoL10.h"
#include "Models/Creator/ProfileAccount.h"

#include "Models/Creator/CreatorNiche.h"
#include "Models/References/TargetAudience.h"

//REPOSITORIES 

#include "Db/Creator/CreatorRepository.h"
#include "Db/Creator/CommunityReachesRepository.h"
#include "Db/Creator/HPVAuditRepository.h"
#include "Db/Creator/StrategicNoteRepository.h"
#include "Db/Creator/VideoL10Repository.h"
#include "Db/Creator/VerdictRepository.h"
#include "Db/Creator/ProfileAccountRepository.h"

#include "Db/Reference/Creator/CreatorNicheRepository.h"
#include "Db/Reference/Creator/CreatorTargetAudienceRepository.h"

#include "Utils/ActivityDetails.h"



struct CreatorAggregate {

    //  ROOT
    Creator creator;

    // 1:1
    std::optional<CommunityReaches> community;
    std::optional<Verdict> verdict;

    //  1:M 
    std::vector<HPVAudit> hpvAudits;

    std::vector<StrategicNote> strategicNotes;

    std::vector<VideoL10> videos;

    std::vector<ProfileAccount> profileAccounts;

    //  M:M 
    std::vector<CreatorNiche> niches;

    std::vector<TargetAudience> targetAudience;

    //PATCH SUPPORT 
    std::unordered_set<std::string> clearFields;
};



class CreatorAggregateRepository {

private:

    CreatorRepository& creatorRepo;

    CommunityReachesRepository& communityRepo;

    CreatorNicheRepository& nicheRepo;

    ProfileAccountRepository& profileRepo;

    HPVAuditRepository& hpvRepo;

    StrategicNoteRepository& strategicRepo;

    VideoL10Repository& videoRepo;

    VerdictRepository& verdictRepo;

    CreatorTargetAudienceRepository& creatorTargetRepo;

public:

    CreatorAggregateRepository(
        CreatorRepository& creator,
        CommunityReachesRepository& community,
        CreatorNicheRepository& niche,
        ProfileAccountRepository& profile,
        HPVAuditRepository& hpv,
        StrategicNoteRepository& strategic,
        VideoL10Repository& video,
        VerdictRepository& verdict,
        CreatorTargetAudienceRepository& targetRepo
    );



    //  CREATE 

    int create(
        sql::Connection* conn,
        int userId,
        const CreatorAggregate& data
    );



    //  READ 

    CreatorAggregate getById(int creatorId);



    // UPDATE 

    void update(
        sql::Connection* conn,
        int userId,
        int creatorId,
        const CreatorAggregate& data
    );



    // DELETE 

    void softDelete(
        sql::Connection* conn,
        int userId,
        int creatorId
    );

    void deletePartial(
        sql::Connection* conn,
        int userId,
        int creatorId,
        const CreatorAggregate& data
    );
};