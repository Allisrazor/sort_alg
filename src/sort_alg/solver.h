#ifndef SOLVER_H
#define SOLVER_H

#include <queue>
#include <vector>
#include <QVector>
#include <QStack>
#include <QQueue>
#include <QHash>
#include <QString>
#include <math.h>

using namespace std;

typedef int* TStateCode;
//typedef int* TState;  если надо будет кодировать
typedef char* TState;

struct TNode {
    int priority;
    TState value;
};

struct CompareNode {
    bool operator()(TNode& n1, TNode& n2)
    {
        if (n1.priority >= n2.priority) return true;
        return false;
    }
};

struct AnswerNode {
    int Index1;
    int Index2;
};

union converter
{
    int number;
    char bytes[4];
};

class solver
{
public:
    solver();
    ~solver();

    QStack<AnswerNode> *Answer;                 //Запись движений пятнашек

    int Solve(TState StateA, TState StateB);     //Основная функция расчета
    int Analyze(TState StateA, TState StateB);   //Проверка верности исходных данных и расчета

private:
    priority_queue<TNode, vector<TNode>, CompareNode> *StateQueue; //Очередь состояний для проверки
    int IterNum;                                                   //Количество вершин графа
    QVector<int>  *PIter;                                          //Вектор вершин графа
    QHash<QString, int> PIterHash;                                 //Хэш таблица вершин графа
    int NullNumber;                                                //Количество нулей состояния

    int powInt(int a, int b);                                      //Целочисленное возведение в степень

    TStateCode code(TState CurState, TStateCode PrevStateCode);    //??? Закодировать состояние
    TState decode(TStateCode CurStateCode);                        //??? Раскодировать состояние

    bool IsSame(TState CurState, TState ExpectState);             //Проверка на достижение состояния B
    bool IsHead(TState State);                                    //Проверка: первое ли состояние
    int  GetNul(TState State, int NullPos);                       //Поиск нуля в массиве
    int  GetNulNumber(TState State);                              //Поиск нуля в массиве
    void OpenState(TState State, TState LastState, int index);    //Добавление новых состояний
    int  GetGreed(TState CurState, TState LastState);             //Найти жадность
    void GetResult(TState LastState);                             //Записать ходы в стэк
};

#endif // SOLVER_H
