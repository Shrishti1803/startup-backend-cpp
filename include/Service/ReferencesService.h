#pragma once

#include "Db/Reference/Brand/GenreRepository.h"
#include "Db/Reference/Brand/TargetAudienceRepository.h"
#include "Db/Brand/PositionRepository.h"
#include "Db/Reference/Creator/NicheRepository.h"
#include "Db/Reference/Creator/PlatformRepository.h"


#include "Auth/AuthService.h"
#include "Auth/RBAC.h"

#include "Utils/Logger.h"

#include <vector>
#include <optional>

class ReferencesService {

private:

    GenreRepository* genreRepo;
    TargetAudienceRepository* targetRepo;
    NicheRepository* nicheRepo;
    PlatformRepository* platformRepo;
    PositionRepository* positionRepo;

public:

    ReferencesService(
        GenreRepository& genreRepo,
        TargetAudienceRepository& targetRepo,
        NicheRepository& nicheRepo,
        PlatformRepository& platformRepo,
        PositionRepository& positionRepo
    );

    // (Brand references only)
    ReferencesService(
        GenreRepository& genreRepo,
        TargetAudienceRepository& targetRepo,
        PositionRepository& positionRepo
    );

    // (Creator references only)
    ReferencesService(
        NicheRepository& nicheRepo,
        PlatformRepository& platformRepo
    );


    // ================= GENRE =================

    int createGenre(
        const Auth::Session& session,
        const std::string& category
    );

    std::vector<Genre> getAllGenres(
        const Auth::Session& session
    );

    void updateGenre(
        const Auth::Session& session,
        int genreId,
        const std::optional<std::string>& category,
        const std::optional<std::string>& description
    );

    void deleteGenre(
        const Auth::Session& session,
        int genreId
    );


    // ================= TARGET AUDIENCE =================

    int createTargetAudience(
        const Auth::Session& session,
        const std::string& name
    );

    std::vector<TargetAudience> getAllTargetAudience(
        const Auth::Session& session
    );

    void updateTargetAudience(
        const Auth::Session& session,
        int audId,
        const std::optional<std::string>& name
    );

    void deleteTargetAudience(
        const Auth::Session& session,
        int audId
    );


    // ================= NICHE =================

    int createNiche(
        const Auth::Session& session,
        const std::string& nicheType
    );

    std::vector<Niche> getAllNiches(
        const Auth::Session& session
    );

    void updateNiche(
        const Auth::Session& session,
        int nicheId,
        const std::optional<std::string>& nicheType
    );

    void deleteNiche(
        const Auth::Session& session,
        int nicheId
    );


    // ================= PLATFORM =================

    int createPlatform(
        const Auth::Session& session,
        const std::string& name
    );

    std::vector<Platform> getAllPlatforms(
        const Auth::Session& session
    );

    void updatePlatform(
        const Auth::Session& session,
        int platformId,
        const std::optional<std::string>& name
    );

    void deletePlatform(
        const Auth::Session& session,
        int platformId
    );
    // ================= POSITION =================

    int createPosition(
        const Auth::Session& session,
        const std::string& name
    );

    std::vector<Position> getAllPositions(
        const Auth::Session& session
    );

    void updatePosition(
        const Auth::Session& session,
        int positionId,
        const std::optional<std::string>& name
    );

    void deletePosition(
        const Auth::Session& session,
        int positionId
    );
};