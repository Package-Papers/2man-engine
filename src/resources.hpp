
#include "imgui.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <unordered_map>

namespace textures
{
enum class ID
{
    Landscape,
    Airplane
};
}
namespace fonts
{
enum class ID
{
    Aerial
};
}

namespace resource_holder
{
template <typename Resource, typename Identifier>
class ResourceHolder
{
  public:
    std::unordered_map<Identifier, std::unique_ptr<Resource>> m_resource_map;

    void load(Identifier id, const std::string& filename)
    {
        auto resource_to_add = std::make_unique<Resource>();
        if (!resource_to_add->loadFromFile(filename))
            throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

        auto inserted = m_resource_map.insert(std::make_pair(id, std::move(resource_to_add)));
        assert(inserted.second);
    }

    Resource& get(Identifier id)
    {
        auto find = m_resource_map.find(id);
        assert(find != m_resource_map.end());
        return *find->second;
    }
    const Resource& get(Identifier id) const
    {
        auto find = m_resource_map.find(id);
        assert(find != m_resource_map.end());
        return *find->second;
    }
};
using TextureHolder = ResourceHolder<sf::Texture, textures::ID>;
using FontHolder    = ResourceHolder<sf::Font, fonts::ID>;

} // namespace resource_holder