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
	sf::VertexArray _bufferDisplay;
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
	void fillQuad(sf::VertexArray& buffer, int posStart, const sf::Color& color);
};

template<class T, size_t Rows, size_t Colums>
inline void GraphicsChip8::fillBufferByArray(std::array<std::array<T, Colums>, Rows>& array)
{
	for (int y = 0; y < _localHeight; y++)
	{
		for (int x = 0; x < _localWidth; x++)
		{
			int offset = (y * _localWidth +x) * 4;

			if (array[y][x] == 1)
				fillQuad(_bufferDisplay, offset, sf::Color::White);
			else
				fillQuad(_bufferDisplay, offset, sf::Color::Black);
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
