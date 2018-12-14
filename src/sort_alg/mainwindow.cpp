#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QListWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CondNumberA = new QStack<int>;
    CondNumberA->push(0);
    CondNumberB = new QStack<int>;
    CondNumberB->push(0);

    Solver = new solver();

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
    delete Solver;
    delete CondNumberA;
    delete CondNumberB;
    delete ui;
}

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

void MainWindow::NumberButtonBClicked()  //Обработка нажатия кнопки состояния B
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
    int MaxNumber = inputBox->getInt(this,"","Введите максимальное количество паллет: ",0,0,18,1,0,0);

    //Обнуление позиций
    QPushButton *ButtonBuf1;
    QPushButton *ButtonBuf2;
    CondNumberA->clear();
    CondNumberA->push(0);
    CondNumberB->clear();
    CondNumberB->push(0);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 9; j++){
            CondNumberA->push(18 - i * 9 - j);
            CondNumberB->push(18 - i * 9 - j);
            ButtonBuf1 = (QPushButton *)(ui->gridLayout_A->itemAtPosition(i,j)->widget());
            ButtonBuf1->setText("");
            ButtonBuf2 = (QPushButton *)(ui->gridLayout_B->itemAtPosition(i,j)->widget());
            ButtonBuf2->setText("");
        }
    }

    ui->label_max_Numb->setText("Максимальный номер: " + QString::number(0));
    ui->label_max_Numb_2->setText("Максимальный номер: " + QString::number(0));

    //Случайное заполнение состояний
    for (int i = 0; i < MaxNumber; i++) {
        ButtonBuf1 = (QPushButton *)(ui->gridLayout_A->itemAtPosition((i / 9),(i % 9))->widget());
        ButtonBuf1->clicked();
        ButtonBuf2 = (QPushButton *)(ui->gridLayout_B->itemAtPosition((i / 9),(i % 9))->widget());
        ButtonBuf2->clicked();
    }  //Заполнение первых MaxNumber мест

    int j = 0;
    for (int i = 0; i < MaxNumber; i++) {
        //Состояние А
        j = qrand() % 18;
        ButtonBuf1 = (QPushButton *)(ui->gridLayout_A->itemAtPosition((i / 9),(i % 9))->widget());
        ButtonBuf2 = (QPushButton *)(ui->gridLayout_A->itemAtPosition((j / 9),(j % 9))->widget());
        if (ButtonBuf2->text().isEmpty()) {
            ButtonBuf2->setText("U " + ButtonBuf1->text().mid(2));
            ButtonBuf1->setText("");
        } else {
            int Number2 = ButtonBuf2->text().mid(2).toInt();
            ButtonBuf2->setText("U " + ButtonBuf1->text().mid(2));
            ButtonBuf1->setText("U " + QString::number(Number2));
        }
        //Состояние B
        j = qrand() % 18;
        ButtonBuf1 = (QPushButton *)(ui->gridLayout_B->itemAtPosition((i / 9),(i % 9))->widget());
        ButtonBuf2 = (QPushButton *)(ui->gridLayout_B->itemAtPosition((j / 9),(j % 9))->widget());
        if (ButtonBuf2->text().isEmpty()) {
            ButtonBuf2->setText("U " + ButtonBuf1->text().mid(2));
            ButtonBuf1->setText("");
        } else {
            int Number2 = ButtonBuf2->text().mid(2).toInt();
            ButtonBuf2->setText("U " + ButtonBuf1->text().mid(2));
            ButtonBuf1->setText("U " + QString::number(Number2));
        }
    }  //Перемена мест первых MaxNumber на случайные
}

void MainWindow::on_restoreButton_clicked()
{
    //Обнуление позиций
    QPushButton *ButtonBuf1;
    QPushButton *ButtonBuf2;
    CondNumberA->clear();
    CondNumberA->push(0);
    CondNumberB->clear();
    CondNumberB->push(0);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 9; j++){
            CondNumberA->push(18 - i * 9 - j);
            CondNumberB->push(18 - i * 9 - j);
            ButtonBuf1 = (QPushButton *)(ui->gridLayout_A->itemAtPosition(i,j)->widget());
            ButtonBuf1->setText("");
            ButtonBuf2 = (QPushButton *)(ui->gridLayout_B->itemAtPosition(i,j)->widget());
            ButtonBuf2->setText("");
        }
    }

    ui->label_max_Numb->setText("Максимальный номер: " + QString::number(0));
    ui->label_max_Numb_2->setText("Максимальный номер: " + QString::number(0));
}

