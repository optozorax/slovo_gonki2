#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>

#include <QMenuBar>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QLabel>
#include <QLineEdit>

#include <QCheckBox>

#include <QFont>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QLabel *stat;
    QLabel *word;

    QLineEdit *answer;

    QVBoxLayout *vertLayer;
    QVBoxLayout *mainLayer;
    QHBoxLayout *horLayer;

    QMenuBar *menuBar;
    QMenu *menu;

    QAction *lang;
    QAction *addWords;
    QAction *disableStat;
    QAction *cancelMist;
    QAction *thema;

    QCheckBox *checkDisStat;
    QCheckBox *checkThema;

    QFont *font;

private:
    void createWidgets();
    virtual void resizeEvent(QResizeEvent *event);

private slots:
    void slotHideStat(QCheckBox *chek);
    void slotThema(QCheckBox *checkThema);

//private slots:
};

#endif // MAINWINDOW_H
