#pragma once

#include <string>

namespace logic
{

enum StateType
{
	STATE_UNKNOWN,  // Неизвестное слово
	STATE_NEGATIVE, // Слово с положительным рейтингом
	STATE_POSITIVE  // Слово с отрицательным рейтингом
};

class MainHandler
{
public:
	MainHandler();

    void save();

	void changeLanguage(void);
    void loadNewWords(const std::wstring& file);
	void cancelMistake(void);

	StateType getCurrentState(void) const;
	std::wstring getCurrentQuestion(void) const;
    bool answer(const std::wstring& str);

	std::wstring getStatisticInformation(void) const;

    int getTodayWorkTime(void) const;
    int getAllWorkTime(void) const;
};

};