void MainWindow::on_SolveButton_clicked()
{
    QPushButton *ButtonBuf;

    TState StateA = (char* ) (calloc(26, sizeof(char)));
    TState StateB = (char* ) (calloc(26, sizeof(char)));

    for (int i = 0; i < 18; i++) {                     //Запись сотояния А
        ButtonBuf = (QPushButton *)(ui->gridLayout_A->itemAtPosition((i / 9),(i % 9))->widget());
        if (ButtonBuf->text().isEmpty()) {
            StateA[i] = 0;
        } else {StateA[i] = ButtonBuf->text().mid(2).toInt(); }
    }

    for (int i = 0; i < 18; i++) {                     //Запись состояния B
        ButtonBuf = (QPushButton *)(ui->gridLayout_B->itemAtPosition((i / 9),(i % 9))->widget());
        if (ButtonBuf->text().isEmpty()) {
            StateB[i] = 0;
        } else {StateB[i] = ButtonBuf->text().mid(2).toInt(); }
    }

    QMessageBox::StandardButton reply;
    ui->listWidget->clear();
    ui->listWidget->addItem("Идет расчет...");
    switch (Solver->Analyze(StateA,StateB)) {  //Анализ перед расчетом
    case 2:
        QMessageBox::critical(this,tr("Ошибка размеров"),tr("У состояния А и состояния B не совпадает количество элементов"));
        ui->listWidget->clear();
        ui->listWidget->addItem("Ошибка: у состояния А и состояния B не совпадает количество элементов");
        break;
    case 1:
        reply = QMessageBox::question(this,"Информация","Количество шагов для достижения состояния B будет больше 29 и возможно расчет не будет проведен. Желаете продолжить?");
        if (reply == QMessageBox::Yes) {
            int Count = Solver->Solve(StateA,StateB);
        }
        if (Solver->Answer->isEmpty()) {
            ui->listWidget->clear();
            ui->listWidget->addItem("Решение не найдено");
        }
        break;
    case 0:
        int Count = Solver->Solve(StateA,StateB);
        if (Solver->Answer->isEmpty()) {
            ui->listWidget->clear();
            ui->listWidget->addItem("Решение не найдено");
        }
        break;
    }

    if (!Solver->Answer->isEmpty()) {
        ui->listWidget->clear();
        ui->listWidget->addItem("Ходы для решения:");
        int step = 1;
        while (!Solver->Answer->isEmpty()) {
            AnswerNode AnswerNodeBuf = Solver->Answer->pop();
            ui->listWidget->addItem(QString::number(step) +" ход: Из [" + QString::number((AnswerNodeBuf.Index1 / 9) + 1) + ", " +
                                    QString::number((AnswerNodeBuf.Index1 % 9) + 1) + "] в [" +
                                    QString::number((AnswerNodeBuf.Index2 / 9) + 1) + ", " +
                                    QString::number((AnswerNodeBuf.Index2 % 9) + 1) + "]");
            step++;
        }
    }

    free(StateB);
    free(StateA);

    bool cool = true;

}

void MainWindow::on_GenerateButton_clicked()
{
    ui->restoreButton->click();

    QPushButton *ButtonBuf1;
    QPushButton *ButtonBuf2;

    //Заполнение первых 4 состояний
    for (int i = 0; i < 4; i++) {
        ButtonBuf1 = (QPushButton *)(ui->gridLayout_A->itemAtPosition((i / 9),(i % 9))->widget());
        ButtonBuf1->clicked();
        ButtonBuf2 = (QPushButton *)(ui->gridLayout_B->itemAtPosition((i / 9),(i % 9))->widget());
        ButtonBuf2->clicked();
    }  //Заполнение первых MaxNumber мест

    //Заполнение последних 13 состояний
    for (int i = 5; i < 18; i++) {
        ButtonBuf1 = (QPushButton *)(ui->gridLayout_A->itemAtPosition((i / 9),(i % 9))->widget());
        ButtonBuf1->clicked();
        ButtonBuf2 = (QPushButton *)(ui->gridLayout_B->itemAtPosition((i / 9),(i % 9))->widget());
        ButtonBuf2->clicked();
    }  //Заполнение первых MaxNumber мест

}
