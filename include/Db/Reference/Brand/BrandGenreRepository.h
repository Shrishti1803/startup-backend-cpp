#pragma once

#include <vector>
#include "Db/DbManager.h"
#include "Models/References/Genre.h"

class BrandGenreRepository {

private:
    DbManager& dbManager;

public:
    BrandGenreRepository(DbManager& db);

    // CREATE
    void insert(int brandId, int genreId);

    // READ (JOIN)
    std::vector<Genre> getByBrandId(int brandId);

    // DELETE (soft)
    void softDelete(int brandId, int genreId);
    
};