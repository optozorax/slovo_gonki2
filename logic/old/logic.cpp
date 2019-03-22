#include "logic.h"

//-----------------------------------------------------------------------------
Date getTodayDate(void) {
	/// TODO
	return {};
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
WordStatStorage::WordStatStorage(std::wstring file) {
	// Если при считывании статистика за какой-то день не являегося сегодняшней, то прибавлять её к общей.
}

//-----------------------------------------------------------------------------
int WordStatStorage::size(void) const {
	return 1;
}

//-----------------------------------------------------------------------------
int WordStatStorage::addWord(Word word) {
	// обновить кэш неизученных слов
	return {};
}

//-----------------------------------------------------------------------------
Word WordStatStorage::getWord(int wordId) const {
	return m_words[wordId];
}

//-----------------------------------------------------------------------------
void WordStatStorage::answer(int wordId, bool isCorrect) {
	auto& stat = m_wordStat[wordId];
	if (isCorrect) {
		if (stat.rating.count < 4) {
			stat.rating.count++;
		} else {
			stat.rating.count = 0;
			switch (stat.status) {
				case WORD_UNKNOWN: {
					stat.status = WORD_POSITIVE_0;
					m_cachedUnstudiedWordsCount--;
					m_
				} break;
				case WORD_NEGATIVE: {
					stat.status = WORD_POSITIVE_0;
				} break;
				case WORD_POSITIVE_0: {
					stat.status = WORD_POSITIVE_1;
				} break;
				case WORD_POSITIVE_1: {
					stat.status = WORD_POSITIVE_2;
				} break;
				case WORD_POSITIVE_2: {
					stat.status = WORD_POSITIVE_3;
				} break;
				case WORD_POSITIVE_3: {
					stat.status = WORD_KNOWN;
				} break;
				case WORD_KNOWN: {
					throw std::exception();
				} break;
			}
		}
	} else {
		stat.rating.count = 0;
		stat.rating.status = WORD_NEGATIVE;
	}
}

//-----------------------------------------------------------------------------
void WordStatStorage::startAnswerTimer(void) {

}

//-----------------------------------------------------------------------------
void WordStatStorage::endAnswerTimer(void) {

}

//-----------------------------------------------------------------------------
std::vector<int> WordStatStorage::getAllMeanings(int wordId) const {
	return {};
}

//-----------------------------------------------------------------------------
WordRating WordStatStorage::getRating(int word) const {
	return m_wordStat[word].rating;
}

//-----------------------------------------------------------------------------
Date WordStatStorage::getAddDate(int word) const {
	return m_wordStat[word].addDate;
}

//-----------------------------------------------------------------------------
Date WordStatStorage::getLastRepeatDate(int word) const {
	return m_wordStat[word].getLastRepeatDate;

}

//-----------------------------------------------------------------------------
int WordStatStorage::getAverageAnswerTime(void) const {
	return m_ever.answerTime / m_ever.answersCount;
}

//-----------------------------------------------------------------------------
int WordStatStorage::getAllTime(StatisticRange range) const {
	if (range == STAT_TODAY)
		return m_today.allTime;
	else
		return m_even.allTime;
}

//-----------------------------------------------------------------------------
int WordStatStorage::getAnswersCount(StatisticRange range) const {
	if (range == STAT_TODAY)
		return m_today.answersCount;
	else
		return m_even.answersCount;
}

//-----------------------------------------------------------------------------
int WordStatStorage::getRightAnswersCount(StatisticRange range) const {
	if (range == STAT_TODAY)
		return m_today.rightAnswersCount;
	else
		return m_even.rightAnswersCount;
}

//-----------------------------------------------------------------------------
int WordStatStorage::getWrongAnswersCount(StatisticRange range) const {
	if (range == STAT_TODAY)
		return m_today.wrongAnswersCount;
	else
		return m_even.wrongAnswersCount;
}

//-----------------------------------------------------------------------------
int WordStatStorage::getUnstidedWordsCount(StatisticRange range) const {
	return m_cachedUnstudiedWordsCount;
}

//-----------------------------------------------------------------------------
int WordStatStorage::getNegativeWordsCount(StatisticRange range) const {
	return m_cachedNegativeWordsCount;
}

//-----------------------------------------------------------------------------
int WordStatStorage::getPositiveWordsCount(StatisticRange range, int repeat) const {
	return m_cachedPositiveWordsCount;
}

//-----------------------------------------------------------------------------
int WordStatStorage::getStudiedWordsCount(StatisticRange range) const {
	return m_cachedStudiedWordsCount;
}

//-----------------------------------------------------------------------------
void WordStatStorage::save(std::wstring file) const {

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
WordGetter::WordGetter(WordStatStorage& lang1, WordStatStorage& lang2) : m_storage1(lang1), m_storage2(lang2) {

}

//-----------------------------------------------------------------------------
bool WordGetter::isCorrect(std::wstring answer) {
	return {};
}

//-----------------------------------------------------------------------------
bool WordGetter::answer(std::wstring answer) {
	return {};
}

//-----------------------------------------------------------------------------
void WordGetter::swapLanguage(void) {
	
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
StandardWordGetter::StandardWordGetter(WordStatStorage& lang1, WordStatStorage& lang2) : WordGetter(lang1, lang2) {

}

//-----------------------------------------------------------------------------
void StandardWordGetter::getQuestion(std::wstring question, std::wstring pattern) {

}

//-----------------------------------------------------------------------------
void StandardWordGetter::calcNewWordStack(void) {

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RandomWordGetter::RandomWordGetter(WordStatStorage& lang1, WordStatStorage& lang2) : WordGetter(lang1, lang2) {

}

//-----------------------------------------------------------------------------
void RandomWordGetter::getQuestion(std::wstring question, std::wstring pattern) {

}

//-----------------------------------------------------------------------------
void RandomWordGetter::calcNewWordStack(void) {

}