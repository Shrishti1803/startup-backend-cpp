#pragma once

#include <vector>
#include <optional>
#include "Db/DbManager.h"
#include "Models/References/Genre.h"

class GenreRepository {

private:
    DbManager& dbManager;

public:
    GenreRepository(DbManager& db);

    // CREATE
    int insert(const std::string& category);

    // READ
    Genre getById(int genreId);
    std::vector<Genre> getAll();

    // UPDATE (PATCH)
    void update(int genreId,
                const std::optional<std::string>& category);

    // DELETE (soft)
    void softDelete(int genreId);
    
};