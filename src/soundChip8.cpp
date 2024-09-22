#include "../include/soundChip8.h"

SoundChip8::SoundChip8(const std::string& fileName)
{
	if (!_buffer.loadFromFile(fileName))
	{
		throw std::exception("error loadFile");
	}

	_sound.setBuffer(_buffer);
}

void SoundChip8::play()
{
	_sound.play();
}

void SoundChip8::setVolume(int volume)
{
	_sound.setVolume(volume);
}
