#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
using namespace std;

#define len 100//максимальная длина введенного выражения (длина  С-строки)

void add_component(char**& expression, char*& expr3, int& counter)
{
    counter++;
    //создал временное хранилище
    char** buff = new char* [counter];
    for (int i = 0; i < counter; i++) {
        buff[i] = new char[len];
    }
    //скопировал из массива токенов токены во временное хранилище
    for (int i = 0; i < counter - 1; i++) {
        for (int j = 0; j <= strlen(expression[i]); j++) {
            buff[i][j] = expression[i][j];
        }
    }
    //скопировал в буфер добавляемый токен
    for (int i = 0; i <= strlen(expr3); i++) {
        buff[counter - 1][i] = expr3[i];
    }
    //отчистил память во входящем массиве
    if (counter > 1) {
        for (int i = 0; i < (counter - 1); i++) {
            delete[] expression[i];
        }
        delete[]expression;
    }
    expression = buff;
}

void change_component(char**& expression, int ii, int& counter, double dbuf)
{
    int n;
    n = counter - 2;//кол-во токенов которые останутся в массиве 
    //после операции на двух соседних возле ii элементов 
    char** buf = new char* [n];//создаю временное хранилище
    for (int i = 0; i < n; i++)
        buf[i] = new char[len];
    int i = 0;//указатель на buf;
    int ei = 0;//указатель на expression
    for (; i < ii - 1; i++, ei++)
        for (int j = 0; j <= strlen(expression[i]); j++) {
            buf[i][j] = expression[ei][j];
        }
    ei += 3;
    string sbuf = to_string(dbuf);
    for (int j = 0; j < sbuf.length(); j++) {//копирую в буфер результат посчитанного выражения
        buf[i][j] = sbuf[j];
    }
    buf[i][sbuf.length()] = '\0';
    i++;

    if (counter > 3) {//если было больше 3 токенов записываю в буфер оставшуюся непосчитанную часть токенов
        for (; ei < counter; ei++, i++) {
            for (int j = 0; j <= strlen(expression[ei]); j++) {
                buf[i][j] = expression[ei][j];
            }
        }
    }
    if (counter > 1) {
        for (int i = 0; i < counter; i++) {
            delete[] expression[i];
        }
        delete[]expression;
    }
    expression = buf;
    counter -= 2;
}

void calc1(char**& expression, int i, char ch, int& counter)
{
    double dbuf;
    switch (ch)
    {
    case '+': {
        dbuf = atof(expression[i - 1]) + atof(expression[i + 1]);
        change_component(expression, i, counter, dbuf);
        break;
    }
    case '*': {
        dbuf = atof(expression[i - 1]) * atof(expression[i + 1]);
        change_component(expression, i, counter, dbuf);
        break;
    }
    case '/': {
        if (atof(expression[i + 1]) == 0) {
            cout << "can't divide by zero" << endl;
            exit(1);
        }
        dbuf = atof(expression[i - 1]) / atof(expression[i + 1]);
        change_component(expression, i, counter, dbuf);
        break;
    }
    default:
        break;
    }
}

