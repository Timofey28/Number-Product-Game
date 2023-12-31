#include <iostream>
#include <array>
#include <queue>
#include <deque>
#include <conio.h>
#include <windows.h>
#include "records_observer.cc"
#include "help.cc"
#include "utils.h"
using namespace std;
mt19937 mersenne(static_cast<unsigned int>(time(0)));

enum eShowState {CALCULATION, SINGLE, BEST5, BEST20, BEST50, BEST100, LAST} showState;
int nConsoleWidth = 78,
    nConsoleHeight = 12;
bool toPause = 0;
bool directlyToHelp = 0,
     directlyTo1 = 0,
     directlyTo5 = 0,
     directlyTo2 = 0,
     directlyTo4 = 0,
     directlyTo0 = 0,
     directlyToL = 0,
     directlyToSmth = 0;

void numsProductGame(RecordsObserver* recordsObserver, Help* help);
int reversedInput(RecordsObserver* recordsObserver, clock_t& tStart, double& extraTime, const int& num1, const int& num2);
void printAnswer(int answer, colors color);
void setConsole();
void greeting(RecordsObserver* recordsObserver);

int main()
{
    setlocale(0, "");
    setConsole();
    auto *help = new Help();
    auto *recordsObserver = new RecordsObserver();
    greeting(recordsObserver);
    recordsObserver->WriteRecordsInConsole();
    while(1) {
        numsProductGame(recordsObserver, help);
    }
    return 0;
}

