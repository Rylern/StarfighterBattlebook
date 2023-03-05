#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SFML/Audio.hpp>
#include <array>


class SoundManager
{

public:
    SoundManager();
    ~SoundManager();
    void handleEvent();
    void playMenuMusic();
    void pauseMenuMusic();
    void explosion(std::string team);
    void fightersComing(std::string team);
    void hit(std::string team);
    void lasers(std::string team);
    void lasersAndExplosion(std::string team);
    void onYourTail(std::string team);
    void flying(std::string team);

private:
    static const unsigned char NB_SOUND_BUFFER = 2;
    static const unsigned char NB_FLYING_SOUNDS = 3;
    static const unsigned char NB_GAME_MUSICS = 13;
    sf::Music menuMusic;
    sf::Music gameMusic;
    bool gameMusicPlaying;
    std::array<sf::SoundBuffer, NB_SOUND_BUFFER> soundBuffers;
    std::array<sf::Sound, NB_SOUND_BUFFER> sounds;
    unsigned char indexSoundBufferFree;

    void playGameMusic();
    void pauseGameMusic();
    void playSound(std::string team, std::string sound);
};

#endif