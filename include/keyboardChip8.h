#pragma once
#include "SFML/Window.hpp"

class KeyboardChip8
{
private:
	bool* _keyArray;
	int _size;
public:
	 KeyboardChip8(bool* arrayKeyChip8,int size) : _keyArray(arrayKeyChip8), _size(size) {}
public:
	void keyPressed(sf::Event& event);
	void keyReleased(sf::Event& event);
private:
	void setKeyArray(sf::Event& event, bool setValue);
};