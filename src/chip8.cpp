#include "../include/chip8.h"


#ifdef _DEBUG
#include <iostream>
#include <iomanip>
#define logger(text, number) std::cerr << text << " " << std::hex << std::setw(4) << std::setfill('0') << number << "\n";
#endif // _DEBUG

#ifdef _RELEASE
#define logger(text, number) 
#endif // _RELEASE



Chip8::Chip8() : _sound("sound.wav")
{
	init();
	loadFont();
	_sound.setVolume(15);
}

void Chip8::emulateCycle()
{
	_operationCode = getOpCode(_memory[_programCounter], _memory[_programCounter + 1]);

	parseOperation parseOperation = parse(_operationCode);

	if (_delayTimer > 0)
		_delayTimer--;

	if (_soundTimer > 0)
	{
		_sound.play();
		_soundTimer--;
	}

	switch (parseOperation.firstBlock)
	{
		case 0x0:
		{
			if (parseOperation.thirdBlock == 0xE && parseOperation.fourthBlock == 0xE) // RET
			{
				_ptrStack--;
				_programCounter = _stack[_ptrStack];
				_programCounter += 2;
			}
			else if (parseOperation.thirdBlock == 0xE)	// CLS
			{
				clearDisplay();
				_programCounter += 2;
			}
			else //SYS addr
			{
				//ignore
				logger("Unknown opcode", _operationCode)
					_programCounter += 2;
				return;
			}
			break;
		}
		case 0x1: //JP
		{
			uint16_t jumpAddress = mergeBlock(3, parseOperation.secondBlock, parseOperation.thirdBlock, parseOperation.fourthBlock);
			_programCounter = jumpAddress;
			break;
		}
		case 0x2: //callFunc
		{
			uint16_t jumpAddress = mergeBlock(3, parseOperation.secondBlock, parseOperation.thirdBlock, parseOperation.fourthBlock);
			_stack[_ptrStack++] = _programCounter;
			_programCounter = jumpAddress;
			break;
		}
		case 0x3: //SE Vx
		{
			uint16_t value = mergeBlock(2, parseOperation.thirdBlock, parseOperation.fourthBlock);
			uint8_t valueRegister = _regs.regs[parseOperation.secondBlock];

			if (valueRegister == value)
				_programCounter += 4;
			else
				_programCounter += 2;
			break;
		}
		case 0x4: //SNE Vx
		{
			uint16_t value = mergeBlock(2, parseOperation.thirdBlock, parseOperation.fourthBlock);
			uint8_t valueRegister = _regs.regs[parseOperation.secondBlock];

			if (valueRegister != value)
				_programCounter += 4;
			else
				_programCounter += 2;
			break;
		}
		case 0x5: //SE Vx, Vy
		{
			uint16_t regX = _regs.regs[parseOperation.secondBlock];
			uint16_t regY = _regs.regs[parseOperation.thirdBlock];

			if (regX == regY)
				_programCounter += 4;
			else
				_programCounter += 2;
			break;
		}
		case 0x6: //LD Vx, byte
		{
			uint8_t byte = mergeBlock(2, parseOperation.thirdBlock, parseOperation.fourthBlock);
			_regs.regs[parseOperation.secondBlock] = byte;
			_programCounter += 2;
			break;
		}
		case 0x7: //ADD Vx, byte
		{
			uint8_t byte = mergeBlock(2, parseOperation.thirdBlock, parseOperation.fourthBlock);
			_regs.regs[parseOperation.secondBlock] += byte;
			_programCounter += 2;
			break;
		}
		case 0x8: //LD Vx, Vy
		{
			switch (parseOperation.fourthBlock)
			{
			case 0x0: //LD Vx, Vy
			{
				_regs.regs[parseOperation.secondBlock] = _regs.regs[parseOperation.thirdBlock];
				break;
			}
			case 0x1: //OR Vx, Vy
			{
				_regs.regs[parseOperation.secondBlock] |= _regs.regs[parseOperation.thirdBlock];
				break;
			}
			case 0x2: //AND Vx, Vy
			{
				_regs.regs[parseOperation.secondBlock] &= _regs.regs[parseOperation.thirdBlock];
				break;
			}
			case 0x3: //XOR Vx, Vy
			{
				_regs.regs[parseOperation.secondBlock] ^= _regs.regs[parseOperation.thirdBlock];
				break;
			}
			case 0x4: //ADD Vx, Vy
			{
				if (_regs.regs[parseOperation.secondBlock] > 255 - _regs.regs[parseOperation.thirdBlock])
					_regs.VF = 1;
				else
					_regs.VF = 0;

				_regs.regs[parseOperation.secondBlock] += _regs.regs[parseOperation.thirdBlock];
				break;
			}
			case 0x5: //SUB Vx, Vy
			{
				if (_regs.regs[parseOperation.thirdBlock] > _regs.regs[parseOperation.secondBlock])
					_regs.VF = 0;
				else
					_regs.VF = 1;

				_regs.regs[parseOperation.secondBlock] -= _regs.regs[parseOperation.thirdBlock];
				break;
			}
			case 0x6: //SHR Vx
			{
				uint8_t bit = _regs.regs[parseOperation.secondBlock] & (0x0001);
				_regs.VF = bit;
				_regs.regs[parseOperation.secondBlock] >>= 1;
				break;
			}
			case 0x7://SUBN Vx, Vy
			{
				if (_regs.regs[parseOperation.thirdBlock] > _regs.regs[parseOperation.secondBlock])
					_regs.VF = 1;
				else
					_regs.VF = 0;

				_regs.regs[parseOperation.secondBlock] = _regs.regs[parseOperation.thirdBlock] - _regs.regs[parseOperation.secondBlock];
				break;
			}
			case 0xE: //SHL Vx 
			{
				_regs.VF = _regs.regs[parseOperation.secondBlock] >> 7;
				_regs.regs[parseOperation.secondBlock] <<= 1;
				break;
			}
			default:
			{
				logger("Unknown opcode", _operationCode)
					break;
			}
			}
			_programCounter += 2;
			break;
		}
		case 0x9: //SNE Vx, Vy
		{
			uint8_t& regX = _regs.regs[parseOperation.secondBlock];
			uint8_t& regY = _regs.regs[parseOperation.thirdBlock];

			if (regX != regY)
				_programCounter += 4;
			else
				_programCounter += 2;
			break;
		}
		case 0xA: //LD I, addr
		{
			uint16_t address = mergeBlock(3, parseOperation.secondBlock, parseOperation.thirdBlock, parseOperation.fourthBlock);
			_regs.I = address;
			_programCounter += 2;
			break;
		}
		case 0xB: //JP VO, addr
		{
			uint16_t address = mergeBlock(3, parseOperation.secondBlock, parseOperation.thirdBlock, parseOperation.fourthBlock);
			_programCounter = address + _regs.regs[0];
			break;
		}
		case 0xC: //RAND Vx, byte
		{
			uint8_t byte = mergeBlock(2, parseOperation.thirdBlock, parseOperation.fourthBlock);
			_regs.regs[parseOperation.secondBlock] = (rand() % 0xFF) & byte;
			_programCounter += 2;
			break;
		}
		case 0xD: //Draw Vx, Vy, N
		{
			uint16_t coordX, coordY;
			coordX = _regs.regs[parseOperation.secondBlock];
			coordY = _regs.regs[parseOperation.thirdBlock];
			uint8_t height = parseOperation.fourthBlock;

			_regs.VF = 0;

			for (int y = 0; y < height; y++)
			{
				uint8_t pixels = _memory[_regs.I + y];

				for (int x = 0; x < 8; x++)
				{
					bool fillPixel = pixels & (0b10000000 >> x);

					int tempX = (coordX + x) % 64;
					int tempY = (coordY + y) % 32;

					if (_regs.VF == 0 && _displayBuffer[tempY][tempX] == 1 && fillPixel == true)
						_regs.VF = 1;

					_displayBuffer[tempY][tempX] ^= fillPixel;
				}
			}
			_flagDraw = true;
			_programCounter += 2;
			break;
		}
		case 0xE:
		{
			switch (parseOperation.fourthBlock)
			{
			case 0xE: //SKP Vx
			{
				uint8_t key = _regs.regs[parseOperation.secondBlock];

				if (_keys[key] == true)
					_programCounter += 4;
				else
					_programCounter += 2;
				break;
			}
			case 0x1: //SKP Vx
			{
				uint8_t key = _regs.regs[parseOperation.secondBlock];

				if (_keys[key] != true)
					_programCounter += 4;
				else
					_programCounter += 2;
				break;
			}
			default:
				logger("Unknown opcode", _operationCode)
					break;
			}
			break;
		}
		case 0xF:
		{
			switch (parseOperation.fourthBlock)
			{
			case 0x7:
			{
				_regs.regs[parseOperation.secondBlock] = _delayTimer;
				_programCounter += 2;
				break;
			}
			case 0xA:
			{
				bool isKeyPressed = false;

				for (int i = 0; i < 16; i++)
				{
					if (_keys[i] == true)
					{
						isKeyPressed = true;
						_regs.regs[parseOperation.secondBlock] = i;
						break;
					}
				}

				if (!isKeyPressed)
					return;

				_programCounter += 2;

				break;
			}
			case 0x5:
			{
				switch (parseOperation.thirdBlock)
				{
				case 0x1:
				{
					_delayTimer = _regs.regs[parseOperation.secondBlock];
					_programCounter += 2;
					break;
				}
				case 0x5:
				{
					uint8_t lastReg = parseOperation.secondBlock;

					for (int shift = 0; shift <= lastReg; shift++)
					{
						_memory[_regs.I + shift] = _regs.regs[shift];
					}

					_regs.I += parseOperation.secondBlock + 1;
					_programCounter += 2;
					break;
				}
				case 0x6:
				{
					uint8_t lastReg = parseOperation.secondBlock;

					for (int shift = 0; shift <= lastReg; shift++)
					{
						_regs.regs[shift] = _memory[_regs.I + shift];
					}

					_regs.I += parseOperation.secondBlock + 1;
					_programCounter += 2;
					break;
				}
				default:
					logger("Unknown opcode", _operationCode)
						break;
				}
				break;
			}
			case 0x8:
			{
				_soundTimer = _regs.regs[parseOperation.secondBlock];
				_programCounter += 2;
				break;
			}
			case 0xE:
			{
				_regs.I += _regs.regs[parseOperation.secondBlock];
				_programCounter += 2;
				break;
			}
			case 0x9:
			{
				_regs.I = _regs.regs[parseOperation.secondBlock] * 5;
				_programCounter += 2;
				break;
			}
			case 0x3:
			{
				uint8_t value = _regs.regs[parseOperation.secondBlock];

				uint8_t hundreds = value / 100;
				uint8_t tens = value / 10 % 10;
				uint8_t units = value % 10;

				_memory[_regs.I] = hundreds;
				_memory[_regs.I + 1] = tens;
				_memory[_regs.I + 2] = units;

				_programCounter += 2;
				break;
			}
			default:
				logger("Unknown opcode", _operationCode)
					break;
			}
			break;
		}
		default:
		{
			logger("Unknown opcode", _operationCode);
			break;
		}
	}
}