void numsProductGame(RecordsObserver* recordsObserver, Help* help)
{
    bool canDeleteLastBadAttempt;
    int num1 = mersenne() % 9000 + 1000;
    int num2 = mersenne() % 9000 + 1000;
    setPosition(38, 2);
    cout << "x";
    setPosition(39, 4);
    cout << "----\n";
    if(!toPause) {
        setPosition(39, 1);
        cout << num1;
        setPosition(39, 3);
        cout << num2;
    }

    int real_answer = num1 * num2;
    if(real_answer < 1e7) cout << " ";
    clock_t tStart = clock();

    double extraTime = 0;
    int input_answer = reversedInput(recordsObserver, tStart, extraTime, num1, num2);
    if(input_answer == -1) return;
    if(!directlyToSmth) {
        int time = (clock() - tStart) / CLOCKS_PER_SEC + round(extraTime);
        canDeleteLastBadAttempt = 1;

        if(input_answer == real_answer) {
            printAnswer(real_answer, GREEN);
            recordsObserver->AddAttempt(num1, num2, time);
            canDeleteLastBadAttempt = 0;
        }
        else printAnswer(real_answer, RED);

        recordsObserver->AddToErrorStatistics(input_answer == real_answer);
        recordsObserver->RefreshRecordsInConsole();

        setPosition(29, 10);
        cout << "Время решения: ";
        int secs = time % 60;
        if(time / 60) {
            cout << time / 60 << ":";
            if(secs < 10) cout << "0";
            cout << secs << "\n";
        }
        else cout << secs << "c\n";
    }
    else directlyToSmth = 0;

    showState = CALCULATION;
    char choice, buffer = '.';
    while(1) {
        toPause = 1;
        if(directlyToHelp) {
            directlyToHelp = 0;
            goto needHelp;
        }
        else if(directlyTo1) {
            directlyTo1 = 0;
            buffer = '1';
        }
        else if(directlyTo5) {
            directlyTo5 = 0;
            buffer = '5';
        }
        else if(directlyTo2) {
            directlyTo2 = 0;
            buffer = '2';
        }
        else if(directlyTo4) {
            directlyTo4 = 0;
            buffer = '4';
        }
        else if(directlyTo0) {
            directlyTo0 = 0;
            buffer = '0';
        }
        else if(directlyToL) {
            directlyToL = 0;
            buffer = 'l';
        }

        if(buffer != '.') {
            choice = buffer;
            buffer = '.';
        }
        else choice = _getch();
        if(choice == 27) {
            delete recordsObserver;
            exit(0);
        }

        if(choice == '1') {
            canDeleteLastBadAttempt = 0;
            showState = SINGLE;
            if(recordsObserver->ShowSingle(buffer)) break;
        }
        else if(choice == '5') {
            canDeleteLastBadAttempt = 0;
            showState = BEST5;
            if(recordsObserver->ShowBest5(buffer)) break;
        }
        else if(choice == '2') {  // best 20
            canDeleteLastBadAttempt = 0;
            showState = BEST20;
            if(recordsObserver->ShowBest20(buffer)) break;
        }
        else if(choice == '4') {  // best 50
            canDeleteLastBadAttempt = 0;
            showState = BEST50;
            if(recordsObserver->ShowBest50(buffer)) break;
        }
        else if(choice == '0') {  // best 100
            canDeleteLastBadAttempt = 0;
            showState = BEST100;
            if(recordsObserver->ShowBest100(buffer)) break;
        }
        else if(choice == 'l' || choice == -92) {
            canDeleteLastBadAttempt = 0;
            showState = LAST;
            if(recordsObserver->ShowLast(buffer)) break;
        }
        else if((choice == 'n' || choice == -30) && showState != CALCULATION) {
            canDeleteLastBadAttempt = 0;
            if(showState == SINGLE) {
                showState = BEST5;
                if(recordsObserver->ShowBest5(buffer)) break;
            }
            else if(showState == BEST5) {
                showState = BEST20;
                if(recordsObserver->ShowBest20(buffer)) break;
            }
            else if(showState == BEST20) {
                showState = BEST50;
                if(recordsObserver->ShowBest50(buffer)) break;
            }
            else if(showState == BEST50) {
                showState = BEST100;
                if(recordsObserver->ShowBest100(buffer)) break;
            }
            else if(showState == BEST100) {
                showState = SINGLE;
                if(recordsObserver->ShowSingle(buffer)) break;
            }
        }
        else if((choice == 'N' || choice == -110) && showState != CALCULATION) {
            canDeleteLastBadAttempt = 0;
            if(showState == SINGLE) {
                showState = BEST100;
                if(recordsObserver->ShowBest100(buffer)) break;
            }
            else if(showState == BEST5) {
                showState = SINGLE;
                if(recordsObserver->ShowSingle(buffer)) break;
            }
            else if(showState == BEST20) {
                showState = BEST5;
                if(recordsObserver->ShowBest5(buffer)) break;
            }
            else if(showState == BEST50) {
                showState = BEST20;
                if(recordsObserver->ShowBest20(buffer)) break;
            }
            else if(showState == BEST100) {
                showState = BEST50;
                if(recordsObserver->ShowBest50(buffer)) break;
            }
        }
        else if(choice == 'd' || choice == -94) {
            if(canDeleteLastBadAttempt) {
                canDeleteLastBadAttempt = 0;
                recordsObserver->DeleteLastFromErrorStatistics();
            }
        }
        else if(choice == 'h' || choice == -32) {
            canDeleteLastBadAttempt = 0;
            needHelp:
            showState = CALCULATION;
            int close = help->ScrollingPages(buffer);
            if(close) {
                delete recordsObserver;
                exit(0);
            }
            if(buffer == '.') {
                recordsObserver->WriteRecordsInConsole();
                return;
            }
        }
        else {
            toPause = 0;
            break;
        }
    }

    if(showState != CALCULATION) {
        recordsObserver->WriteRecordsInConsole();
        return;
    }

    setPosition(32, 5); // мой ответ
    cout << "           ";
    setPosition(32, 6); // реальный ответ
    cout << "           ";
    setPosition(29, 10); // время решения: ...
    cout << "                    ";
}

