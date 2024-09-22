#include "../include/gameReader.h"

bool GameReader::readGame(const std::string& path)
{
	std::ifstream game(path, std::ios::binary | std::ios::in);

	if (!game.is_open())
		return false;
	
	uint8_t byte;
	_bytes.clear();

	while (game.read(reinterpret_cast<char*>(&byte), sizeof(byte)))
	{
		_bytes.push_back(byte);
	}

	if (!game.eof())
		return false;


	return true;
}

std::vector<uint8_t>& GameReader::getGame() noexcept
{
	return _bytes;
}
