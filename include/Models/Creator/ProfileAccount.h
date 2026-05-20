#pragma once

#include <string>
#include <optional>

class ProfileAccount {

private:

    int profile_id = 0;

    int creator_id = 0;

    int platform_id = 0;

    std::string link;

    std::optional<long long> followers;

public:

    ProfileAccount() = default;

    ProfileAccount(
        int profileId,
        int creatorId,
        int platformId,
        const std::string& profileLink,
        const std::optional<long long>& followerCount = std::nullopt
    )
        : profile_id(profileId),
          creator_id(creatorId),
          platform_id(platformId),
          link(profileLink),
          followers(followerCount)
    {
    }

    // ================= GETTERS =================

    int getId() const
    {
        return profile_id;
    }

    int getCreatorId() const
    {
        return creator_id;
    }

    int getPlatformId() const
    {
        return platform_id;
    }

    const std::string& getLink() const
    {
        return link;
    }

    const std::optional<long long>& getFollowers() const
    {
        return followers;
    }

    // ================= SETTERS =================

    void setId(int id)
    {
        profile_id = id;
    }

    void setCreatorId(int creatorId)
    {
        creator_id = creatorId;
    }

    void setPlatformId(int platformId)
    {
        platform_id = platformId;
    }

    void setLink(const std::string& profileLink)
    {
        link = profileLink;
    }

    void setFollowers(
        const std::optional<long long>& followerCount
    )
    {
        followers = followerCount;
    }
};