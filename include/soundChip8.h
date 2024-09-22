#pragma once
#include "SFML/Audio.hpp"
#include <exception>

class SoundChip8
{
private:
	sf::SoundBuffer _buffer;
	sf::Sound _sound;
public:
	SoundChip8(const std::string& fileName) throw (std::exception);
public:
	void play();
	void setVolume(int volume);
};