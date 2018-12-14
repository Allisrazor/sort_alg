#include "solver.h"

solver::solver()
{
    StateQueue = new priority_queue<TNode, vector<TNode>, CompareNode>;
    IterNum = 0;
    PIter = new QVector<int>;
    Answer = new QStack<AnswerNode>;
}

solver::~solver()
{
    delete StateQueue;
    delete PIter;
    delete Answer;
}

int solver::Solve(TState StateA, TState StateB)
{
    //Инициализация расчета
    NullNumber = GetNulNumber(StateA);
    TNode NodeBuf;
    NodeBuf.priority = GetGreed(StateA,StateB);
    NodeBuf.value = StateA;
    StateQueue->push(NodeBuf);
    TState StateBuf = StateA;
    PIter->append((int) StateBuf);
    QString StrState = "";
    for (int i = 0; i < 18; i++) {
        StrState.append(StateBuf[i]);
    }
    PIterHash.insert(StrState, IterNum);

    //Расчет
    while (not IsSame(StateBuf, StateB)) {
        for (int i = 0; i < NullNumber; i++) {
            int index = GetNul(StateBuf, i);
            OpenState(StateBuf, StateB, index);
        }
        StateBuf = StateQueue->top().value;
        StateQueue->pop();
        if (IterNum > 7000000) {break;}
    }

    //Вывод данных
    if (IsSame(StateBuf,StateB)) {GetResult(StateBuf);}
    int res = IterNum;

    //Очистка памяти
    while (! StateQueue->empty()) {
        StateQueue->pop();
    }
    PIterHash.clear();
    TState DelPIterBuf = 0;
    for (int i = IterNum + 1; i <= 0; i--) {
        DelPIterBuf = (TState) PIter->data()[i - 1];
        free(DelPIterBuf);
    }
    PIter->clear();
    IterNum = 0;

    //Вывод данных
    return res;
}

int solver::Analyze(TState StateA, TState StateB)
{
    //Проверка одинаковой размерности состояний
    int DimStateA = 0;
    int DimStateB = 0;
    for (int i = 0; i < 18; i++) {
        if (StateA[i] > 0) {DimStateA++;}
        if (StateB[i] > 0) {DimStateB++;}
    }
    if (DimStateA != DimStateB) {return 2;}  //2 - выход соответствует неверности размерности

    //Проверка количества возможных шагов для достижения расчета
    if (GetGreed(StateA,StateB) > 29) {return 1;} //1 - выход соответствует сложности расчета

    return 0;  //0 - выход соответствует нормальному расчету
}

int solver::powInt(int a, int b)
{
    int res = 1;
    if (b > 0) {
        for (int i = 0; i < b; i++) {
            res *= a;
        }
    }
    return res;
}

TStateCode solver::code(TState CurState, TStateCode PrevStateCode)  //??? если нужно будет кодировать
{
//    TStateCode CurStateCode = (int* ) (calloc(4,sizeof (int)));
//    int a = 0;
//    for (int i = 0; i < 3; i++) {
//        a = 0;
//        for (int j = 0; j < 3; j++){
//            a += CurState[i*3 + j] * powInt(18,(j));
//            a += CurState[9 + i*3 + j] * powInt(18,(j + 3));
//        }
//        CurStateCode[i] = a;
//    }
//    CurStateCode[3] = (int)(PrevStateCode);
//    return CurStateCode;
}

TState solver::decode(TStateCode CurStateCode)              //??? если нужно будет кодировать
{
//    TState CurState = (int* ) (calloc(18,sizeof (int)));;
//    for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 3; j++){
//            int a = (CurStateCode[i] / powInt(18,j)) % 18;
//            a = (CurStateCode[i] / powInt(18,(j + 3))) % 18;
//            CurState[i*3 + j] = (CurStateCode[i] / powInt(18,j)) % 18;
//            CurState[9 + i*3 + j] = (CurStateCode[i] / powInt(18,(j + 3))) % 18;
//        }
//    }
//    return CurState;
}

bool solver::IsSame(TState CurState, TState ExpectState)
{
    for (int i = 0; i < 18; i++) {
        if (CurState[i] != ExpectState[i]) {return false;}
    }
    return true;
}

bool solver::IsHead(TState State)
{
    converter c;
    c.bytes[0] = State[18];
    c.bytes[1] = State[19];
    c.bytes[2] = State[20];
    c.bytes[3] = State[21];
    if (c.number == 0) {return true;}
    else {return false;}
}

