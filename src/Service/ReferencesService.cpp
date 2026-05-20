#include "Service/ReferencesService.h"
#include "Utils/Logger.h"

#include <stdexcept>
using namespace Auth;


// ================= FULL CONSTRUCTOR =================
ReferencesService::ReferencesService(
    GenreRepository& genreRepo,
    TargetAudienceRepository& targetRepo,
    NicheRepository& nicheRepo,
    PlatformRepository& platformRepo,
    PositionRepository& positionRepo
)
: genreRepo(&genreRepo),
  targetRepo(&targetRepo),
  nicheRepo(&nicheRepo),
  platformRepo(&platformRepo),
  positionRepo(&positionRepo)
{
}


// ================= BRAND ONLY =================
ReferencesService::ReferencesService(
    GenreRepository& genreRepo,
    TargetAudienceRepository& targetRepo,
    PositionRepository& positionRepo
)
: genreRepo(&genreRepo),
  targetRepo(&targetRepo),
  nicheRepo(nullptr),
  platformRepo(nullptr),
  positionRepo(&positionRepo)
{
}


// ================= CREATOR ONLY =================
ReferencesService::ReferencesService(
    NicheRepository& nicheRepo,
    PlatformRepository& platformRepo
)
    : genreRepo(nullptr),
      targetRepo(nullptr),
      nicheRepo(&nicheRepo),
      platformRepo(&platformRepo),
      positionRepo(nullptr)
{
}


// ================= GENRE =================

int ReferencesService::createGenre(
    const Session& session,
    const std::string& category
)
{
    if (!genreRepo) throw std::runtime_error("GenreRepo not initialized");

    if (!RBAC::canCreate(session)) {
        throw std::runtime_error("Permission denied: cannot create genre");
    }

    Logger::db()->info(
        "User {} creating genre",
        session.user_id
    );

    for(const auto& genre :
        genreRepo->getAll())
    {
        if(
            genre.getCategory() ==
            category
        )
        {
            throw std::runtime_error(
                "Genre already exists."
            );
        }
    }

    return genreRepo->insert(category);
}


std::vector<Genre> ReferencesService::getAllGenres(
    const Session& session
)
{
    if (!genreRepo) throw std::runtime_error("GenreRepo not initialized");

    if (!RBAC::canView(session)) {
        throw std::runtime_error("Permission denied: cannot view genres");
    }

    return genreRepo->getAll();
}


void ReferencesService::updateGenre(
    const Session& session,
    int genreId,
    const std::optional<std::string>& category,
    [[maybe_unused]] const std::optional<std::string>& description
)
{
    if (!genreRepo) throw std::runtime_error("GenreRepo not initialized");

    if (!RBAC::canUpdate(session)) {
        throw std::runtime_error("Permission denied: cannot update genre");
    }

    Logger::db()->info("User {} updating genre {}", session.user_id, genreId);

    genreRepo->update(genreId, category);
}


void ReferencesService::deleteGenre(
    const Session& session,
    int genreId
)
{
    if (!genreRepo) throw std::runtime_error("GenreRepo not initialized");

    if (!RBAC::canDelete(session)) {
        throw std::runtime_error("Permission denied: cannot delete genre");
    }

    Logger::db()->info("User {} deleting genre {}", session.user_id, genreId);

    genreRepo->softDelete(genreId);
}


// ================= TARGET AUDIENCE =================

int ReferencesService::createTargetAudience(
    const Session& session,
    const std::string& name
)
{
    if (!targetRepo) throw std::runtime_error("TargetRepo not initialized");

    if (!RBAC::canCreate(session)) {
        throw std::runtime_error("Permission denied: cannot create target audience");
    }

    Logger::db()->info(
        "User {} creating target audience",
        session.user_id
    );

    for(const auto& audience :
        targetRepo->getAll())
    {
        if(
            audience.getName() ==
            name
        )
        {
            throw std::runtime_error(
                "Target audience already exists."
            );
        }
    }

    return targetRepo->insert(name);
}


std::vector<TargetAudience> ReferencesService::getAllTargetAudience(
    const Session& session
)
{
    if (!targetRepo) throw std::runtime_error("TargetRepo not initialized");

    if (!RBAC::canView(session)) {
        throw std::runtime_error("Permission denied: cannot view target audience");
    }

    return targetRepo->getAll();
}


void ReferencesService::updateTargetAudience(
    const Session& session,
    int audId,
    const std::optional<std::string>& name
)
{
    if (!targetRepo) throw std::runtime_error("TargetRepo not initialized");

    if (!RBAC::canUpdate(session)) {
        throw std::runtime_error("Permission denied: cannot update target audience");
    }

    Logger::db()->info("User {} updating target audience {}", session.user_id, audId);

    targetRepo->update(audId, name);
}


void ReferencesService::deleteTargetAudience(
    const Session& session,
    int audId
)
{
    if (!targetRepo) throw std::runtime_error("TargetRepo not initialized");

    if (!RBAC::canDelete(session)) {
        throw std::runtime_error("Permission denied: cannot delete target audience");
    }

    Logger::db()->info("User {} deleting target audience {}", session.user_id, audId);

    targetRepo->softDelete(audId);
}


// ================= NICHE =================

