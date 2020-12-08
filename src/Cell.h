#pragma once
#include "MyColor.h"
#include <vector>

enum class CellType : unsigned int
{
	Empty = 0,
	PipeLine,
	PipeCorner,
	PipePlus,
	PipeTriple,
	ColorSwitcher,
	Source,
	Stock
};

enum class Ground : unsigned int
{
	Empty = 0,
	Stone
};

enum class Rotation : unsigned int
{
	Angle0 = 0,
	Angle90,
	Angle180,
	Angle270
};

struct Cell
{
	CellType type = CellType::Empty;
	Ground ground = Ground::Empty;
	Rotation rotation = Rotation::Angle0;
	bool activated1 = false; // есть ли в трубе1 вода
	bool activated2 = false; // есть ли в трубе2 вода
	unsigned int clr1 = 0;
	unsigned int clr2 = 0;
	unsigned int clr3 = 0; // цвет воды в colorswitcher

	Cell();
	Cell(FILE* file);
	Cell(CellType type);
	Cell(CellType type, Rotation rotation);
	Cell(CellType type, Ground ground);
	Cell(CellType type, Ground ground, Rotation rotation);

	bool IsPipe() const;
	void Output(FILE* file) const;
	void DoRotate();
	const std::vector<std::pair<char, char>> GetDirections() const; // ќ„≈Ќ№ много костылей
};