int solver::GetNul(TState State, int NullPos)
{
    int PosBuf = 0;
    for (int i = 0; i < 18; i++) {
        if (State[i] == 0) {
            PosBuf++;
            if (PosBuf == (NullPos + 1)) {return i;}
        }
    }
}

int solver::GetNulNumber(TState State)
{
    int res = 0;
    for (int i = 0; i < 18; i++) {
        if (State[i] == 0) {res++;}
    }
    return res;
}

void solver::OpenState(TState State, TState LastState, int index)
{
    converter c;
    converter NodeNum;

    c.number = ((int) State);       //Запись предыдущей вершины графа
    NodeNum.bytes[0] = State[22];   //Запись ряда вершин графа
    NodeNum.bytes[1] = State[23];
    NodeNum.bytes[2] = State[24];
    NodeNum.bytes[3] = State[25];
    NodeNum.number++;

    QString StrState1 = "";
    QString StrState2 = "";
    QString StrState3 = "";
    TState NewState1 = (char* ) (calloc(26, sizeof(char)));
    TState NewState2 = (char* ) (calloc(26, sizeof(char)));
    TState NewState3 = (char* ) (calloc(26, sizeof(char)));
    switch (index) {
    case 0:                     //Верхний левый угол
        for (int i = 0; i < 18; i++) {
            NewState1[i] = State[i];
            NewState2[i] = State[i];
        }
        NewState1[0] = State[1]; NewState2[0] = State[9];
        NewState1[1] = State[0]; NewState2[9] = State[0];

        //Проверка на то, была ли вершина и если нет, то запись в очередь
        StrState1 = "";
        StrState2 = "";
        for (int i = 0; i < 18; i++) {
            StrState1.append(NewState1[i]);
            StrState2.append(NewState2[i]);
        }

        if (not PIterHash.contains(StrState1)){
            NewState1[18] = c.bytes[0];
            NewState1[19] = c.bytes[1];
            NewState1[20] = c.bytes[2];
            NewState1[21] = c.bytes[3];
            NewState1[22] = NodeNum.bytes[0];
            NewState1[23] = NodeNum.bytes[1];
            NewState1[24] = NodeNum.bytes[2];
            NewState1[25] = NodeNum.bytes[3];
            TNode NodeBuf;
            NodeBuf.priority = GetGreed(NewState1,LastState);
            NodeBuf.value = NewState1;
            StateQueue->push(NodeBuf);
            IterNum++; PIter->append((int) NewState1);
            PIterHash.insert(StrState1, IterNum);
        } else {free(NewState1);}

        if (not PIterHash.contains(StrState2)){
            NewState2[18] = c.bytes[0];
            NewState2[19] = c.bytes[1];
            NewState2[20] = c.bytes[2];
            NewState2[21] = c.bytes[3];
            NewState2[22] = NodeNum.bytes[0];
            NewState2[23] = NodeNum.bytes[1];
            NewState2[24] = NodeNum.bytes[2];
            NewState2[25] = NodeNum.bytes[3];
            TNode NodeBuf;
            NodeBuf.priority = GetGreed(NewState2,LastState);
            NodeBuf.value = NewState2;
            StateQueue->push(NodeBuf);
            IterNum++; PIter->append((int) NewState2);
            PIterHash.insert(StrState2, IterNum);
        } else {free(NewState2);}
        free(NewState3);
        break;
    case 9:                     //Нижний левый угол
        for (int i = 0; i < 18; i++) {
            NewState1[i] = State[i];
            NewState2[i] = State[i];
        }
        NewState1[9] = State[10]; NewState2[0] = State[9];
        NewState1[10] = State[9]; NewState2[9] = State[0];

        //Проверка на то, была ли вершина и если нет, то запись в очередь
        StrState1 = "";
        StrState2 = "";
        for (int i = 0; i < 18; i++) {
            StrState1.append(NewState1[i]);
            StrState2.append(NewState2[i]);
        }

        if (not PIterHash.contains(StrState1)){
            NewState1[18] = c.bytes[0];
            NewState1[19] = c.bytes[1];
            NewState1[20] = c.bytes[2];
            NewState1[21] = c.bytes[3];
            NewState1[22] = NodeNum.bytes[0];
            NewState1[23] = NodeNum.bytes[1];
            NewState1[24] = NodeNum.bytes[2];
            NewState1[25] = NodeNum.bytes[3];
            TNode NodeBuf;
            NodeBuf.priority = GetGreed(NewState1,LastState);
            NodeBuf.value = NewState1;
            StateQueue->push(NodeBuf);
            IterNum++; PIter->append((int) NewState1);
            PIterHash.insert(StrState1, IterNum);
        } else {free(NewState1);}

        if (not PIterHash.contains(StrState2)){
            NewState2[18] = c.bytes[0];
            NewState2[19] = c.bytes[1];
            NewState2[20] = c.bytes[2];
            NewState2[21] = c.bytes[3];
            NewState2[22] = NodeNum.bytes[0];
            NewState2[23] = NodeNum.bytes[1];
            NewState2[24] = NodeNum.bytes[2];
            NewState2[25] = NodeNum.bytes[3];
            TNode NodeBuf;
            NodeBuf.priority = GetGreed(NewState2,LastState);
            NodeBuf.value = NewState2;
            StateQueue->push(NodeBuf);
            IterNum++; PIter->append((int) NewState2);
            PIterHash.insert(StrState2, IterNum);
        } else {free(NewState2);}
        free(NewState3);
        break;
    case 8:                     //Верхний правый угол
        for (int i = 0; i < 18; i++) {
            NewState1[i] = State[i];
            NewState2[i] = State[i];
        }
        NewState1[8] = State[7]; NewState2[8] = State[17];
        NewState1[7] = State[8]; NewState2[17] = State[8];

        //Проверка на то, была ли вершина и если нет, то запись в очередь
        StrState1 = "";
        StrState2 = "";
        for (int i = 0; i < 18; i++) {
            StrState1.append(NewState1[i]);
            StrState2.append(NewState2[i]);
        }

        if (not PIterHash.contains(StrState1)){
            NewState1[18] = c.bytes[0];
            NewState1[19] = c.bytes[1];
            NewState1[20] = c.bytes[2];
            NewState1[21] = c.bytes[3];
            NewState1[22] = NodeNum.bytes[0];
            NewState1[23] = NodeNum.bytes[1];
            NewState1[24] = NodeNum.bytes[2];
            NewState1[25] = NodeNum.bytes[3];
            TNode NodeBuf;
            NodeBuf.priority = GetGreed(NewState1,LastState);
            NodeBuf.value = NewState1;
            StateQueue->push(NodeBuf);
            IterNum++; PIter->append((int) NewState1);
            PIterHash.insert(StrState1, IterNum);
        } else {free(NewState1);}

        if (not PIterHash.contains(StrState2)){
            NewState2[18] = c.bytes[0];
            NewState2[19] = c.bytes[1];
            NewState2[20] = c.bytes[2];
            NewState2[21] = c.bytes[3];
            NewState2[22] = NodeNum.bytes[0];
            NewState2[23] = NodeNum.bytes[1];
            NewState2[24] = NodeNum.bytes[2];
            NewState2[25] = NodeNum.bytes[3];
            TNode NodeBuf;
            NodeBuf.priority = GetGreed(NewState2,LastState);
            NodeBuf.value = NewState2;
            StateQueue->push(NodeBuf);
            IterNum++; PIter->append((int) NewState2);
            PIterHash.insert(StrState2, IterNum);
        } else {free(NewState2);}
        free(NewState3);
        break;
    case 17:                     //Нижний правый угол
        for (int i = 0; i < 18; i++) {
            NewState1[i] = State[i];
            NewState2[i] = State[i];
        }
        NewState1[17] = State[16]; NewState2[8] = State[17];
        NewState1[16] = State[17]; NewState2[17] = State[8];

        //Проверка на то, была ли вершина и если нет, то запись в очередь
        StrState1 = "";
        StrState2 = "";
        for (int i = 0; i < 18; i++) {
            StrState1.append(NewState1[i]);
            StrState2.append(NewState2[i]);
        }

        if (not PIterHash.contains(StrState1)){
            NewState1[18] = c.bytes[0];
            NewState1[19] = c.bytes[1];
            NewState1[20] = c.bytes[2];
            NewState1[21] = c.bytes[3];
            NewState1[22] = NodeNum.bytes[0];
            NewState1[23] = NodeNum.bytes[1];
            NewState1[24] = NodeNum.bytes[2];
            NewState1[25] = NodeNum.bytes[3];
            TNode NodeBuf;
            NodeBuf.priority = GetGreed(NewState1,LastState);
            NodeBuf.value = NewState1;
            StateQueue->push(NodeBuf);
            IterNum++; PIter->append((int) NewState1);
            PIterHash.insert(StrState1, IterNum);
        } else {free(NewState1);}

        if (not PIterHash.contains(StrState2)){
            NewState2[18] = c.bytes[0];
            NewState2[19] = c.bytes[1];
            NewState2[20] = c.bytes[2];
            NewState2[21] = c.bytes[3];
            NewState2[22] = NodeNum.bytes[0];
            NewState2[23] = NodeNum.bytes[1];
            NewState2[24] = NodeNum.bytes[2];
            NewState2[25] = NodeNum.bytes[3];
            TNode NodeBuf;
            NodeBuf.priority = GetGreed(NewState2,LastState);
            NodeBuf.value = NewState2;
            StateQueue->push(NodeBuf);
            IterNum++; PIter->append((int) NewState2);
            PIterHash.insert(StrState2, IterNum);
        } else {free(NewState2);}
        free(NewState3);
        break;
    case 1:               //Верхний ряд
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        for (int i = 0; i < 18; i++) {
            NewState1[i] = State[i];
            NewState2[i] = State[i];
            NewState3[i] = State[i];
        }
        NewState1[index] = State[index + 1];
        NewState1[index + 1] = State[index];
        NewState2[index] = State[index - 1];
        NewState2[index - 1] = State[index];
        NewState3[index] = State[index + 9];
        NewState3[index + 9] = State[index];

        //Проверка на то, была ли вершина и если нет, то запись в очередь
        StrState1 = "";
        StrState2 = "";
        StrState3 = "";
        for (int i = 0; i < 18; i++) {
            StrState1.append(NewState1[i]);
            StrState2.append(NewState2[i]);
            StrState3.append(NewState3[i]);
        }

        if (not PIterHash.contains(StrState1)){
            NewState1[18] = c.bytes[0];
            NewState1[19] = c.bytes[1];
            NewState1[20] = c.bytes[2];
            NewState1[21] = c.bytes[3];
            NewState1[22] = NodeNum.bytes[0];
            NewState1[23] = NodeNum.bytes[1];
            NewState1[24] = NodeNum.bytes[2];
            NewState1[25] = NodeNum.bytes[3];
            TNode NodeBuf;
            NodeBuf.priority = GetGreed(NewState1,LastState);
            NodeBuf.value = NewState1;
            StateQueue->push(NodeBuf);
            IterNum++; PIter->append((int) NewState1);
            PIterHash.insert(StrState1, IterNum);
        } else {free(NewState1);}

        if (not PIterHash.contains(StrState2)){
            NewState2[18] = c.bytes[0];
            NewState2[19] = c.bytes[1];
            NewState2[20] = c.bytes[2];
            NewState2[21] = c.bytes[3];
            NewState2[22] = NodeNum.bytes[0];
            NewState2[23] = NodeNum.bytes[1];
            NewState2[24] = NodeNum.bytes[2];
            NewState2[25] = NodeNum.bytes[3];
            TNode NodeBuf;
            NodeBuf.priority = GetGreed(NewState2,LastState);
            NodeBuf.value = NewState2;
            StateQueue->push(NodeBuf);
            IterNum++; PIter->append((int) NewState2);
            PIterHash.insert(StrState2, IterNum);
        } else {free(NewState2);}

        if (not PIterHash.contains(StrState3)){
            NewState3[18] = c.bytes[0];
            NewState3[19] = c.bytes[1];
            NewState3[20] = c.bytes[2];
            NewState3[21] = c.bytes[3];
            NewState3[22] = NodeNum.bytes[0];
            NewState3[23] = NodeNum.bytes[1];
            NewState3[24] = NodeNum.bytes[2];
            NewState3[25] = NodeNum.bytes[3];
            TNode NodeBuf;
            NodeBuf.priority = GetGreed(NewState3,LastState);
            NodeBuf.value = NewState3;
            StateQueue->push(NodeBuf);
            IterNum++; PIter->append((int) NewState3);
            PIterHash.insert(StrState3, IterNum);
        } else {free(NewState3);}
        break;
    default:              //Нижний ряд
        for (int i = 0; i < 18; i++) {
            NewState1[i] = State[i];
            NewState2[i] = State[i];
            NewState3[i] = State[i];
        }
        NewState1[index] = State[index + 1];
        NewState1[index + 1] = State[index];
        NewState2[index] = State[index - 1];
        NewState2[index - 1] = State[index];
        NewState3[index] = State[index - 9];
        NewState3[index - 9] = State[index];

        //Проверка на то, была ли вершина и если нет, то запись в очередь
        StrState1 = "";
        StrState2 = "";
        StrState3 = "";
        for (int i = 0; i < 18; i++) {
            StrState1.append(NewState1[i]);
            StrState2.append(NewState2[i]);
            StrState3.append(NewState3[i]);
        }

        if (not PIterHash.contains(StrState1)){
            NewState1[18] = c.bytes[0];
            NewState1[19] = c.bytes[1];
            NewState1[20] = c.bytes[2];
            NewState1[21] = c.bytes[3];
            NewState1[22] = NodeNum.bytes[0];
            NewState1[23] = NodeNum.bytes[1];
            NewState1[24] = NodeNum.bytes[2];
            NewState1[25] = NodeNum.bytes[3];
            TNode NodeBuf;
            NodeBuf.priority = GetGreed(NewState1,LastState);
            NodeBuf.value = NewState1;
            StateQueue->push(NodeBuf);
            IterNum++; PIter->append((int) NewState1);
            PIterHash.insert(StrState1, IterNum);
        } else {free(NewState1);}

        if (not PIterHash.contains(StrState2)){
            NewState2[18] = c.bytes[0];
            NewState2[19] = c.bytes[1];
            NewState2[20] = c.bytes[2];
            NewState2[21] = c.bytes[3];
            NewState2[22] = NodeNum.bytes[0];
            NewState2[23] = NodeNum.bytes[1];
            NewState2[24] = NodeNum.bytes[2];
            NewState2[25] = NodeNum.bytes[3];
            TNode NodeBuf;
            NodeBuf.priority = GetGreed(NewState2,LastState);
            NodeBuf.value = NewState2;
            StateQueue->push(NodeBuf);
            IterNum++; PIter->append((int) NewState2);
            PIterHash.insert(StrState2, IterNum);
        } else {free(NewState2);}

        if (not PIterHash.contains(StrState3)){
            NewState3[18] = c.bytes[0];
            NewState3[19] = c.bytes[1];
            NewState3[20] = c.bytes[2];
            NewState3[21] = c.bytes[3];
            NewState3[22] = NodeNum.bytes[0];
            NewState3[23] = NodeNum.bytes[1];
            NewState3[24] = NodeNum.bytes[2];
            NewState3[25] = NodeNum.bytes[3];
            TNode NodeBuf;
            NodeBuf.priority = GetGreed(NewState3,LastState);
            NodeBuf.value = NewState3;
            StateQueue->push(NodeBuf);
            IterNum++; PIter->append((int) NewState3);
            PIterHash.insert(StrState3, IterNum);
        } else {free(NewState3);}
        break;
    }
}