double calc(char*& expr)
{
    int fragment_length = 0;
    for (int i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == '(') {//записываю в expr2 фрагмент между скобок

            char* expr2 = new char[len];//строка между скобок
            int flag = 0;//0-скобок '(' не найдено
            if (expr[i + 1] != '\0') {
                for (int j = i + 1, k = 0; j < strlen(expr); j++, k++) {//j-указатель на фрагмент внутри скобок
                    //скобка пропускается для этого j=i+1
                    expr2[k] = expr[j];
                    if (expr[j] == '(') {
                        flag++;
                    }
                    if (expr[j] == ')') {
                        if (flag == 0) {
                            expr2[k] = '\0';
                            break;
                        }
                        else
                            flag--;
                    }

                    if (j + 1 == strlen(expr) && expr[j] != ')')
                        cout << "ошибка №1, не найдена вторая скобка )" << endl;
                }
            }
            else
                cout << "ошибка №2, не найдена вторая скобка )" << endl;
            //отправляю выделенный фрагмент между скобок считать в рекурсивно
            fragment_length = strlen(expr2);
            double rez;
            rez = calc(expr2);
            string sbuf = to_string(rez);//перевожу полученный результат из double  в string
            //заменяю фрагмент из строки между скобок на посчитанный результат фрагмента 
            char* buf = new char[strlen(expr) - fragment_length - 2 + 1 + sbuf.length()];
            buf[strlen(expr) - fragment_length - 2 + sbuf.length()] = '\0';
            bool flag1 = 0;
            for (int j = 0, jj = 0; expr[j] != '\0'; j++, jj++) {//нахожу скобку '(' начала фргмента
                if (expr[j] == '(' && flag1 == 0) {
                    flag1 = 1;//указывает на то что найдено начало первой пары скобок

                    for (int k = 0; k < sbuf.length(); k++) {//копирую в buf посчитанный участок expressio_на
                        buf[jj] = sbuf[k];
                        jj++;
                    }
                    int flag = 0;//копирую остаток участка expression
                    for (int k = j + 1; j < strlen(expr); k++) {
                        //пропускаю посчитанный участок
                        if (expr[k] == '(') {
                            flag++;
                        }
                        if (expr[k] == ')') {
                            if (flag == 0) {
                                j = k + 1;
                                //jj++;
                                break;

                            }
                            else
                                flag--;
                        }
                    }
                }
                buf[jj] = expr[j];
                if (expr[j] == '\0') j--;
            }
            delete[]expr;
            expr = buf;
            delete[]expr2;

        }
    }
    //разлаживаю выражение на токены
    char** expression = nullptr;
    char* expr3 = new char[len];
    bool opperator = 0;//флаг того в данные момент идет оператор или число
    int counter = 0;//счетчик компонентов в выражении
    if (expr[0] == '+' || expr[0] == '*' || expr[0] == '/') opperator = 1;//первый токен оператор
    bool first_expr = 1;
    int iterator = 0;//указатель какой по счету символ токена записывается
    for (int i = 0; i <= strlen(expr); i++) {
        if (opperator == 0) {//токен числовой
            if (expr[i] == '+' || expr[i] == '*' || expr[i] == '/') {//числовой токен закончился 
                opperator = 1;//показываю что дальше идет оператор
                expr3[iterator] = '\0';
                add_component(expression, expr3, counter);//добавляю числовой токен в массив
                iterator = 0;
                expr3[iterator] = expr[i];
                iterator++;
            }
            else {//записываю числовой токен в строку
                if (expr[i] == '-' && first_expr == 1) {//минусовое число с начала выражения например -2+4+5
                    expr3[iterator] = expr[i];
                    iterator++;
                }
                else if (expr[i] == '-' && first_expr == 0) {//минусовое число после числа например 3-2
                    //добавить токен + и токен минусового числа
                    expr3[iterator] = '\0';
                    add_component(expression, expr3, counter);//добавляю числовой токен в массив
                    iterator = 0;
                    expr3[iterator] = '+';
                    expr3[iterator + 1] = '\0';
                    add_component(expression, expr3, counter);//добавляю '+' токен в массив
                    iterator = 0;
                    expr3[iterator] = expr[i];
                    iterator++;
                }
                else {
                    expr3[iterator] = expr[i];
                    iterator++;
                }
            }
        }
        else {//токен оператор operator=1
            if (expr[i] == '+' || expr[i] == '*' || expr[i] == '/') {//операторный токен начался или продолжается 
                expr3[iterator] = expr[i];
                iterator++;

            }

            else {//оператор закончился
                opperator = 0;//дальше идет число
                expr3[iterator] = '\0';
                add_component(expression, expr3, counter);//добавляю оператор токен в массив
                iterator = 0;
                if (expr[i] == '-' && first_expr == 1) {//минусовое число после числа например 3-2
                    //добавить токен + и токен минусового числа
                    expr3[iterator] = '+';
                    add_component(expression, expr3, counter);//добавляю '+' токен в массив
                    iterator = 0;
                    expr3[iterator] = expr[i];
                    iterator++;
                }
                else {
                    expr3[iterator] = expr[i];
                    iterator++;
                }
            }
        }
        first_expr = 0;
        if (expr[i] == '\0')//если входящее выражение закончилось отправить последний токен в массив токенов 
            add_component(expression, expr3, counter);
    }
    delete[] expr3;
    //если выражение=отридцательное число
    if (counter == 1 && expression[0][0] == '-') {
        for (int i = 0; i < counter; i++) {
            delete[] expression[i];
        }
        delete[]expression;

        return atof(expr);

    }
    //провожу вычисление выражения
    while (counter > 1) {//провожу вычисления пока не останется один токен(являющийся ответом)
        //ищу есть ли знак операций и запускаю операцию вычисления
        for (int i = 0; i < counter; i++) {
            if (expression[i][0] == '*') {
                calc1(expression, i, '*', counter);
                if (counter > 1)
                    i = -1;
            }
        }
        for (int i = 0; i < counter; i++) {
            if (expression[i][0] == '/') {
                calc1(expression, i, '/', counter);
                if (counter > 1)
                    i = -1;
            }
        }
        for (int i = 0; i < counter; i++) {
            if (expression[i][0] == '+') {
                calc1(expression, i, '+', counter);
                if (counter > 1)
                    i = -1;
            }
        }
    }
    double x = atof(expression[0]);
    for (int i = 0; i < counter; i++) {
        delete[] expression[i];
    }
    delete[]expression;
    return x;
}


int main()
{
    setlocale(LC_ALL, "");
    char* expression;

    while (1) {
        expression = new char[len];
        cout << "введите выражение(не более " << len << " символов)\nНапример: -12+3*5*(23-6*(4-5/9))-34*59*(78-89/3*(10-8*(40/59-1)))" << endl;
        cin >> expression;
        cout << "=" << fixed << setprecision(5) << calc(expression) << endl;
        delete[]expression;
    }

}//-12+3*5*(23-6*(4-5/9))-34*59*(78-89/3*(10-8*(40/59-1)))
