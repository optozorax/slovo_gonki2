#include <map>
#include <sstream>
#include <fstream>

#include <twg/image/image_drawing.h>

#include "interface.h"

using namespace twg;

//-----------------------------------------------------------------------------
void drawText(ImageBase* buffer, const std::vector<TextFragment>& text, Point_i pos, bool isOnCenter, int fontSize, int maxWidth) {
	ImageDrawing_aa img(buffer);
	img.setTextStyle(TextStyle(fontSize, L"Consolas", TEXT_NONE));

	std::wstring str;
	std::map<int, bool> txtMap;
	int pos33 = 0;
	for (const auto& i : text)
		for (const auto& j : i.text) {
			if (j == L'\n')
				txtMap[pos33] = true;
			str += j;
			pos33++;
		}
			

	start:
	for (int i = 0; i <= str.size(); ++i) {
		if (img.getTextSize(str.substr(0, i)).x > maxWidth) {
			str.insert(i-1, L"\n");
			goto start;
		}
	}

	auto size = img.getTextSize(str);
	if (isOnCenter)
		pos -= size/2;

	auto changeSymbols = [&txtMap] (std::wstring str1, std::wstring str2, int pos) -> std::wstring {
		int pos2 = 0;
		int noff = 0;
		for (int i = 0; i < str1.size(); ++i) {
			if (str1[i] != L'\n') {
				if (pos2 >= pos && pos2 < (pos + str2.size()))
					str1[i] = str2[pos2 - pos];
				else
					str1[i] = L' ';
				pos2++;
			} else {
				if (txtMap[i - noff])
					pos2++;
				else 
					noff++;
			}
		}
		return str1;
	};

	int pos1 = 0;
	for (const auto& i : text) {
		auto strPart = changeSymbols(str, i.text, pos1);
		std::wstring back;
		for (int j = 0; j < i.text.size(); ++j)
			if (i.text[j] != L'\n')
				back += L'█';
			else
				back += L'\n';
		auto strBack = changeSymbols(str, back, pos1);

		if (i.backColor != Transparent) {
			img.setPen(Pen(1, i.backColor));
			img.drawText(pos, strBack);
		}
		if (i.textColor != Transparent) {
			img.setPen(Pen(1, i.textColor));
			img.drawText(pos, strPart);
		}

		pos1 += i.text.size();
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
MainHandler::MainHandler(EventsBase* parent) : 
	BrainCtrl(parent),
	m_regime(REGIME_NEXT_WORD),
	//m_rating({WORD_UNKNOWN, 0}),
	m_menu(L"", parent),
	//m_words1(L"words_1.txt"),
	//m_words2(L"words_2.txt"),
	//m_getter1(m_words1, m_words2),
	//m_getter2(m_words1, m_words2),
	m_drawStat(true),
	m_isRandomRegime(false),
	m_switcher(L"punto.txt") {
	m_layout = GetKeyboardLayout(GetCurrentThreadId());
	loadSettings();
	onKeyboard(KEY_RETURN, true);
}

//-----------------------------------------------------------------------------
MainHandler::~MainHandler() {
	saveSettings();
}

//-----------------------------------------------------------------------------
void MainHandler::init(void) {
	makeMenu();
	m_wnd->worthRedraw();
}

//-----------------------------------------------------------------------------
void MainHandler::makeMenu(void) {
	// Создание меню в соответствии с текущими данными
	std::wstringstream sout;
	sout << L"=100 Swap language | =101 Add words | =102 ";
	if (m_drawStat) 
		sout << L"Disable";
	else
		sout << L"Enable";
	sout << L" statistic | =103 Cancel mistake | Regime >";
	if (m_isRandomRegime)
		sout << " =110 Normal | =111 ! Random <";
	else
		sout << " =110 ! Normal | =111 Random <";
	m_menu.change(sout.str());
}

//-----------------------------------------------------------------------------
bool MainHandler::onMessageNext(int32u messageNo, void* data) {
	// Обработка меню
	if (messageNo == MENU_CLICK) {
		int id = *((int32u*)data);

		// Порядок языка
		if (id == 100) {
			//m_getter1.swapLanguage();
			//m_getter2.swapLanguage();
		} else

		// Добавление слов
		if (id == 101) {
			addWords();
		} else

		// Скрыть\показать статистику
		if (id == 102) {
			m_drawStat = !m_drawStat;
			makeMenu();
		} else

		// Отменить ошибку
		if (id == 3) {
			m_isMistakeValid = false;	
		} else 

		// Смена режима подбора слов
		if (id == 110) {
			m_isRandomRegime = false;
			remakeScreen();
		} else 
		if (id == 111) {
			m_isRandomRegime = true;
			remakeScreen();
		}
	} else 
		return false;

	return true;
}

//-----------------------------------------------------------------------------
bool MainHandler::onKeyboard(KeyType key, bool isDown) {
	// Обработка нажатий клаивиш и основное управление
	bool enter = false;
	bool backspace = false;
	std::wstring symbols;
	wchar_t symbol;
	if (m_regime == REGIME_NEXT_WORD) {
		// Получить вопрос, ответ, шаблон, рейтинг слова.
		question = L"hello";
		pattern = L"______";
		correctAnswer = L"привет";
		currentText = L"";
		m_regime = REGIME_ENTER_QUESTION;
		//if (m_rating.status == WORD_UNKNOWN)
			//m_rating = {WORD_POSITIVE_2, 2};
		//m_isUnknownWord = m_rating.status == WORD_UNKNOWN;
		m_isCorrectAnswer = false;
		m_isMistakeValid = true;
		// Стартуется таймер
	} else
	if (!isDown) {
		if (key == KEY_RETURN) {
			enter = true;
		} else
		if (key == KEY_BACK) {
			backspace = true;
		} else {
			symbols = key2wstr(key);
			if (symbols == L"")
				return true;
			if (symbols.size() != 1)
				throw std::exception();
			symbol = m_switcher.makeNormal(symbols[0]);
			auto qLang = m_switcher.getLang(question[0]);
			auto aLang = m_switcher.getLang(correctAnswer[0]);
			auto sLang = m_switcher.getLang(symbol);
			if (qLang != LANG_UNKNOWN && aLang != LANG_UNKNOWN) {
				if (m_regime == REGIME_ENTER_QUESTION) {
					if (sLang != qLang)
						symbol = m_switcher.getChar(symbol, sLang);
				} else
				if (m_regime == REGIME_ENTER_ANSWER || m_regime == REGIME_ENTER_KNOWN_ANSWER) {
					if (sLang != aLang)
						symbol = m_switcher.getChar(symbol, sLang);
				}
			}
		}

		if (backspace) {
			if (!currentText.empty())
				currentText.erase(currentText.begin() + currentText.size() - 1);
		} else
		if (m_regime == REGIME_ENTER_QUESTION) {
			if (!enter) {
				currentText += symbol;

				if (currentText == question) {
					currentText.clear();
					if (m_rating.status == WORD_UNKNOWN)
						m_regime = REGIME_ENTER_KNOWN_ANSWER;
					else
						m_regime = REGIME_ENTER_ANSWER;
				}
			}
		} else if (m_regime == REGIME_ENTER_ANSWER) {
			if (!enter) {
				currentText += symbol;
			} else {
				m_isCorrectAnswer = currentText == correctAnswer;
				currentText.clear();
				m_regime = REGIME_RESULT;
			}
		} else if (m_regime == REGIME_ENTER_KNOWN_ANSWER) {
			if (!enter) {
				currentText += symbol;

				if (currentText == correctAnswer) {
					m_regime = REGIME_RESULT;
					m_isCorrectAnswer = true;
				}
			}
		} else if (m_regime == REGIME_RESULT) {
			if (enter) {
				m_regime = REGIME_NEXT_WORD;
				/// TODO
				if (m_isCorrectAnswer)
					m_rating.count++;
				else
					m_rating = {WORD_NEGATIVE, 0};
				onKeyboard(key, isDown);
			}
		}

		m_wnd->worthRedraw();
	}
	return true;
}

//-----------------------------------------------------------------------------
bool MainHandler::onResize(Rect rect, SizingType type) {
	saveSettings();
	return true;
}

//-----------------------------------------------------------------------------
bool MainHandler::onMove(Point_i newPos) {
	saveSettings();
	return false;
}

//-----------------------------------------------------------------------------
void MainHandler::draw(ImageBase* buffer) {
	buffer->clear(White);

	auto makeRightWrite = [] (std::vector<TextFragment>& text, std::wstring question, std::wstring currentText) {
		// Подсчет правильных симолов
		std::vector<bool> correctSymbols;
		for (int i = 0; i < currentText.size() && i < question.size(); i++)
			correctSymbols.push_back({currentText[i] == question[i]});

		for (int i = 0; i < currentText.size(); i++) {
			if (correctSymbols[i]) {
				text.push_back({std::wstring(L"") + question[i], Black, Transparent});
			} else {
				text.push_back({currentText.substr(i, currentText.size() - i), Red, Transparent});
				break;
			}
		}
		if (currentText.size() <= question.size())
			text.push_back({question.substr(currentText.size(), question.size() - currentText.size()), Gray, Transparent});
		else
			text.push_back({currentText.substr(question.size(), currentText.size() - question.size()), Red, Transparent});
	};

	Point_i center1(100 + 10, 10 + (m_wnd->getClientSize().y-25)/4);
	Point_i center2(100 + 10, 10 + (m_wnd->getClientSize().y-25)*3/4);

	// Создаем текст для первой части
	std::vector<TextFragment> text;
	if (m_regime == REGIME_ENTER_QUESTION) {
		makeRightWrite(text, question, currentText);
	} else
		text.push_back({question, Black, Transparent});
	drawText(buffer, text, center1, true, 30, 200);

	// Создаем текст для второй части
	if (m_regime == REGIME_ENTER_ANSWER) {
		std::vector<TextFragment> text2;
		std::wstring current = currentText + pattern.substr(currentText.size(), pattern.size() - currentText.size());
		text2.push_back({current, Black, Transparent});
		drawText(buffer, text2, center2, true, 30, 200);
	} else if (m_regime == REGIME_ENTER_KNOWN_ANSWER) {
		std::vector<TextFragment> text2;
		makeRightWrite(text2, correctAnswer, currentText);
		drawText(buffer, text2, center2, true, 30, 200);
	} else {
		std::vector<TextFragment> text2;
		if (m_regime == REGIME_RESULT)
			if (m_isCorrectAnswer)
				text2.push_back({correctAnswer, Green, Transparent});
			else
				text2.push_back({correctAnswer, Red, Transparent});
		else
			if (m_isUnknownWord)
				text2.push_back({correctAnswer, Gray, Transparent});
			else
				text2.push_back({pattern, Black, Transparent});
		drawText(buffer, text2, center2, true, 30, 200);
	}

	Point_d size(200, m_wnd->getClientSize().y - 25);

	Polygon_d poly;
	poly.array.push_back({10, 10});
	poly.array.push_back(poly.array[0] + Point_d(size.x, 0));
	poly.array.push_back(poly.array[1] + Point_d(0, size.y/2));
	poly.array.push_back(poly.array[0] + Point_d(0, size.y/2));

	Polygon_d poly2;
	poly2.array.push_back(poly.array[3] + Point_d(0, 5));
	poly2.array.push_back(poly2.array[0] + Point_d(size.x, 0));
	poly2.array.push_back(poly2.array[1] + Point_d(0, size.y/2));
	poly2.array.push_back(poly2.array[0] + Point_d(0, size.y/2));

	ImageDrawing_aa img(buffer);
	img.setPen(Pen(2, Black));
	img.drawPolyline(poly, true);
	img.drawPolyline(poly2, true);

	std::vector<TextFragment> textR;
	auto makeProgress = [&textR] (int count, Color color, int rating) {
		for (int i = 0; i < rating; ++i)
			textR.push_back({L"+", color, Transparent});
		for (int i = 0; i < 4 - rating; ++i)
			textR.push_back({L" ", color, Transparent});
		for (int i = 0; i < count; ++i)
			textR.push_back({L"●", color, Transparent});
		for (int i = 0; i < 5 - count; ++i)
			textR.push_back({L"○", color, Transparent});
	};
	switch (m_rating.status) {
		case WORD_UNKNOWN: {
			makeProgress(m_rating.count, Gray, 0);
		} break;
		case WORD_NEGATIVE: {
			makeProgress(m_rating.count, Red, 0);
		} break;
		case WORD_POSITIVE_0: {
			makeProgress(m_rating.count, Green, 1);
		} break;
		case WORD_POSITIVE_1: {
			makeProgress(m_rating.count, Green, 2);
		} break;
		case WORD_POSITIVE_2: {
			makeProgress(m_rating.count, Green, 3);
		} break;
		case WORD_POSITIVE_3: {
			makeProgress(m_rating.count, Green, 4);
		} break;
	}
	drawText(buffer, textR, poly2.array[0] - Point_d(-1, 4), false, 20, 200);

	/// TODO statistics
}

//-----------------------------------------------------------------------------
void MainHandler::saveSettings(void) {
	/*
		Размер и положение окна
		Текущий язык
		Рисовать ли статистику
		Текущий режим
	*/
	/// TODO
}

//-----------------------------------------------------------------------------
void MainHandler::loadSettings(void) {
	/// TODO
}

//-----------------------------------------------------------------------------
void MainHandler::addWords(void) {
	const std::wstring wordsFile = L"new_words.txt";
	/// TODO
}

//-----------------------------------------------------------------------------
void MainHandler::remakeScreen(void) {
	/// TODO
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
PuntoSwitcher::PuntoSwitcher(std::wstring file) {
	std::wifstream fin(file);
	int n, m;
	fin >> n >> m;
	for (int i = 0; i < n; ++i) {
		wchar_t a, b;
		fin >> a >> b;
		m_langMap[a].push_back(LANG_FIRST);
		m_langMap[b].push_back(LANG_SECOND);
		m_first2second[a] = b;
		m_second2first[b] = a;
	}

	for (int i = 0; i < m; ++i) {
		wchar_t a, b;
		fin >> a >> b;
		m_normal[a] = b;
	}
}

//-----------------------------------------------------------------------------
PuntoLang PuntoSwitcher::getLang(wchar_t c) {
	 if (m_langMap[c].size() == 1)
	 	return m_langMap[c][0];
	 else
	 	return LANG_UNKNOWN;
}

//-----------------------------------------------------------------------------
wchar_t PuntoSwitcher::getChar(wchar_t c, int cLang) {
	if (cLang == LANG_UNKNOWN)
		throw std::exception();

	if (cLang == LANG_FIRST)
		return m_first2second[c];
	else
		return m_second2first[c];
}

//-----------------------------------------------------------------------------
wchar_t PuntoSwitcher::makeNormal(wchar_t c) {
	if (m_normal[c] == 0)
		return c;
	else
		return m_normal[c];
}