#include "Level.h"
#include "Constants.h"
#include "Utilities.h"
#include "EditorUnit1.h"
#include <ctime>
#include <random>

std::mt19937 random_engine(time(0));

Level::Level() // из EditorUnit1 вначале
	: LevelPreview(), fromEditor(true)
{
	LoadDefault();
}

Level::Level(const wchar_t* path, bool fromEditor)
	: LevelPreview(path), fromEditor(fromEditor)
{
	LoadFromFile();
}

Level::Level(bool fromEditor) // из GameUnit1 вначале
	: LevelPreview(), fromEditor(fromEditor)
{
	if (fromEditor)
        LoadFromFile();
}

void Level::LoadDefault()
{
	colorMap.resize(colorsCount);
	FillDefaultColors(0, colorsCount - 1);
	cells.resize(colCount * rowCount);
    highlighted.resize(colCount * rowCount);
	for (unsigned int i = 0; i < cells.size(); ++i)
		cells[i] = Cell();
}

void Level::FillDefaultColors(unsigned int l, unsigned int r)
{
	for (unsigned int i = l; i <= r; ++i)
	{
		if (!i)
			colorMap[i] = Colors::eUniversalColor;
		else
			colorMap[i] = Colors::ePresetColors[(i - 1) % Colors::ePresetColorsCount];
    }
}

// unsafe (no checks)
void Level::LoadFromFile()
{
	if (!path.length())
		return;
	FILE* f = _wfopen(path.c_str(), L"r");

	if (!f)
		return;

	fscanf_s(f, "%u%u%u%u%u%u%u%u", &levelId, &type, &difficulty,
		&completed, &rowCount, &colCount, &time, &colorsCount);

	colorMap.reserve(colorsCount);
	for (unsigned int i = 0; i < colorsCount; ++i)
	{
		unsigned int clr;
		fscanf_s(f, "%u", &clr);
		colorMap.emplace_back(clr);
	}

	unsigned int fieldSize = rowCount * colCount;
	highlighted.resize(fieldSize);
	cells.reserve(fieldSize);
	for (unsigned int i = 0; i < fieldSize; ++i)
		cells.emplace_back(f);

	fclose(f);
}

// unsafe (no checks)
void Level::WriteToFile(const wchar_t* path) const
{
	FILE* f = _wfopen(path, L"w");
	if (!f)
		return;

	fprintf_s(f, "%u %u %u %u %u %u %u %u\n", levelId, type, difficulty,
		completed, rowCount, colCount, time, colorsCount);

	for (unsigned int i = 0; i < colorsCount; ++i)
	{
		fprintf_s(f, "%u ", (unsigned int)colorMap[i]);
	}
	fprintf_s(f, "\n");

	unsigned int fieldSize = rowCount * colCount;
	for (unsigned int i = 0; i < fieldSize; ++i)
		cells[i].Output(f);

	fclose(f);
}

void Level::UpdateCompleted() const
{
	const wchar_t* cPath = path.c_str();
	Level levelCopy(cPath);
	levelCopy.completed = true;
	levelCopy.WriteToFile(cPath);
}

void Level::RegenerateCell()
{
	static std::uniform_int_distribution<unsigned int> uniCell(0, Stuff::gClassicRandomCount - 1);
	randomCell = Stuff::gClassicRandom[uniCell(random_engine)];
}

bool Level::CheckBounds(int X, int Y) const
{
	return X >= 0 && Y >= 0 && X < colCount && Y < rowCount;
}

void Level::SwitchToSecondPhase()
{
	directions.resize(cells.size());
	for (unsigned int i = 0; i < cells.size(); ++i)
	{
		directions[i] = cells[i].GetDirections();
		if (cells[i].type == CellType::Source)
		{
			bfs.push(i);
			cells[i].activated1 = true;
			cells[i].activated2 = true;
		}
    }
}

