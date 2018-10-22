#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CondNumberA = new QStack<int>;
    CondNumberA->push(0);
    CondNumberB = new QStack<int>;
    CondNumberB->push(0);

    QPushButton* ButtonBufA;           //Связка кнопок состояния А с обработкой нажатия
    QPushButton* ButtonBufB;           //Связка кнопок состояния B с обработкой нажатия
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 9; j++) {
            ButtonBufA = (QPushButton *)(ui->gridLayout_A->itemAtPosition(i,j)->widget());
            connect(ButtonBufA, SIGNAL(clicked()),this, SLOT(NumberButtonAClicked()));
            CondNumberA->push(18-(i*9 + j));
            ButtonBufB = (QPushButton *)(ui->gridLayout_B->itemAtPosition(i,j)->widget());
            connect(ButtonBufB, SIGNAL(clicked()),this, SLOT(NumberButtonBClicked()));
            CondNumberB->push(18-(i*9 + j));
        }
    }
}

MainWindow::~MainWindow()
{
    delete CondNumberA;
    delete CondNumberB;
    delete ui;
}
<<<<<<< HEAD
=======

void MainWindow::NumberButtonAClicked()  //Обработка нажатия кнопки состояния А
{
    QPushButton *button = (QPushButton *)sender();
    int CurrentNumber;

    if (button->text().isEmpty()) {
        CurrentNumber = CondNumberA->pop();
        if (CurrentNumber >= ui->label_max_Numb->text().mid(20).toInt()) {
            ui->label_max_Numb->setText("Максимальный номер: " + QString::number(CurrentNumber));
        }
        button->setText("U " + QString::number(CurrentNumber));
    } else {
        CurrentNumber = button->text().mid(2).toInt();
        if (CurrentNumber == ui->label_max_Numb->text().mid(20).toInt()) {
            ui->label_max_Numb->setText("Максимальный номер: " + QString::number(CurrentNumber - 1));
        }
        CondNumberA->push(CurrentNumber);
        button->setText("");
    }
}

void MainWindow::NumberButtonBClicked()  //Обработка нажатия кнопки состояния А
{
    QPushButton *button = (QPushButton *)sender();
    int CurrentNumber;

    if (button->text().isEmpty()) {
        CurrentNumber = CondNumberB->pop();
        if (CurrentNumber >= ui->label_max_Numb_2->text().mid(20).toInt()) {
            ui->label_max_Numb_2->setText("Максимальный номер: " + QString::number(CurrentNumber));
        }
        button->setText("U " + QString::number(CurrentNumber));
    } else {
        CurrentNumber = button->text().mid(2).toInt();
        if (CurrentNumber == ui->label_max_Numb_2->text().mid(20).toInt()) {
            ui->label_max_Numb_2->setText("Максимальный номер: " + QString::number(CurrentNumber - 1));
        }
        CondNumberB->push(CurrentNumber);
        button->setText("");
    }
}



void MainWindow::on_shuffleButton_clicked() //Генерация случайных состояний
{
    //Диалоговое окно для ввода максимального числа паллет
    QInputDialog *inputBox;
    inputBox = new QInputDialog;
    int MaxNumber = inputBox->getInt(this,"","Введите максимальное количество случайных шагов: ",0,0,100,1,0,0);

    //Обнуление позиций
    QPushButton *ButtonBuf1;
    QPushButton *ButtonBuf2;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 9; j++){
            ButtonBuf1 = (QPushButton *)(ui->gridLayout_A->itemAtPosition(i,j)->widget());
            ButtonBuf1->setText("");
            ButtonBuf2 = (QPushButton *)(ui->gridLayout_B->itemAtPosition(i,j)->widget());
            ButtonBuf2->setText("");
        }
    }

    ui->label_max_Numb->setText("Максимальный номер: " + QString::number(0));
    ui->label_max_Numb_2->setText("Максимальный номер: " + QString::number(0));

    //Случайные шаги заполнения
    for (int i = 0; i < MaxNumber; i++) {
        int j = qrand() % 18;
        ButtonBuf1 = (QPushButton *)(ui->gridLayout_A->itemAtPosition((j / 9),(j % 9))->widget());
        ButtonBuf1->clicked();
        j = qrand() % 18;
        ButtonBuf2 = (QPushButton *)(ui->gridLayout_B->itemAtPosition((j / 9),(j % 9))->widget());
        ButtonBuf2->clicked();
    }
}

void MainWindow::on_restoreButton_clicked()
{
    //Обнуление позиций
    QPushButton *ButtonBuf1;
    QPushButton *ButtonBuf2;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 9; j++){
            ButtonBuf1 = (QPushButton *)(ui->gridLayout_A->itemAtPosition(i,j)->widget());
            ButtonBuf1->setText("");
            ButtonBuf2 = (QPushButton *)(ui->gridLayout_B->itemAtPosition(i,j)->widget());
            ButtonBuf2->setText("");
        }
    }

    ui->label_max_Numb->setText("Максимальный номер: " + QString::number(0));
    ui->label_max_Numb_2->setText("Максимальный номер: " + QString::number(0));
}
>>>>>>> 9df8b43ba7743d001e468c457a19ea022d43e57d
