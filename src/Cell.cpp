#include "Cell.h"

static void GetRW(const CellType type, bool& rwRotation, int& rwColors)
{
    switch (type)
	{
	case CellType::PipeLine:
	case CellType::PipeCorner:
	case CellType::PipeTriple:
		rwRotation = true;
		rwColors = 0;
		break;

	case CellType::Empty:
	case CellType::PipePlus:
		rwRotation = false;
		rwColors = 0;
		break;

	case CellType::ColorSwitcher:
		rwRotation = true;
		rwColors = 2;
		break;

	case CellType::Source:
		rwRotation = true;
		rwColors = 1;
		break;

	case CellType::Stock:
		rwRotation = false;
		rwColors = 1;
		break;

	default:
		rwRotation = true;
		rwColors = 2;
		break;
	}
}

Cell::Cell(FILE* file)
{
	fscanf_s(file, "%u%u", &type, &ground);
	bool rwRotation = false;
	int rwColors = 0;
	GetRW(type, rwRotation, rwColors);
	if (rwRotation)
		fscanf_s(file, "%u", &rotation);
	if (rwColors >= 1)
		fscanf_s(file, "%u", &clr1);
	if (rwColors >= 2)
		fscanf_s(file, "%u", &clr2);
}

Cell::Cell()
{
}

Cell::Cell(CellType type)
	: type(type)
{
}

Cell::Cell(CellType type, Rotation rotation)
	: type(type), rotation(rotation)
{
}

Cell::Cell(CellType type, Ground ground)
	: type(type), ground(ground)
{
}

Cell::Cell(CellType type, Ground ground, Rotation rotation)
	: type(type), ground(ground), rotation(rotation)
{
}

void Cell::Output(FILE* file) const
{
	fprintf_s(file, "%u %u ", type, ground);
	bool rwRotation = false;
	int rwColors = 0;
	GetRW(type, rwRotation, rwColors);
	if (rwRotation)
		fprintf_s(file, "%u ", rotation);
	if (rwColors >= 1)
		fprintf_s(file, "%u ", clr1);
	if (rwColors >= 2)
		fprintf_s(file, "%u ", clr2);
}

void Cell::DoRotate()
{
	switch (rotation)
	{
	case Rotation::Angle0:
		rotation = Rotation::Angle90;
		break;
	case Rotation::Angle90:
		rotation = Rotation::Angle180;
		break;
	case Rotation::Angle180:
		rotation = Rotation::Angle270;
		break;
	case Rotation::Angle270:
		rotation = Rotation::Angle0;
		break;
    }
}

bool Cell::IsPipe() const
{
	switch (type)
	{
	case CellType::PipeLine:
	case CellType::PipeCorner:
	case CellType::PipePlus:
	case CellType::PipeTriple:
		return true;

	default:
		return false;
	}
}

const std::vector<std::pair<char, char>> Cell::GetDirections() const // можно нормально оптимизировать
{
	using namespace std;
	switch (type)
	{

	case CellType::Empty:
	case CellType::Stock:
		return { };

	case CellType::PipeLine:
	case CellType::ColorSwitcher:
		switch (rotation)
		{
		case Rotation::Angle0:
		case Rotation::Angle180:
			return { make_pair(-1, 0), make_pair(1, 0) };
		default:
			return { make_pair(0, -1), make_pair(0, 1) };
		}

	case CellType::PipeCorner:
		switch (rotation)
		{
		case Rotation::Angle0:
			return { make_pair(-1, 0), make_pair(0, -1) };
		case Rotation::Angle90:
			return { make_pair(0, -1), make_pair(1, 0) };
		case Rotation::Angle180:
			return { make_pair(1, 0), make_pair(0, 1) };
		case Rotation::Angle270:
			return { make_pair(0, 1), make_pair(-1, 0) };
		}

	case CellType::PipePlus:
		switch (rotation)
		{
		case Rotation::Angle0:
		case Rotation::Angle180:
			return { make_pair(-1, 0), make_pair(1, 0), make_pair(0, -1), make_pair(0, 1) };

		default:
			return { make_pair(0, -1), make_pair(0, 1), make_pair(-1, 0), make_pair(1, 0) };
		}

	case CellType::PipeTriple:
		switch (rotation)
		{
		case Rotation::Angle0:
			return { make_pair(-1, 0), make_pair(0, -1), make_pair(1, 0) };
		case Rotation::Angle90:
			return { make_pair(0, -1), make_pair(1, 0), make_pair(0, 1) };
		case Rotation::Angle180:
			return { make_pair(1, 0), make_pair(0, 1), make_pair(-1, 0) };
		case Rotation::Angle270:
			return { make_pair(0, 1), make_pair(-1, 0), make_pair(0, -1) };
		}

	case CellType::Source:
		switch (rotation)
		{
        case Rotation::Angle0:
			return { make_pair(0, -1) };
		case Rotation::Angle90:
			return { make_pair(1, 0) };
		case Rotation::Angle180:
			return { make_pair(0, 1) };
		case Rotation::Angle270:
			return { make_pair(-1, 0) };
        }

	}
}

