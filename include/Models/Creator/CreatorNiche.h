#pragma once

#include "../References/Niche.h"

class CreatorNiche {
private:
    int creator_id= 0;
    Niche niche;

public:

    CreatorNiche() = default;

    CreatorNiche(int creatorId, const Niche& n)
        : creator_id(creatorId),
          niche(n) {}

    int getId() const { return creator_id; }

    const Niche& getNiche() const { return niche; }

    void setNiche(const Niche& n){
        niche = n;
    }
};