#include "../include/graphicsChip8.h"

GraphicsChip8::GraphicsChip8(sf::RenderWindow& window, int localHeight, int localWidth, int sizePixel) :
	_bufferDisplay(sf::Quads, localHeight * localWidth * 4), _window(window)
{
	_localHeight = localHeight;
	_localWidth = localWidth;
	_sizePixel = sizePixel;
	_window.clear();

	initBuffer();
}

void GraphicsChip8::initBuffer() noexcept
{
	for (int y = 0; y < _localHeight; y++)
	{
		for (int x = 0; x < _localWidth; x++)
		{	
			int offset = (y * _localWidth + x) * 4;
			
			//left Up
			_bufferDisplay[offset].position = sf::Vector2f(x * _sizePixel, y * _sizePixel);
			_bufferDisplay[offset].color = sf::Color::Black;
			//right Up
			_bufferDisplay[offset + 1] = sf::Vector2f((x + 1) * _sizePixel, y * _sizePixel);
			_bufferDisplay[offset + 1].color = sf::Color::Black;
			//right Down
			_bufferDisplay[offset + 2] = sf::Vector2f((x + 1) * _sizePixel, (y + 1) * _sizePixel);
			_bufferDisplay[offset + 2].color = sf::Color::Black;
			//left Down
			_bufferDisplay[offset + 3] = sf::Vector2f(x * _sizePixel, (y + 1) * _sizePixel);
			_bufferDisplay[offset + 3].color = sf::Color::Black;
		}
	}
}

void GraphicsChip8::fillQuad(sf::VertexArray& buffer, int posStart, const sf::Color& color)
{
	//left Up
	_bufferDisplay[posStart].color = color;
	//right Up
	_bufferDisplay[posStart + 1].color = color;
	//right Down
	_bufferDisplay[posStart + 2].color = color;
	//left Down
	_bufferDisplay[posStart + 3].color = color;
}

void GraphicsChip8::draw()
{
	_window.draw(_bufferDisplay);
}

void GraphicsChip8::show()
{
	_window.display();
}

void GraphicsChip8::clear(const sf::Color& color)
{
	_window.clear(color);
}