void Chip8::loadProgram(std::vector<uint8_t>& bytes)
{
	for (int i = 0x200, j = 0; j < bytes.size(); j++, i++)
		_memory[i] = bytes[j];
}

void Chip8::loadProgram(std::vector<uint8_t>&& bytes)
{
	for (int i = 0x200, j = 0; j < bytes.size(); j++, i++)
		_memory[i] = bytes[j];
}

void Chip8::clearMemory() noexcept
{
	init();
	loadFont();
}

void Chip8::init() noexcept
{ 
	memset(_memory, NULL, sizeof(_memory));
	memset(_regs.regs, NULL, sizeof(_regs.regs));
	memset(_stack, NULL, sizeof(_stack));
	memset(_keys, NULL, sizeof(_keys));

	clearDisplay();

	_regs.I = 0;
	_regs.VF = 0;
	_programCounter = 0x200;
	_ptrStack = 0;
	_operationCode = 0;
	_delayTimer = 0;
	_soundTimer = 0;
}

void Chip8::loadFont() noexcept
{
	const uint8_t fonts[] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	for (int i = 0; i < sizeof(fonts); i++)
	{
		_memory[i] = fonts[i];
	}

}

uint16_t Chip8::getOpCode(uint8_t& firstBlockMemory, uint8_t& secondBlockMemory)
{
	uint16_t opCode = (firstBlockMemory << 8) | (secondBlockMemory);
	return opCode;
}

