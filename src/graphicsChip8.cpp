#include "../include/graphicsChip8.h"

GraphicsChip8::GraphicsChip8(sf::RenderWindow& window, int localHeight, int localWidth, int sizePixel) :
	_bufferDisplay(localHeight, std::vector<sf::RectangleShape>()), _window(window), _sizePixel(sizePixel)
{
	_localHeight = localHeight;
	_localWidth = localWidth;
	_window.clear();
	initBuffer();
}

void GraphicsChip8::initBuffer() noexcept
{
	for (int y = 0; y < _localHeight; y++)
	{
		for (int x = 0; x < _localWidth; x++)
		{
			sf::RectangleShape shapeRect(sf::Vector2f(_sizePixel, _sizePixel));
			shapeRect.setPosition(x * _sizePixel, y * _sizePixel);
			shapeRect.setFillColor(sf::Color::Black);
			_bufferDisplay[y].push_back(shapeRect);
		}
	}
}

void GraphicsChip8::draw()
{
	for (int y = 0; y < _localHeight; y++)
	{
		for (int x = 0; x < _localWidth; x++)
		{
			sf::RectangleShape& shape = _bufferDisplay[y][x];
			if (shape.getFillColor() == sf::Color::Black)
				continue;
			_window.draw(shape);
		}
	}
}

void GraphicsChip8::show()
{
	_window.display();
}

void GraphicsChip8::clear(const sf::Color& color)
{
	_window.clear(color);
}