int reversedInput(RecordsObserver* recordsObserver, clock_t& tStart, double& extraTime, const int& num1, const int& num2)
{
    string input;
    char ch;
    int digit_counter = 0, space_counter = 0;
    int indent = 42;
    if(toPause) {
        toPause = false;
        goto pause;
    }
    setPosition(indent, 5);
    while((ch = _getch()) != 13) {
        if(ch == 'n' || ch == 'N' || ch == -30 || ch == -110) {
            setPosition(32, 5); // стираем написанный ответ если он есть
            cout << "           "; // 11 spaces
            return -1;
        }
        if(ch == 27) {
            delete recordsObserver;
            exit(0);
        }
        if(ch == 32) {  // space
            extraTime += (double) (clock() - tStart) / CLOCKS_PER_SEC;

            pause:
            setPosition(34, 8);
            setColor(LIGHT_RED);
            cout << (char) 17 << " ПАУЗА " << (char) 16;
            setColor(204);
            for(int i = 1; i <= 8; ++i) {
                setPosition(46, i);
                cout << "|";
            }
            for(int i = 1; i <= 8; ++i) {
                setPosition(29, i);
                cout << "|";
            }
            setPosition(39, 1);
            cout << num1;
            setPosition(39, 3);
            cout << num2;
            setColor(WHITE);

            bool nextExercise = 0;
            while(1) {
                ch = _getch();
                if(ch == 27) {
                    delete recordsObserver;
                    exit(0);
                }

                if(ch == 'h' || ch == -32) {
                    directlyToHelp = 1;
                    directlyToSmth = 1;
                    return -2;
                }
                if(ch == '1') {
                    directlyTo1 = 1;
                    directlyToSmth = 1;
                    return -2;
                }
                if(ch == '5') {
                    directlyTo5 = 1;
                    directlyToSmth = 1;
                    return -2;
                }
                if(ch == '2') {
                    directlyTo2 = 1;
                    directlyToSmth = 1;
                    return -2;
                }
                if(ch == '4') {
                    directlyTo4 = 1;
                    directlyToSmth = 1;
                    return -2;
                }
                if(ch == '0') {
                    directlyTo0 = 1;
                    directlyToSmth = 1;
                    return -2;
                }
                if(ch == 'l' || ch == -92) {
                    directlyToL = 1;
                    directlyToSmth = 1;
                    return -2;
                }

                if(ch == 'n' || ch == 'N' || ch == -30 || ch == -110) {
                    nextExercise = 1;
                    break;
                }
                if(ch == 32) break;
            }

            setPosition(34, 8);
            cout << "         ";
            setColor(238);
            for(int i = 1; i <= 8; ++i) {
                setPosition(46, i);
                cout << "|";
            }
            for(int i = 1; i <= 8; ++i) {
                setPosition(29, i);
                cout << "|";
            }
            setColor(WHITE);
            if(nextExercise) {
                setPosition(32, 5); // стираем написанный ответ если он есть
                cout << "           "; // 11 spaces
                return -1;
            }
            setPosition(39, 1);
            cout << num1;
            setPosition(39, 3);
            cout << num2;

            tStart = clock();
            setPosition(indent - digit_counter - space_counter, 5);
            continue;
        }
        if(ch == 8 && digit_counter && input.size()) {  // backspace
            input.erase(0, 1);
            if(digit_counter % 3 == 0) space_counter--;
            digit_counter--;
            setPosition(indent - digit_counter - space_counter, 5);
            cout << " ";
            setPosition(indent - digit_counter - space_counter, 5);
        }
        if(!isdigit(ch) || input.size() == 9) continue;
        cout << ch;
        if(++digit_counter % 3 == 0) space_counter++;
        setPosition(indent - digit_counter - space_counter, 5);
        input = string(1, ch) + input;
    }
    return atoi(input.c_str());
}

void printAnswer(int answer, colors color)
{
    if(color == RED) cout << '\a';
    setPosition(33, 6);
    if(answer < 1e7) cout << " ";
    setColor(color);
    cout << answer / 1000000 << " ";
    int thousands = (answer % 1000000) / 1000;
    if(thousands < 10) cout << "00";
    else if(thousands < 100) cout << "0";
    cout << thousands << " ";
    int units = answer % 1000;
    if(units < 10) cout << "00";
    else if(units < 100) cout << "0";
    cout << units;
    setColor(WHITE);
}

