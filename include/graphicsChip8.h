#pragma once
#include <vector>
#include <array>
#include "SFML/Graphics.hpp"

class GraphicsChip8
{
private:
	sf::RenderWindow& _window;
	int _height, _width;
	int _localHeight, _localWidth;
	int _sizePixel;
	std::vector<std::vector<sf::RectangleShape>> _bufferDisplay;
public:
	GraphicsChip8(sf::RenderWindow& window, int localHeight, int localWidth, int sizePixel);
public:
	void show();
	void clear(const sf::Color& color);
	void draw();
	template<class T, size_t Rows, size_t Colums>
	void fillBufferByArray(std::array<std::array<T, Colums>, Rows>& array);
	template<class T, size_t Rows, size_t Colums>
	void drawForCycle(std::array<std::array<T, Colums>, Rows>& array);
private:
	void initBuffer() noexcept;
};

template<class T, size_t Rows, size_t Colums>
inline void GraphicsChip8::fillBufferByArray(std::array<std::array<T, Colums>, Rows>& array)
{
	for (int y = 0; y < _localHeight; y++)
	{
		for (int x = 0; x < _localWidth; x++)
		{
			sf::RectangleShape& shape = _bufferDisplay[y][x];

			if (array[y][x] != 0)
				shape.setFillColor(sf::Color::White);
			else
				shape.setFillColor(sf::Color::Black);
		}
	}
}

template<class T, size_t Rows, size_t Colums>
inline void GraphicsChip8::drawForCycle(std::array<std::array<T, Colums>, Rows>& array)
{
	fillBufferByArray<T, Rows, Colums>(array);
	clear(sf::Color::Black);
	draw();
	show();
}
