#pragma once
#include <unordered_map>
#include <functional>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"


class EventSystem
{
private:
	sf::Event _event;
	sf::RenderWindow& _window;
	std::unordered_map<sf::Event::EventType, std::vector<std::function<void(sf::Event&)>>> _callbackFunctions;
public:
	EventSystem(sf::RenderWindow& window) : _window(window) {}
	EventSystem(const EventSystem&) = delete;
	EventSystem& operator=(const EventSystem&) = delete;
public:
	void proccessEvent();
	void addEvent(const sf::Event::EventType& type, const std::function<void(sf::Event&)> callFunction);
};