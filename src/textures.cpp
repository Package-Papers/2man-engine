
#include <iostream>
#include <map>
#include "imgui.h"
#include <SFML/Graphics.hpp> 

namespace Textures
{
    enum ID { Landscape, Airplane };
}

class TextureHolder
{

    public:
    void load(Textures::ID id,
    const std::string& filename);
    sf::Texture& get(Textures::ID id);
    const sf::Texture& get(Textures::ID id) const;

    private:
        std::map<Textures::ID,
        std::unique_ptr<sf::Texture>> TexturesMap;

    void TextureHolder::load(Textures::ID id, const std::string &filename)
    {
        // Create a texture in a unique ptr and load the texture given the file name
        std:: unique_ptr<sf::Texture> textureToAdd(new sf::Texture());
        if (!textureToAdd->loadFromFile(filename))
            throw std::runtime_error("TextureHolder::load - Failed to load "
            + filename);

        auto inserted = TexturesMap.insert(std::make_pair(id, std::move(textureToAdd)));
        assert(inserted.second);
    }

    sf::Texture &TextureHolder::get(Textures::ID id) {
        auto found = TexturesMap.find(id);
        assert(found != TexturesMap.end());
        return *found->second;
    }


};