parseOperation Chip8::parse(uint16_t& operation)
{
	parseOperation parseOp;

	parseOp.firstBlock = (operation & (0xF000)) >> 12;
	parseOp.secondBlock = (operation & (0x0F00)) >> 8;
	parseOp.thirdBlock = (operation & (0x00F0)) >> 4;
	parseOp.fourthBlock = operation & (0x000F);

	return parseOp;
}

uint16_t Chip8::mergeBlock(int count, ...)
{
	int shift = (count - 1) * 4;

	va_list args;

	va_start(args, count);

	uint16_t mergedBlock = 0;

	if (count > 4 || count <= 0)
		throw std::exception();


	for (int i = 0; i < count; i++)
	{
		int block = (va_arg(args, int) << shift);

		mergedBlock = (mergedBlock | block);

		shift = shift - 4 >= 0 ? shift - 4 : 0;
	}

	va_end(args);

	return mergedBlock;
}

void Chip8::clearDisplay() noexcept
{
	for (int y = 0; y < _displayBuffer.size(); y++)
	{
		for (int x = 0; x < _displayBuffer[y].size(); x++)
			_displayBuffer[y][x] = 0;
	}
}

std::array<std::array<uint8_t, 64>, 32>& Chip8::getDisplay() noexcept
{
	return _displayBuffer;
}

bool* Chip8::getKeyArray(int& size)
{
	size = 16;
	return _keys;
}
