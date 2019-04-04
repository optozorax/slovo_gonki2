#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(200, 200);
    createWidgets();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createWidgets(){

    /* Объявление переменных */
    stat = new QLabel("Stat");
    word = new QLabel("Word");
    answer = new QLineEdit("Answer");

    menuBar = new QMenuBar(this);
    menu = new QMenu("&Menu");

    vertLayer = new QVBoxLayout();
    mainLayer = new QVBoxLayout();
    horLayer = new QHBoxLayout();

    lang = new QAction("&Language");
    addWords = new QAction("&Add words");
    disableStat = new QAction("&Disable statistic");
    cancelMist = new QAction("&Cancel mistake");
    thema = new QAction("&Night thema");

    checkDisStat = new QCheckBox();
    checkThema = new QCheckBox();

    font = new QFont();

    /* Горячие клавиши */
    cancelMist->setShortcut(Qt::CTRL + Qt::Key_D);

    /* Меню */
    menu->addAction(lang);
    menu->addAction(addWords);
    menu->addAction(disableStat);
    menu->addAction(cancelMist);
    menu->addAction(thema);

    /* Статистика */
    disableStat->setCheckable(true);

    /* Тема */
    thema->setCheckable(true);

    /* Параметры меню */
    menuBar->addMenu(menu);
    menuBar->setMinimumWidth(frameGeometry().width());
    menuBar->setMaximumHeight(20);

    /* Расположение текста */
    stat->setAlignment(Qt::AlignCenter);
    word->setAlignment(Qt::AlignCenter);
    answer->setAlignment(Qt::AlignCenter);

    /* Слой 1 */
    vertLayer->addWidget(word);
    vertLayer->addWidget(answer);

    /* Слой 2 */
    horLayer->addLayout(vertLayer);
    horLayer->addWidget(stat);

    setLayout(horLayer);

    /* События */
    connect(disableStat, &QAction::triggered, [this](){slotHideStat(checkDisStat);});
    connect(thema, &QAction::triggered, [this](){slotThema(checkThema);});
}

/* Динамическое окно и увеличение размера шрифта */
void MainWindow::resizeEvent(QResizeEvent *event){
    menuBar->setMinimumWidth(frameGeometry().width());
    stat->setMinimumSize(geometry().width()/2, geometry().height()/2);
    word->setMinimumSize(geometry().width()/2, geometry().height()/2);
    answer->setMinimumSize(geometry().width()/2, geometry().height()/2);

    font->setPixelSize(this->size().height() / 15);

    stat->setFont(*font);
    word->setFont(*font);
    answer->setFont(*font);
}

/* Отключение статистики */
void MainWindow::slotHideStat(QCheckBox *checkDisStat){
    if (checkDisStat->checkState() != Qt::Checked){
        checkDisStat->setCheckState(Qt::Checked);
        stat->hide();
    }

    else {
        checkDisStat->setCheckState(Qt::Unchecked);
        stat->show();
    }
}

/* Тема */
void MainWindow::slotThema(QCheckBox *checkThema){
    if(checkThema->checkState() != Qt::Checked){
        checkThema->setCheckState(Qt::Checked);
        MainWindow::setStyleSheet("QLineEdit { border: rgb(19, 39, 71); background-color: rgb(25, 38, 61); color: rgb(168, 171, 175); } \
                                  QLabel { color: rgb(168, 171, 175); } \
                                  MainWindow { background-color: rgb(25, 38, 61); } ");
    }

    else {
        checkThema->setCheckState(Qt::Unchecked);
        MainWindow::setStyleSheet("default");
    }
}
