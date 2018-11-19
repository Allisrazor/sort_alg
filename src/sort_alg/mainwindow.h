#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include "solver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void NumberButtonAClicked();
    void NumberButtonBClicked();

    void on_shuffleButton_clicked();

    void on_restoreButton_clicked();

    void on_SolveButton_clicked();

    void on_GenerateButton_clicked();

private:
    Ui::MainWindow *ui;
    QStack<int> *CondNumberA;
    QStack<int> *CondNumberB;
    solver *Solver;
};

#endif // MAINWINDOW_H
