#pragma once
#include <fstream>
#include <vector>
#include <exception>

class GameReader
{
private:
	std::vector<uint8_t> _bytes;
public:
	GameReader() = default;
	~GameReader() = default;
	GameReader(const GameReader&) = delete;
	GameReader& operator=(const GameReader&) = delete;
public:
	bool readGame(const std::string& path) throw (std::exception);
	std::vector<uint8_t>& getGame() noexcept;
};