void setConsole()
{
    // ширина и высота монитора компьютера в пикселях
    int monitorWidth = GetSystemMetrics(SM_CXSCREEN);
    int monitorHeight = GetSystemMetrics(SM_CYSCREEN);

    // установка размера текста в консоли
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(fontInfo);
    GetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo);
    fontInfo.dwFontSize.Y = 28;
    SetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo);

    // установка координат появления консоли
    HWND window_header = GetConsoleWindow();
    SetWindowPos(window_header, HWND_TOP, monitorWidth / 2 - 550, monitorHeight / 2 - 280, 500, 500, SWP_SHOWWINDOW);

    // установка длины и высоты консоли в символах
    string mode = "mode con cols=" + to_string(nConsoleWidth) + " lines=" + to_string(nConsoleHeight);
    system(mode.c_str());

    // сделать курсор невидимым
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(hConsole, &structCursorInfo);
    structCursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &structCursorInfo);
}

void greeting(RecordsObserver* recordsObserver)
{
    setColor(10);
    string indent(16, ' ');
    cout << "\n\n" << indent << " _____             ____\
\n" << indent << "|_   _|_ _ _ __   / ___| _ __   __ _  ___ ___\
\n" << indent << "  | |/ _` | '_ \\  \\___ \\| '_ \\ / _` |/ __/ _ \\\
\n" << indent << "  | | (_| | |_) |  ___) | |_) | (_| | (_|  __/\
\n" << indent << "  |_|\\__,_| .__/  |____/| .__/ \\__,_|\\___\\___|\
\n" << indent << "          |_|           |_|";
    setColor(WHITE);
    while(1) {
        char choice = _getch();
        if(choice == 27) {
            delete recordsObserver;
            exit(0);
        }
        if(choice == 32) break;
    }
}

//void help_old(RecordsObserver* recordsObserver, char& buffer)
//{
//    system("cls");
//    setColor(RED);   cout << "\n    " << "Пробел";
//    setColor(HELP); cout << " - пауза/пуск, во время\n    паузы время приостанавливается";
//    setColor(RED);   cout << "\n    " << "n (next)";
//    setColor(HELP); cout << " - следующий пример\n";
//
//    setColor(RED);   cout << "\n    " << "1";
//    setColor(HELP); cout << " - лучший результат";
//    setColor(RED);   cout << "\n    " << "[5, 2, 4, 0]";
//    setColor(HELP); cout << " - лучшие средние из\n    5, 20, 50 и 100 соответственно";
//    setColor(RED);   cout << "\n    " << "l (last)";
//    setColor(HELP); cout << " - последние 100 попыток";
//    setColor(RED);   cout << "\n    " << "Любая другая клавиша";
//    setColor(HELP); cout << " == следу-\n    ющий пример";
//
//    setPosition(39, 1);
//    setColor(2);
//    cout << "Перемещение между рекордами:";
//    setPosition(39, 2);
//    setColor(RED);   cout << "n";
//    setColor(HELP); cout << " - к следующему рекорду";
//    setPosition(39, 3);
//    setColor(RED);   cout << "N";
//    setColor(HELP); cout << " - к предыдущему рекорду";
//
//    setPosition(39, 5);
//    setColor(2);
//    cout << "Перемещение по списку:";
//    setPosition(39, 6);
//    setColor(RED);   cout << "[j, k] или [n, N]";
//    setColor(HELP); cout << " - вниз, вверх";
//    setPosition(39, 7);
//    setColor(RED);   cout << "C^j, C^k";
//    setColor(HELP); cout << " - до конца вниз, до начала";
//    setPosition(39, 8);
//    cout << "вверх (аналогично со стрелками)";
//    setPosition(39, 9);
//    setColor(RED);   cout << "Enter";
//    setColor(HELP); cout << " - возвращение в главный режим";
//    setPosition(39, 10);
//    setColor(RED);   cout << "Esc";
//    setColor(HELP); cout << " - выход";
//    setColor(WHITE);
//
//    again:
//    char key = _getch();
//    if(key == 27) {
//        delete recordsObserver;
//        exit(0);
//    }
//    if(key == '1' || key == '5' || key == '2' || key == '4' || key == '0' ||
//       (key == 'l' || key == -92)) {
//        buffer = key;
//        return;
//    }
//    if(key != 13) goto again;
//    recordsObserver->WriteRecordsInConsole();
////    numsProductGame(recordsObserver);
//}