int ReferencesService::createNiche(
    const Session& session,
    const std::string& nicheType
)
{
    if (!nicheRepo) throw std::runtime_error("NicheRepo not initialized");

    if (!RBAC::canCreate(session)) {
        throw std::runtime_error("Permission denied: cannot create niche");
    }
    Logger::db()->info(
        "User {} creating niche",
        session.user_id
    );

    for(const auto& niche :
        nicheRepo->getAll())
    {
        if(
            niche.getType() ==
            nicheType
        )
        {
            throw std::runtime_error(
                "Niche already exists."
            );
        }
    }

    return nicheRepo->insert(nicheType);
}


std::vector<Niche> ReferencesService::getAllNiches(
    const Session& session
)
{
    if (!nicheRepo) throw std::runtime_error("NicheRepo not initialized");

    if (!RBAC::canView(session)) {
        throw std::runtime_error("Permission denied: cannot view niches");
    }

    return nicheRepo->getAll();
}


void ReferencesService::updateNiche(
    const Session& session,
    int nicheId,
    const std::optional<std::string>& nicheType
)
{
    if (!nicheRepo) throw std::runtime_error("NicheRepo not initialized");

    if (!RBAC::canUpdate(session)) {
        throw std::runtime_error("Permission denied: cannot update niche");
    }

    Logger::db()->info("User {} updating niche {}", session.user_id, nicheId);

    nicheRepo->update(nicheId, nicheType);
}


void ReferencesService::deleteNiche(
    const Session& session,
    int nicheId
)
{
    if (!nicheRepo) throw std::runtime_error("NicheRepo not initialized");

    if (!RBAC::canDelete(session)) {
        throw std::runtime_error("Permission denied: cannot delete niche");
    }

    Logger::db()->info("User {} deleting niche {}", session.user_id, nicheId);

    nicheRepo->softDelete(nicheId);
}


// ================= PLATFORM =================

int ReferencesService::createPlatform(
    const Session& session,
    const std::string& name
)
{
    
    if (!platformRepo) throw std::runtime_error("PlatformRepo not initialized");

    if (!RBAC::canCreate(session)) {
        throw std::runtime_error("Permission denied: cannot create platform");
    }
    Logger::db()->info(
        "User {} creating platform",
        session.user_id
    );

    for(const auto& platform :
        platformRepo->getAll())
    {
        if(
            platform.getName() ==
            name
        )
        {
            throw std::runtime_error(
                "Platform already exists."
            );
        }
    }

    return platformRepo->insert(name);
}


std::vector<Platform> ReferencesService::getAllPlatforms(
    const Session& session
)
{
    if (!platformRepo) throw std::runtime_error("PlatformRepo not initialized");

    if (!RBAC::canView(session)) {
        throw std::runtime_error("Permission denied: cannot view platforms");
    }

    return platformRepo->getAll();
}


void ReferencesService::updatePlatform(
    const Session& session,
    int platformId,
    const std::optional<std::string>& name
)
{
    if (!platformRepo) throw std::runtime_error("PlatformRepo not initialized");

    if (!RBAC::canUpdate(session)) {
        throw std::runtime_error("Permission denied: cannot update platform");
    }

    Logger::db()->info("User {} updating platform {}", session.user_id, platformId);

    platformRepo->update(platformId, name);
}


void ReferencesService::deletePlatform(
    const Session& session,
    int platformId
)
{
    if (!platformRepo) throw std::runtime_error("PlatformRepo not initialized");

    if (!RBAC::canDelete(session)) {
        throw std::runtime_error("Permission denied: cannot delete platform");
    }

    Logger::db()->info("User {} deleting platform {}", session.user_id, platformId);

    platformRepo->softDelete(platformId);
}

// ================= POSITION =================

int ReferencesService::createPosition(
    const Session& session,
    const std::string& name
)
{
    if (!positionRepo)
        throw std::runtime_error(
            "PositionRepo not initialized"
        );

    if (!RBAC::canCreate(session)) {
        throw std::runtime_error(
            "Permission denied: cannot create position"
        );
    }

    Logger::db()->info(
        "User {} creating position",
        session.user_id
    );

    for(const auto& position :
        positionRepo->getAll())
    {
        if(position.getName() == name)
        {
            throw std::runtime_error(
                "Position already exists."
            );
        }
    }

    return positionRepo->insert(name);
}


std::vector<Position>
ReferencesService::getAllPositions(
    const Session& session
)
{
    if (!positionRepo)
        throw std::runtime_error(
            "PositionRepo not initialized"
        );

    if (!RBAC::canView(session)) {
        throw std::runtime_error(
            "Permission denied: cannot view positions"
        );
    }

    return positionRepo->getAll();
}


void ReferencesService::updatePosition(
    const Session& session,
    int positionId,
    const std::optional<std::string>& name
)
{
    if (!positionRepo)
        throw std::runtime_error(
            "PositionRepo not initialized"
        );

    if (!RBAC::canUpdate(session)) {
        throw std::runtime_error(
            "Permission denied: cannot update position"
        );
    }

    Logger::db()->info(
        "User {} updating position {}",
        session.user_id,
        positionId
    );

    positionRepo->update(
        positionId,
        name
    );
}


void ReferencesService::deletePosition(
    const Session& session,
    int positionId
)
{
    if (!positionRepo)
        throw std::runtime_error(
            "PositionRepo not initialized"
        );

    if (!RBAC::canDelete(session)) {
        throw std::runtime_error(
            "Permission denied: cannot delete position"
        );
    }

    Logger::db()->info(
        "User {} deleting position {}",
        session.user_id,
        positionId
    );

    positionRepo->softDelete(positionId);
}