LevelState Level::ProcessCell(unsigned int linear, int X, int Y,
	unsigned int color, const std::vector<std::pair<char, char>>& dir, LevelState ret)
{
    const Cell& cell = cells[linear];
	for (int j = 0; j < dir.size(); ++j)
	{
		int newX = X + dir[j].first;
		int newY = Y + dir[j].second;
		if (!CheckBounds(newX, newY))
		{
			if (ret == LevelState::NotFinished || ret == LevelState::LostLeaked)
			{
				ret = LevelState::LostLeaked;
				highlighted[linear] = true;
			}
		}
		else
		{
			const unsigned int newLinear = Utilities::LinearFrom2D(newX, newY, colCount);
			Cell& newCell = cells[newLinear];
			const std::vector<std::pair<char, char>>& newDir = directions[newLinear];
			auto it = find(newDir.begin(), newDir.end(),
				std::make_pair((char)-dir[j].first, (char)-dir[j].second));
			if (it == newDir.end() && newCell.type != CellType::Stock)
			{
				if (ret == LevelState::NotFinished || ret == LevelState::LostLeaked)
				{
					ret = LevelState::LostLeaked;
					highlighted[linear] = highlighted[linear] ? true :
						Stuff::gHighlightLeakFrom;
					highlighted[newLinear] = true;
				}
			}
			else
			{
				const bool plus2 = (it - newDir.begin()) >= 2;
				const bool targetActivated = newCell.type == CellType::PipePlus ?
					(plus2 ? newCell.activated2 : newCell.activated1) :
					(newCell.type == CellType::Stock ?
					true : newCell.activated1);
				const unsigned int targetClr = newCell.type == CellType::PipePlus ?
					(plus2 ? newCell.clr2 : newCell.clr1) :
					(newCell.type == CellType::ColorSwitcher ? newCell.clr3 :
					(newCell.type == CellType::Stock && !newCell.clr1 ? color : newCell.clr1));

				bool mixed = false;
				if (cell.type == CellType::ColorSwitcher)
					mixed = targetActivated && targetClr != color &&
					!(targetClr == cell.clr1 && color == cell.clr2);
				else
					mixed =	targetActivated && targetClr != color;

				if (mixed)
				{
					if (newCell.type == CellType::Stock)
					{
						if (ret == LevelState::NotFinished || ret == LevelState::LostStockError)
						{
							ret = LevelState::LostStockError;
							highlighted[linear] = true;
							highlighted[newLinear] = true;
						}
					}
					else
					{
						if (ret == LevelState::NotFinished || ret == LevelState::LostMixed)
						{
							ret = LevelState::LostMixed;
							highlighted[linear] = true;
							highlighted[newLinear] = true;
						}
					}
				}
				else if (!targetActivated || (newCell.type == CellType::Stock &&
					!newCell.activated1))
				{
					bool& _activated = newCell.type == CellType::PipePlus ?
					(plus2 ? newCell.activated2 : newCell.activated1) :
					newCell.activated1;
					unsigned int& _color = newCell.type == CellType::PipePlus ?
					(plus2 ? newCell.clr2 : newCell.clr1) :
					(newCell.type == CellType::ColorSwitcher ? newCell.clr3 : newCell.clr1);
					_activated = true;
					if (newCell.type != CellType::Stock)
					{
						if (newCell.type == CellType::ColorSwitcher)
						{
							if (color == newCell.clr1)
								_color = newCell.clr2;
							else
								_color = color;
						}
						else
							_color = color;
						bfs.push(newLinear);
					}
				}
			}
		}
	}

	return ret;
}

LevelState Level::GameTick()
{
	LevelState ret = LevelState::NotFinished;
	unsigned int size = bfs.size();
	if (!size)
	{
		ret = LevelState::Completed;
		for (unsigned int i = 0; i < cells.size(); ++i)
		{
			if (cells[i].type == CellType::Stock && !cells[i].activated1)
			{
				highlighted[i] = true;
				ret = LevelState::LostStocks;
			}
		}
		return ret;
	}
	for (unsigned int i = 0; i < size; ++i)
	{
		unsigned int linear = bfs.front();
		bfs.pop();
		int X, Y;
		Utilities::LinearTo2D(linear, colCount, X, Y);
		const Cell& cell = cells[linear];
		if (cell.type == CellType::PipePlus)
		{
			const std::vector<std::pair<char, char>>& dir4 = directions[linear];
			if (cell.activated1)
			{
				const std::vector<std::pair<char, char>> dir(dir4.begin(), dir4.begin() + 2);
				ret = ProcessCell(linear, X, Y, cell.clr1, dir, ret);
			}
			if (cell.activated2)
			{
				const std::vector<std::pair<char, char>> dir(dir4.begin() + 2, dir4.end());
				ret = ProcessCell(linear, X, Y, cell.clr2, dir, ret);
			}
		}
		else
		{
			const unsigned int color = cell.type == CellType::ColorSwitcher ? cell.clr3 : cell.clr1;
			const std::vector<std::pair<char, char>>& dir = directions[linear];
			ret = ProcessCell(linear, X, Y, color, dir, ret);
		}
    }

	return ret;
}

void Level::Reload()
{
	if (fromEditor)
		*this = editLevel;
	else
		*this = Level(path.c_str());
}

