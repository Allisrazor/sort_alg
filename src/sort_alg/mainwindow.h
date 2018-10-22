#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

<<<<<<< HEAD
=======
private slots:
    void NumberButtonAClicked();
    void NumberButtonBClicked();

    void on_shuffleButton_clicked();

    void on_restoreButton_clicked();

>>>>>>> 9df8b43ba7743d001e468c457a19ea022d43e57d
private:
    Ui::MainWindow *ui;
    QStack<int> *CondNumberA;
    QStack<int> *CondNumberB;
};

#endif // MAINWINDOW_H
