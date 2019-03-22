#pragma once

#include <map>

#include "logic.h"
#include <twg/twg.h>
#include <twg/ctrl/brain.h>
#include <twg/ctrl/menu.h>
#include <twg/image/image_drawing.h>

//-----------------------------------------------------------------------------
struct TextFragment
{
	std::wstring text;
	twg::Color textColor;
	twg::Color backColor;
};

void drawText(
	twg::ImageBase* buffer, 
	const std::vector<TextFragment>& text, 
	twg::Point_i pos,
	bool isOnCenter,
	int fontSize,
	int maxWidth
);

//-----------------------------------------------------------------------------
enum PuntoLang
{
	LANG_UNKNOWN,
	LANG_FIRST,
	LANG_SECOND
};

class PuntoSwitcher
{
public:
	PuntoSwitcher(std::wstring file);

	PuntoLang getLang(wchar_t c);
	wchar_t getChar(wchar_t c, int cLang);
	wchar_t makeNormal(wchar_t c); // ё to е, big letters to small

private:
	std::map<wchar_t, std::vector<PuntoLang>> m_langMap;
	std::map<wchar_t, wchar_t> m_first2second;
	std::map<wchar_t, wchar_t> m_second2first;
	std::map<wchar_t, wchar_t> m_normal;
};

//-----------------------------------------------------------------------------
/** Основные мозги программы. Получает все сообщения, обрабатывает их. */
class MainHandler : public twg::BrainCtrl
{
public:
	MainHandler(EventsBase* parent);
	~MainHandler();

	void load(std::istream in);
	void save(std::ostream out) const;

	void init(void);
	void makeMenu(void);

	//-------------------------------------------------------------------------
	bool onMessageNext(twg::int32u messageNo, void* data); // Обработка меню
	bool onKeyboard(twg::KeyType key, bool isDown); // Основная логика программы и обработка набора слов
	bool onResize(twg::Rect rect, twg::SizingType type); // Сохранение настроек программы
	bool onMove(twg::Point_i newPos);
	void draw(twg::ImageBase* buffer);

private:
	enum Regime
	{
		REGIME_NEXT_WORD,
		REGIME_ENTER_QUESTION,
		REGIME_ENTER_ANSWER,
		REGIME_ENTER_KNOWN_ANSWER,
		REGIME_RESULT
	};

	Regime m_regime;

	//WordRating m_rating;
	std::wstring question;
	std::wstring pattern;
	std::wstring correctAnswer;
	std::wstring currentText;

	std::vector<bool> correctSymbols;
	HKL m_layout;
	
	
	//WordStatStorage	m_words1;
	//WordStatStorage m_words2;

	//StandardWordGetter	m_getter1;
	//RandomWordGetter	m_getter2;

	twg::StaticMenu	m_menu;
	bool m_drawStat;
	bool m_isCorrectAnswer;
	bool m_isUnknownWord;
	bool m_isMistakeValid;
	bool m_isRandomRegime;

	PuntoSwitcher m_switcher;

	void saveSettings(void);
	void loadSettings(void);
	void addWords(void);
	void remakeScreen(void);
};