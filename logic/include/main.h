#pragma once

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

	void changeLanguage(void);
	void loadNewWords(void);
	void cancelMistake(void);

	StateType getCurrentState(void) const;
	std::wstring getCurrentQuestion(void) const;
	std::wstring getCurrentAnswer(void) const;

	std::wstring getStatisticInformation(void) const;
};

};