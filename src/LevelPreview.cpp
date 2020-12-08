#include "LevelPreview.h"
#include "Constants.h"

LevelPreview::LevelPreview()
	: path(L"")
{
}

LevelPreview::LevelPreview(const wchar_t* path)
	: path(path)
{
}

void LevelPreview::LoadFromFile()
{
	if (!path.length())
		return;
	FILE* f = _wfopen(path.c_str(), L"r");
	if (!f)
		return;

	fscanf_s(f, "%u%u%u%d", &levelId, &type, &difficulty, &completed);

	fclose(f);
}

bool LevelPreview::operator<(const LevelPreview& other) const
{
	return levelId < other.levelId;
}

const std::wstring& LevelPreview::GetPath() const
{
	return path;
}

const wchar_t* LevelPreview::GetTypeName() const
{
	switch (type)
	{
	case LevelType::Classic:
		return L"Классический";
	case LevelType::Building:
		return L"Строительство";
	case LevelType::Moving:
		return L"Перемещение";
	case LevelType::Rotating:
		return L"Вращение";
	default:
		return L"Классический";
	}
}

const wchar_t* LevelPreview::GetDifficultyName() const
{
	switch (difficulty)
	{
	case LevelDifficulty::Instruction:
		return L"Обучение";
	case LevelDifficulty::Easy:
		return L"Легкий";
	case LevelDifficulty::Medium:
		return L"Средний";
	case LevelDifficulty::Hard:
		return L"Сложный";
	case LevelDifficulty::MegaHard:
		return L"Мегасложный";
	default:
		return L"Обучение";
	}
}

const wchar_t* LevelPreview::GetIsCompletedName() const
{
	return completed ? L"Пройден" : L"Не пройден";
}

const wchar_t* LevelPreview::GetLevelHint() const
{
	static const UnicodeString baseHint = L"Цель игры: заполнить все стоки водой. \
Воду выливать или смешивать нельзя. Универсальный сток (по умолчанию \
серый) может принять воду любого цвета (используйте в случае избытка воды). \
Его тоже обязательно заполнять. Остальные стоки могут принимать только воду своего цвета.\n\n";
	switch (type)
	{
	case LevelType::Classic:
		return (baseHint + L"В этом режиме вы можете ставить только определенные (случайно выданные) \
трубы. Удалять можно только с помощью взрывчатки (выдается каждые " +
UIntToStr(Stuff::gSpawnBombCounter) + " поставленных труб). \
Управление: ЛКМ - поставить; СКМ - свободный просмотр; ПКМ - удалить.").c_str();
		break;
	case LevelType::Building:
		return (baseHint + L"В этом режиме вы можете ставить любые выданные \
трубы. Управление: ЛКМ - поставить; СКМ - свободный просмотр; ПКМ - удалить.").c_str();
		break;
	case LevelType::Moving:
		return (baseHint + L"В этом режиме вы можете менять местами любые предметы, под которыми \
нет камня. Удаления нет. Управление: ЛКМ - поменять местами; СКМ - свободный просмотр.").c_str();
		break;
    case LevelType::Rotating:
		return (baseHint + L"В этом режиме вы можете вращать любые предметы, под которыми \
нет камня. Удаления нет. Управление: ЛКМ - вращать; СКМ - свободный просмотр.").c_str();
		break;
    }
}

