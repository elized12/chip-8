#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <thread>
#include "include/chip8.h"
#include "include/graphicsChip8.h"
#include "include/gameReader.h"
#include "include/eventSystem/eventsystem.h"
#include "include/keyboardChip8.h"
#include "nfd.h"

const int FPS = 60;

using namespace std::chrono_literals;

#if defined(_WIN32) && defined(_RELEASE)
#include <Windows.h>
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char* argv[])
#endif
{
	sf::RenderWindow window(sf::VideoMode(640, 320), "Chip-8", sf::Style::Titlebar | sf::Style::Close);
	GraphicsChip8 graphicsChip(window, 32, 64, 10);
	Chip8 chip;
	GameReader readerGame;
	EventSystem eventSystem(window);

	bool isStop = true;
	int sizeKeyboardArray;
	bool* keyBoardArray = chip.getKeyArray(sizeKeyboardArray);
	auto& bufferDisplay = chip.getDisplay();
	KeyboardChip8 keyboard(keyBoardArray, sizeKeyboardArray);

	eventSystem.addEvent(sf::Event::EventType::Closed, [&window](sf::Event&) { window.close(); });
	eventSystem.addEvent(sf::Event::EventType::KeyPressed, [&keyboard](sf::Event& event) {keyboard.keyPressed(event); });
	eventSystem.addEvent(sf::Event::EventType::KeyReleased, [&keyboard](sf::Event& event) {keyboard.keyReleased(event); });
	eventSystem.addEvent(sf::Event::EventType::KeyPressed, [&isStop](sf::Event& event) {if (event.key.code == sf::Keyboard::Escape) isStop = true; });

	readerGame.readGame("C:\\Users\\User\\Desktop\\invaders.c8");

	chip.loadProgram(readerGame.getGame());

    while (window.isOpen()) 
    {
		if (isStop == true)
		{
			chip.clearDisplay();
			chip.clearMemory();
			nfdchar_t* outPath = NULL;
			nfdresult_t result = NFD_OpenDialog("c8", NULL, &outPath);

			if (result == NFD_OKAY)
			{
				if (readerGame.readGame(outPath))
				{
					chip.loadProgram(readerGame.getGame());
					isStop = false;
				}
			}

		}
		else
		{
			eventSystem.proccessEvent();
			chip.emulateCycle();
			graphicsChip.drawForCycle(bufferDisplay);
		}
    }

	return 0;
}