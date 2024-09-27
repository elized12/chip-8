#pragma once
#include <cstdint>
#include <memory>
#include <array>
#include <vector>
#include <cstdarg>
#include "soundChip8.h"


struct registers
{
	uint8_t regs[16];
	uint8_t& VF = regs[15];
	uint16_t I : 12;
};

//the first block is the first 4 bits from the left
struct parseOperation
{
	uint16_t firstBlock : 4;
	uint16_t secondBlock : 4;
	uint16_t thirdBlock : 4;
	uint16_t fourthBlock : 4;
};


class Chip8
{
private:
	uint8_t _memory[4096];
	registers _regs; //регистры
	uint16_t _programCounter; //программный счетчик
	uint16_t _stack[16]; //стек
	uint8_t _ptrStack; //указатель стека
	/// <summary>
	/// 1 - WHITE
	/// 0 - BLACK
	/// </summary>
	std::array<std::array<uint8_t, 64>, 32> _displayBuffer; // графика width 64 height 32
	uint8_t _delayTimer;
	uint8_t _soundTimer;
	uint16_t _operationCode; // код операции
	bool _keys[16];
	SoundChip8 _sound;
	bool _flagDraw; //флаг что нужно перерисовать экран
public:
	Chip8();
public:
	void emulateCycle();
	void loadProgram(std::vector<uint8_t>& bytes);
	void loadProgram(std::vector<uint8_t>&& bytes);
	void clearMemory() noexcept;
	void clearDisplay() noexcept;
	std::array<std::array<uint8_t, 64>, 32>& getDisplay() noexcept;
	bool* getKeyArray(int& size);
	bool getDrawFlag() const noexcept;
	void resetDrawFlag() noexcept;
	parseOperation parse(uint16_t& operation);
private:
	void init() noexcept;
	void loadFont() noexcept;
	uint16_t getOpCode(uint8_t& firstBlockMemory, uint8_t& secondBlockMemory);
	uint16_t mergeBlock(int count, ...);
};