# Chip-8 Emulator

Эмулятор классической виртуальной машины [Chip-8](https://en.wikipedia.org/wiki/CHIP-8) на C++ с графическим интерфейсом на базе SFML и поддержкой звука. Проект позволяет запускать оригинальные игры Chip-8, такие как Tetris, Pong, Space Invaders и другие.

## Особенности

- **Полная эмуляция**: поддержка всех основных инструкций Chip-8, таймеров, стека, клавиатуры и графики.
- **Графика**: вывод изображения через SFML, масштабируемые пиксели.
- **Звук**: воспроизведение звукового сигнала при необходимости.
- **Управление**: гибкая обработка событий клавиатуры, поддержка горячих клавиш.
- **Загрузка игр**: удобный выбор и загрузка ROM-файлов через диалоговое окно (NFD).
- **Кроссплатформенность**: работает на Windows и Linux.

## Зависимости

- [SFML](https://www.sfml-dev.org/) (Simple and Fast Multimedia Library)
- [NFD](https://github.com/mlabbe/nativefiledialog) (Native File Dialog)
- Для Windows: положите DLL-файлы SFML и NFD в корневую папку проекта рядом с исполняемым файлом.

## Структура проекта

```
chip-8/
├── main.cpp # Точка входа, запуск эмулятора
├── include/ # Заголовочные файлы классов
│ ├── chip8.h
│ ├── graphicsChip8.h
│ ├── keyboardChip8.h
│ ├── soundChip8.h
│ ├── gameReader.h
│ └── eventSystem/
│ └── eventsystem.h
├── src/ # Реализация классов
│ ├── chip8.cpp
│ ├── graphicsChip8.cpp
│ ├── keyboardChip8.cpp
│ ├── soundChip8.cpp
│ ├── gameReader.cpp
│ └── eventSystem/
│ └── eventsystem.cpp
├── *.c8 # ROM-файлы игр (Tetris, Pong, Invaders и др.)
├── *.dll # Необходимые DLL для SFML и NFD (Windows)
├── README.md # Описание проекта
└── .gitignore
```

## Управление

| Chip-8 клавиша | Клавиша на клавиатуре |
|:--------------:|:---------------------:|
| 1 2 3 C        | 1 2 3 4               |
| 4 5 6 D        | Q W E R               |
| 7 8 9 E        | A S D F               |
| A 0 B F        | Z X C V               |

## Примеры ROM-файлов

В комплекте идут классические игры:
- `tetris.c8`
- `pong2.c8`
- `invaders.c8`