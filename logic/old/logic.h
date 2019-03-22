#pragma once

#include <string>
#include <vector>

class Date;
struct Word;
class WordStorage;
struct AnswerStatistic;
class WordStorageWithDayStatistics;
class WordGetter;
class StandardWordGetter;
class RandomWordGetter;

//-----------------------------------------------------------------------------
class Date
{
public:
	Date(int day, int month, int year);

	void setTodayDate(void);

	int getDay(void) const;
	int getMonth(void) const;
	int getYear(void) const;

	int calcDayDistance(const Date& a) const;
private:

};

std::ostream& operator<<(std::ostream out, const Date& d);
std::istream& operator>>(std::istream in, Date& d);

//-----------------------------------------------------------------------------
typedef unsigned long Milliseconds;

struct Word
{
	const int levelChange = 5;

	enum Rang
	{
		UNKNOWN,
		NEGATIVE,
		POSITIVE_0,
		POSITIVE_1,
		POSITIVE_2,
		POSITIVE_3,
		KNOWN,
	};

	std::wstring first;
	std::wstring second;

	Rang rang;
	int count;
	Date lastRepeatDate;

	Milliseconds msCount; // Суммарное время в миллисекундах ответа на слово
};

std::ostream& operator<<(std::ostream out, const WordRating& d);
std::istream& operator>>(std::istream in, WordRating& d);

//-----------------------------------------------------------------------------
/** Хранилище слов, хранит слова и всю статистику о них. */
class WordStorage
{
public:
	WordStorage();
	void load(std::istream in);
	void save(std::ostream out) const;

	int size(void) const;

	int addWord(std::wstring first, std::wstring second);
	const Word& getWord(int wordId) const;

	bool canAsk(int wordId) const; // Можно ли данное слово спрашивать. Слово нельзя спрашивать, когда оно на сегодня достигло смены статуса, и только спустя 7 дней после последнего спрашивания
	void answer(int wordId, bool isCorrect);

	void startAnswerTimer(int wordId);
	void endAnswerTimer(int wordId);

	//-------------------------------------------------------------------------
	AnswersStatistic getSessionStatistics(void) const;
private:
	std::vector<Word> m_words;
	AnswersStatistic m_sessionStat;
};

//-----------------------------------------------------------------------------
struct AnswersStatistic
{
	int count;
	int right;
	int wrong;
};

struct DayStatistic
{
	AnswerStatistic ans;
	Date day;
};

std::ostream& operator<<(std::ostream out, const DayStatistic& d);
std::istream& operator>>(std::istream in, DayStatistic& d);

class WordStorageWithDayStatistics
{
	WordStorageWithDayStatistics(const WordStorage& words); // Если при считывании оказалось, что сегодняшнего дня нет, то создать бекап в папке для бекапов, если файла с именем предыдущей даты не существует
	void load(std::istream in);
	void save(std::ostream out) const;

	// Хранится у себя внутри, и частично вычисляется через wordstorage
	int getAnswersCount(bool isToday) const;
	int getAnswersRight(bool isToday) const;
	int getAnswersWrong(bool isToday) const;

	// Вычисляется на основе wordstorage
	int getWordsCount(void) const;
	int getWordsUnknownCount(void) const;
	int getWordsNegativeCount(void) const;
	int getWordsPositive0Count(void) const;
	int getWordsPositive1Count(void) const;
	int getWordsPositive2Count(void) const;
	int getWordsPositive3Count(void) const;
	int getWordsKnownCount(void) const;

	// Вычисляется на основе wordstorage
	Milliseconds getAllTime(void) const;
	Milliseconds getAverageAnswerTime(void) const;
	Milliseconds getAverageKnownWordTime(void) const;
public:
	const WordStorage& words;
	std::vector<DayStatistics> m_stat;
};

//-----------------------------------------------------------------------------
class WordGetter
{
public:
	WordGetter(WordStatStorage& lang1, WordStatStorage& lang2);
	void load(std::istream in);
	void save(std::ostream out) const;

	virtual ~WordGetter() {}

	virtual void getQuestion(std::wstring question,
							 std::wstring answer,
							 std::wstring pattern,
							 bool& isUnknown) = 0;

	void startAnswerTimer(void);
	void endAnswerTimer(void);

	bool isCorrect(std::wstring answer);
	bool answer(std::wstring answer);

	void swapLanguage(void);
private:
	WordStatStorage&	m_storage1;
	WordStatStorage&	m_storage2;
	bool m_isFirstLang;

	int m_currentWord;

	virtual void calcNewWordStack(void) = 0;
};

//-----------------------------------------------------------------------------
class StandardWordGetter : public WordGetter
{
public:
	StandardWordGetter(WordStatStorage& lang1, WordStatStorage& lang2);

	void getQuestion(std::wstring question,
					 std::wstring pattern);
private:
	void calcNewWordStack(void);
};

//-----------------------------------------------------------------------------
class RandomWordGetter : public WordGetter
{
public:
	RandomWordGetter(WordStatStorage& lang1, WordStatStorage& lang2);

	void getQuestion(std::wstring question,
					 std::wstring pattern);
private:
	void calcNewWordStack(void);	
};