int solver::GetGreed(TState CurState, TState LastState)
{
    converter NodeNum;
    NodeNum.bytes[0] = CurState[22];   //Запись ряда вершин графа
    NodeNum.bytes[1] = CurState[23];
    NodeNum.bytes[2] = CurState[24];
    NodeNum.bytes[3] = CurState[25];
    int res = NodeNum.number;

    int CurBuf[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int LastBuf[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    for (int i = 0; i < 18; i++) {
        CurBuf[CurState[i]] = i;
        LastBuf[LastState[i]] = i;
    }
    for (int i = 1; i < 18; i++) {
        res += abs((CurBuf[i] % 9) - (LastBuf[i] % 9));
        res += abs((CurBuf[i] / 9) - (LastBuf[i] / 9));
    }
    return res;
}

void solver::GetResult(TState LastState)
{
    converter NextPointer;
    TState PrevState;
    AnswerNode NodeBuf;

    while (!IsHead(LastState)) {
           NextPointer.bytes[0] = LastState[18];
           NextPointer.bytes[1] = LastState[19];
           NextPointer.bytes[2] = LastState[20];
           NextPointer.bytes[3] = LastState[21];

           PrevState = (TState) NextPointer.number;

           for (int i = 0; i < 18; i++) {
               if (PrevState[i] != LastState[i]) {
                   if (PrevState[i] == 0) {NodeBuf.Index2 = i;}
                   else {NodeBuf.Index1 = i;}
               }
           }
           Answer->push(NodeBuf);
           LastState = PrevState;
    }
}
