#include "../../include/eventSystem/eventsystem.h"

void EventSystem::proccessEvent()
{
	while (_window.pollEvent(_event))
	{
		if (_callbackFunctions.find(_event.type) != _callbackFunctions.end())
			for (int i = 0; i < _callbackFunctions[_event.type].size(); i++)
				_callbackFunctions[_event.type][i](_event);
			
	}
}

void EventSystem::addEvent(const sf::Event::EventType& type, const std::function<void(sf::Event&)> callFunction)
{
	_callbackFunctions[type].push_back(callFunction);
}
