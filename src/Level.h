#pragma once
#include "LevelPreview.h"
#include "MyColor.h"
#include "Cell.h"
#include <vector>
#include <queue>

enum class LevelState : unsigned int
{
	NotFinished = 0,
	Completed,
	LostStocks, // не все стоки заполнены
	LostLeaked, // вода вытекла
	LostMixed, // вода смешалась
	LostStockError // сток заполнен водой не того цвета
};

class Level : public LevelPreview
{
private:
	void LoadFromFile();
	void LoadDefault();
	bool CheckBounds(int X, int Y) const;
public:
	bool fromEditor = false;

	unsigned int rowCount = 5;
	unsigned int colCount = 5;
	unsigned int time = 60;
	unsigned int colorsCount = 1;

	unsigned int bombsCount = 3;
	unsigned int bombsIncCounter = 0;
	bool secondPhase = false;
    Cell randomCell = Cell();

    std::vector<MyColor> colorMap;
	std::vector<Cell> cells;
	std::vector<bool> highlighted;
	std::vector<std::vector<std::pair<char, char>>> directions;
	std::queue<unsigned int> bfs;

	Level();
    Level(bool fromEditor);
	Level(const wchar_t* path, bool fromEditor = false);

    void WriteToFile(const wchar_t* path) const;
	void RegenerateCell();
	void SwitchToSecondPhase();
	LevelState ProcessCell(unsigned int linear, int X, int Y, unsigned int color,
		const std::vector<std::pair<char, char>>& dir, LevelState ret);
	LevelState GameTick();
	void UpdateCompleted() const;
	void Reload();
	void FillDefaultColors(unsigned int l, unsigned int r);
};
