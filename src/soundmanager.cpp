#include "soundmanager.hpp"
#include <stdlib.h>
#include "utils.hpp"


SoundManager::SoundManager():   gameMusicPlaying(false),
                                indexSoundBufferFree(0) {
    this->menuMusic.setLoop(true);
    this->menuMusic.setVolume(75);
    this->gameMusic.setVolume(75);

    for (int i=0; i<this->NB_SOUND_BUFFER; i++) {
        this->sounds[i].setBuffer(this->soundBuffers[i]);
    }
    
    srand((unsigned int) time(NULL));
}

SoundManager::~SoundManager() {
}

void SoundManager::handleEvent() {
    if (this->gameMusicPlaying && this->gameMusic.getStatus() == sf::SoundSource::Status::Stopped) {
        this->playGameMusic();
    }
}

void SoundManager::playMenuMusic() {
    if (this->menuMusic.openFromFile(Utils::joinPath({"res", "music", "menu.ogg"}))) {
        this->menuMusic.play();
    }
    this->pauseGameMusic();
}

void SoundManager::pauseMenuMusic() {
    this->menuMusic.stop();
    this->playGameMusic();
}

void SoundManager::playGameMusic() {
    this->gameMusicPlaying = true;

    std::string music = std::to_string(rand() % this->NB_GAME_MUSICS + 1);
    if (this->gameMusic.openFromFile(Utils::joinPath({"res", "music", "game", music + ".ogg"}))) {
        this->gameMusic.play();
    }
}

void SoundManager::pauseGameMusic() {
    this->gameMusicPlaying = false;
    this->gameMusic.stop();
}

void SoundManager::explosion(std::string team) {
    this->playSound(team, "explosion");
}

void SoundManager::fightersComing(std::string team) {
    if (team == "rebellion") {
        this->playSound(team, "fightersComing");
    }
}

void SoundManager::hit(std::string team) {
    if (team == "rebellion") {
        this->playSound(team, "hit");
    }
}

void SoundManager::lasers(std::string team) {
    this->playSound(team, "lasers");
}

void SoundManager::lasersAndExplosion(std::string team) {
    this->playSound(team, "lasersAndExplosion");
}

void SoundManager::onYourTail(std::string team) {
    if (team == "rebellion") {
        this->playSound(team, "onYourTail");
    }
}

void SoundManager::flying(std::string team) {
    std::string sound = std::to_string(rand() % this->NB_FLYING_SOUNDS + 1);
    this->playSound(team, Utils::joinPath({"flying", sound}));
}

void SoundManager::playSound(std::string team, std::string sound) {
    if (this->soundBuffers[this->indexSoundBufferFree].loadFromFile(Utils::joinPath({"res", team, "sounds", sound + ".ogg"}))) {
        this->sounds[this->indexSoundBufferFree].play();
    }
    this->indexSoundBufferFree = (this->indexSoundBufferFree + 1) % this->NB_SOUND_BUFFER;
}