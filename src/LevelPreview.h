#pragma once
#include <string>

enum class LevelType : unsigned int
{
	Classic = 0,
	Building,
	Moving,
	Rotating
};

enum class LevelDifficulty : unsigned int
{
	Instruction = 0,
	Easy,
	Medium,
	Hard,
	MegaHard
};

class LevelPreview
{
protected:
	std::wstring path;

public:
	unsigned int levelId = 1;
	LevelType type = LevelType::Classic;
	LevelDifficulty difficulty = LevelDifficulty::Instruction;
	bool completed = false;

	LevelPreview();
	LevelPreview(const wchar_t* path);

    void LoadFromFile();

	bool operator<(const LevelPreview& other) const;

	const std::wstring& GetPath() const;
	const wchar_t* GetTypeName() const;
	const wchar_t* GetDifficultyName() const;
	const wchar_t* GetIsCompletedName() const;
    const wchar_t* GetLevelHint() const;
};
