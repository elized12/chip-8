#include "../include/keyboardChip8.h"

void KeyboardChip8::keyPressed(sf::Event& event)
{
	setKeyArray(event, true);
}

void KeyboardChip8::keyReleased(sf::Event& event)
{
	setKeyArray(event, false);
}

void KeyboardChip8::setKeyArray(sf::Event& event, bool setValue)
{
	switch (event.key.code)
	{
		case sf::Keyboard::Num1:
		{
			_keyArray[0] = setValue;
			break;
		}
		case sf::Keyboard::Num2:
		{
			_keyArray[1] = setValue;
			break;
		}
		case sf::Keyboard::Num3:
		{
			_keyArray[2] = setValue;
			break;
		}
		case sf::Keyboard::Num4:
		{
			_keyArray[3] = setValue;
			break;
		}
		case sf::Keyboard::Q:
		{
			_keyArray[4] = setValue;
			break;
		}
		case sf::Keyboard::W:
		{
			_keyArray[5] = setValue;
			break;
		}
		case sf::Keyboard::E:
		{
			_keyArray[6] = setValue;
			break;
		}
		case sf::Keyboard::R:
		{
			_keyArray[7] = setValue;
			break;
		}
		case sf::Keyboard::A:
		{
			_keyArray[8] = setValue;
			break;
		}
		case sf::Keyboard::S:
		{
			_keyArray[9] = setValue;
			break;
		}
		case sf::Keyboard::D:
		{
			_keyArray[10] = setValue;
			break;
		}
		case sf::Keyboard::F:
		{
			_keyArray[11] = setValue;
			break;
		}
		case sf::Keyboard::Z:
		{
			_keyArray[12] = setValue;
			break;
		}
		case sf::Keyboard::X:
		{
			_keyArray[13] = setValue;
			break;
		}
		case sf::Keyboard::C:
		{
			_keyArray[14] = setValue;
			break;
		}
		case sf::Keyboard::V:
		{
			_keyArray[15] = setValue;
			break;
		}
		default:
			break;
	}
}
