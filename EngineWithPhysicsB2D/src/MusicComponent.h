//comment
#pragma once

#include "IComponent.hpp"

#include <SFML/Audio.hpp>
#include <unordered_map>
namespace mmt_gd
{
class MusicComponent: public IComponent
{
public:
    explicit MusicComponent(GameObject& gameObject) : IComponent(gameObject)
    {
    }
    ~MusicComponent() override = default;
    void playMusic(const std::string& musicName, bool loop = true);
    void stopMusic(const std::string& musicName);
    void addMusic(const std::string& musicName, const std::string& filePath, float volume);
    bool init() override
    {
        return true;
    }
    void update(float deltaTime)
    {
        return;
    }

private:
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> m_musics;
};
} // namespace mmt_gd
