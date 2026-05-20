#pragma once

#include "Db/Creator/CreatorAggregateRepository.h"
#include "Db/Creator/CreatorRepository.h"
#include "Db/DbManager.h"

#include "Db/Creator/CreatorQueryRepository.h"

#include "Models/Creator/Creator.h"

#include "Auth/RBAC.h"
#include "Auth/AuthService.h"

#include "Utils/TransactionGuard.h"

//  Pagination Response
struct CreatorPage {
    std::vector<CreatorResult> data;
    int totalCount;
    int currentPage;
    int totalPages;
};

class CreatorService {

private:
    DbManager& dbManager;
    CreatorAggregateRepository& creatorAggregateRepo;
    CreatorRepository& creatorRepo;
    CreatorQueryRepository& creatorQueryRepo;   

public:

    
    CreatorService(
        DbManager& dbManager,
        CreatorAggregateRepository& creatorAggregateRepo,
        CreatorRepository& creatorRepo,
        CreatorQueryRepository& creatorQueryRepo
    );

    int createCreator(const Auth::Session& session, const CreatorAggregate& data);
    void updateCreator(const Auth::Session& session, int creatorId, const CreatorAggregate& data);
    void deleteCreator(const Auth::Session& session, int creatorId);
    CreatorAggregate getCreatorById(const Auth::Session& session, int creatorId);

    // MAIN VIEW FUNCTION 
    CreatorPage getCreatorsView(
        const Auth::Session& session,
        const std::string& searchQuery = "",
        int minAudienceSize = -1,
        int maxAudienceSize = -1,
        std::vector<int> nicheIds = {},
        const std::string& sortBy = "name",
        const std::string& sortOrder = "ASC",
        int page = 1,
        int limit = 10
    );
    std::vector<Creator> getAllCreators(
        const Auth::Session& session
    );
    void deletePartial(
        const Auth::Session& session,
        int creatorId,
        const CreatorAggregate& data
    );
};