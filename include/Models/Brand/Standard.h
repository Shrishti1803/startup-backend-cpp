#pragma once

#include <string>
#include <optional>

class BrandStandard {
private:
    int standard_id;
    std::optional<std::string> position;
    std::optional<std::string> brand_identity;
    std::optional<std::string> csr;
    std::optional<std::string> quality;
    std::optional<std::string> perception;

public:
    BrandStandard() = default;

    BrandStandard(int id,
                  const std::optional<std::string>& pos = std::nullopt,
                  const std::optional<std::string>& identity = std::nullopt,
                  const std::optional<std::string>& c = std::nullopt,
                  const std::optional<std::string>& q = std::nullopt,
                  const std::optional<std::string>& p = std::nullopt)
        : standard_id(id),
          position(pos),
          brand_identity(identity),
          csr(c),
          quality(q),
          perception(p) {}

    int getId() const { return standard_id; }

    const std::optional<std::string>& getPosition() const { return position; }

    const std::optional<std::string>& getIdentity() const { return brand_identity; }

    const std::optional<std::string>& getCSR() const { return csr; }

    const std::optional<std::string>& getQuality() const { return quality; }

    const std::optional<std::string>& getPerception() const { return perception; }
};