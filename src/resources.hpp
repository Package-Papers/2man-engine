
#include <iostream>
#include <map>
#include "imgui.h"
#include <SFML/Graphics.hpp> 


template <typename Resource, typename Identifier>
class ResourceHolder
{
    public:
        void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string &filename);
        Resource &ResourceHolder<Resource, Identifier>::get(Identifier id);
        const Resource &ResourceHolder<Resource, Identifier>::get(Identifier id) const;
    private:
        std::map<Identifier,
        std::unique_ptr<Resource>> mResourceMap;        

    template <typename Resource, typename Identifier>
    void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string &filename) {
        std:: unique_ptr<Resource> resourceToAdd(new Resource());
        if (!resourceToAdd->loadFromFile(filename))
            throw std::runtime_error("ResourceHolder::load - Failed to load "
            + filename);

        auto inserted = ResourcesMap.insert(std::make_pair(id, std::move(resourceToAdd)));
        assert(inserted.second);
    }

    template <typename Resource, typename Identifier>
    Resource &ResourceHolder<Resource, Identifier>::get(Identifier id) {
        auto find = ResourcesMap.find(id);
        assert(find != ResourcesMap.end());
        return *find->second;
    }
};