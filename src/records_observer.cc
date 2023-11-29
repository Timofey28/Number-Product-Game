#include "records_observer.h"
#include <bits/stdc++.h>
#include <dirent.h>
#include <fcntl.h>
using namespace std;

RecordsObserver::RecordsObserver()
{
    totalTimeAmount_last5 = 0;
    totalTimeAmount_last20 = 0;
    totalTimeAmount_last50 = 0;
    totalTimeAmount_last100 = 0;
    DIR* dir = opendir("Records");
    if(!dir) {
        mkdir("Records");
        mkdir("Records/best");
        solvingsAmount = 0;
        bestAvg5 = 0.0f;
        bestAvg20 = 0.0f;
        bestAvg50 = 0.0f;
        bestAvg100 = 0.0f;
        bestOne.solutionTime = 0;
        newRecord_avg5 = false;
        newRecord_avg20 = false;
        newRecord_avg50 = false;
        newRecord_avg100 = false;
        newRecord_single = false;
    }
    else {
        ifstream fin("Records/history.txt");
        if(!fin) {
            perror("Records/history.txt");
            exit(-1);
        }
        string row;
        int lineAmount = 0;
        while(getline(fin, row)) lineAmount++;
        lineAmount--;
        fin.clear();
        fin.seekg(0);

        fin >> solvingsAmount;
        Attempt attempt;
        int line_counter = 0;
        while(fin >> attempt) {
            line_counter++;
            last100.push(attempt);
            totalTimeAmount_last100 += attempt.solutionTime;
            if(line_counter > lineAmount - 50) {
                last50.push(attempt);
                totalTimeAmount_last50 += attempt.solutionTime;
            }
            if(line_counter > lineAmount - 20) {
                last20.push(attempt);
                totalTimeAmount_last20 += attempt.solutionTime;
            }
            if(line_counter > lineAmount - 5) {
                last5.push(attempt);
                totalTimeAmount_last5 += attempt.solutionTime;
            }
        }
        fin.close();

        fin.open("Records/newRecord_indicator.txt");
        if(!fin) {
            perror("Records/newRecord_indicator.txt");
            exit(-1);
        }
        fin >> newRecord_single >> newRecord_avg5 >> newRecord_avg20 >> newRecord_avg50 >> newRecord_avg100;
        fin.close();

        fin.open("Records/best/difference.txt");
        if(!fin) {
            perror("Records/best/difference.txt");
            exit(-1);
        }
        fin >> differenceWithPrev_single >> differenceWithPrev_bestAvg5 >> differenceWithPrev_bestAvg20 >> differenceWithPrev_bestAvg50 >> differenceWithPrev_bestAvg100;
        fin.close();

        fin.open("Records/best/avg5.txt");
        if(!fin) {
            perror("Records/best/avg5.txt");
            exit(-1);
        }
        fin >> bestAvg5;
        if(bestAvg5) {
            for(int i = 0; i < 5; ++i)
                fin >> best5[i];
        }
        fin.close();

        fin.open("Records/best/avg20.txt");
        if(!fin) {
            perror("Records/best/avg20.txt");
            exit(-1);
        }
        fin >> bestAvg20;
        if(bestAvg20) {
            for(int i = 0; i < 20; ++i)
                fin >> best20[i];
        }
        fin.close();

        fin.open("Records/best/avg50.txt");
        if(!fin) {
            perror("Records/best/avg50.txt");
            exit(-1);
        }
        fin >> bestAvg50;
        if(bestAvg50) {
            for(int i = 0; i < 50; ++i)
                fin >> best50[i];
        }
        fin.close();

        fin.open("Records/best/avg100.txt");
        if(!fin) {
            perror("Records/best/avg100.txt");
            exit(-1);
        }
        fin >> bestAvg100;
        if(bestAvg100) {
            for(int i = 0; i < 100; ++i)
                fin >> best100[i];
        }
        fin.close();

        fin.open("Records/best/single.txt");
        if(!fin) {
            perror("Records/best/single.txt");
            exit(-1);
        }
        bool exists;
        fin >> exists;
        if(exists) fin >> bestOne;
        else bestOne.solutionTime = 0;
        fin.close();
        closedir(dir);
    }
}

void RecordsObserver::saveNewData()
{
    ofstream fout("Records/history.txt");
    fout << solvingsAmount << '\n';
    while(!last100.empty()) {
        fout << last100.front();
        last100.pop();
    }
    fout.close();

    fout.open("Records/best/avg5.txt");
    fout << bestAvg5 << '\n';
    if(bestAvg5) {
        for(auto& attempt : best5)
            fout << attempt;
    }
    fout.close();

    fout.open("Records/best/avg20.txt");
    fout << bestAvg20 << '\n';
    if(bestAvg20) {
        for(auto& attempt : best20)
            fout << attempt;
    }
    fout.close();

    fout.open("Records/best/avg50.txt");
    fout << bestAvg50 << '\n';
    if(bestAvg50) {
        for(auto& attempt : best50)
            fout << attempt;
    }
    fout.close();

    fout.open("Records/best/avg100.txt");
    fout << bestAvg100 << '\n';
    if(bestAvg100) {
        for(auto& attempt : best100)
            fout << attempt;
    }
    fout.close();

    fout.open("Records/best/single.txt");
    if(bestOne.solutionTime) fout << 1 << '\n' << bestOne;
    else fout << 0;
    fout.close();

    fout.open("Records/best/difference.txt");
    fout << differenceWithPrev_single << ' ' << differenceWithPrev_bestAvg5 << ' ' << differenceWithPrev_bestAvg20 << ' ';
    fout << differenceWithPrev_bestAvg50 << ' ' << differenceWithPrev_bestAvg100;
    fout.close();

    fout.open("Records/newRecord_indicator.txt");
    fout << newRecord_single << ' ' << newRecord_avg5 << ' ' << newRecord_avg20 << ' ' << newRecord_avg50 << ' ' << newRecord_avg100;
    fout.close();
}

void RecordsObserver::addAttempt(int num1, int num2, int solutionTime)
{
    solvingsAmount++;
    Attempt attempt;
    attempt.num1 = num1;
    attempt.num2 = num2;
    attempt.solutionTime = solutionTime;
    attempt.timePoint = time(nullptr);

    last5.push(attempt);
    totalTimeAmount_last5 += solutionTime;
    if(last5.size() == 6) {
        totalTimeAmount_last5 -= last5.front().solutionTime;
        last5.pop();
    }
    last20.push(attempt);
    totalTimeAmount_last20 += solutionTime;
    if(last20.size() == 21) {
        totalTimeAmount_last20 -= last20.front().solutionTime;
        last20.pop();
    }
    last50.push(attempt);
    totalTimeAmount_last50 += solutionTime;
    if(last50.size() == 51) {
        totalTimeAmount_last50 -= last50.front().solutionTime;
        last50.pop();
    }
    last100.push(attempt);
    totalTimeAmount_last100 += solutionTime;
    if(last100.size() == 101) {
        totalTimeAmount_last100 -= last100.front().solutionTime;
        last100.pop();
    }
    if(!bestOne.solutionTime || attempt.solutionTime < bestOne.solutionTime) {
        if(bestOne.solutionTime) differenceWithPrev_single = bestOne.solutionTime - attempt.solutionTime;
        else differenceWithPrev_single = -1;
        bestOne = attempt;
        newRecord_single = true;
    }
    else newRecord_single = false;

    refreshBests();
}

void RecordsObserver::refreshBests()
{
    double currAvg;
    queue<Attempt> lastCopy;

    if(last5.size() == 5) {
        currAvg = (double) totalTimeAmount_last5 / 5;
        if(!bestAvg5 || currAvg < bestAvg5) {
            newRecord_avg5 = true;
            if(bestAvg5) differenceWithPrev_bestAvg5 = bestAvg5 - currAvg;
            else differenceWithPrev_bestAvg5 = -1;
            bestAvg5 = currAvg;
            lastCopy = last5;
            for(int i = 4; ~i; --i) {
                best5[i] = lastCopy.front();
                lastCopy.pop();
            }
        }
        else newRecord_avg5 = false;
    }

    if(last20.size() == 20) {
        currAvg = (double) totalTimeAmount_last20 / 20;
        if(!bestAvg20 || currAvg < bestAvg20) {
            newRecord_avg20 = true;
            if(bestAvg20) differenceWithPrev_bestAvg20 = bestAvg20 - currAvg;
            else differenceWithPrev_bestAvg20 = -1;
            bestAvg20 = currAvg;
            lastCopy = last20;
            for(int i = 19; ~i; --i) {
                best20[i] = lastCopy.front();
                lastCopy.pop();
            }
        }
        else newRecord_avg20 = false;
    }

    if(last50.size() == 50) {
        currAvg = (double) totalTimeAmount_last50 / 50;
        if(!bestAvg50 || currAvg < bestAvg50) {
            newRecord_avg50 = true;
            if(bestAvg50) differenceWithPrev_bestAvg50 = bestAvg50 - currAvg;
            else differenceWithPrev_bestAvg50 = -1;
            bestAvg50 = currAvg;
            lastCopy = last50;
            for(int i = 49; ~i; --i) {
                best50[i] = lastCopy.front();
                lastCopy.pop();
            }
        }
        else newRecord_avg50 = false;
    }

    if(last100.size() == 100) {
        currAvg = (double) totalTimeAmount_last100 / 100;
        if(!bestAvg100 || currAvg < bestAvg100) {
            newRecord_avg100 = true;
            if(bestAvg100) differenceWithPrev_bestAvg100 = bestAvg100 - currAvg;
            else differenceWithPrev_bestAvg100 = -1;
            bestAvg100 = currAvg;
            lastCopy = last100;
            for(int i = 99; ~i; --i) {
                best100[i] = lastCopy.front();
                lastCopy.pop();
            }
        }
        else newRecord_avg100 = false;
    }
}

bool RecordsObserver::showSingle(char& buffer)
{
    system("cls");
    if(!has_single()) {
        cout << "\n\tNo attempts yet";
        return 0;
    }
    setColor(ORANGE);
    cout << "\n\tЛучшая попытка за все время";
    setColor(WHITE);
    cout << "\n\n\tДата рекорда: ";
    setColor(SEA_WAVE);
    time_t timer = bestOne.timePoint;
    tm* time_ptr = localtime(&timer);
    map<int, string> weekDay = {{1, "Понедельник"}, {2, "Вторник"}, {3, "Среда"}, {4, "Четверг"}, {5, "Пятница"}, {6, "Суббота"}, {0, "Воскресенье"}};
    map<int, string> mon = {{0, "января"}, {1, "февраля"}, {2, "марта"}, {3, "апреля"}, {4, "мая"}, {5, "июня"}, {6, "июля"}, {7, "августа"}, {8, "сентября"}, {9, "октября"}, {10, "ноября"}, {11, "декабря"}};
    cout << weekDay[time_ptr->tm_wday] << ", " << time_ptr->tm_mday << " " << mon[time_ptr->tm_mon] << " " << time_ptr->tm_year + 1900 << " года";
    char buf[20];
    if(strftime(buf, sizeof(buf), ", %H:%M:%S", time_ptr)) cout << buf;
    setColor(WHITE);

    cout << "\n\n\n\t" << bestOne.num1 << " x " << bestOne.num2 << " = ";
    setColor(GREEN);
    cout << setw(10) << prettifyAnswer(bestOne.num1 * bestOne.num2);
    setColor(WHITE);
    cout << "  ->  " << setw(5) << timeFormat(bestOne.solutionTime, 1);

    char choice;
    while(1) {
        choice = _getch();
        if(choice == '5' || choice == '2' || choice == '4' || choice == '0' ||
           (choice == 'n' || choice == -30 || choice == 'N' || choice == -110) ||
           (choice == 'l' || choice == -92 || choice == 'h' || choice == -32)) {
            buffer = choice;
            return 0;
        }
        if(choice == 27) {
            saveNewData();
            exit(0);
        }
        if(choice == 13) return 1;
    }
}

bool RecordsObserver::showBest5(char& buffer)
{
    system("cls");
    if(!has_bestAvg5()) {
        cout << "\n\tПопыток менее 5";
        return 0;
    }
    setColor(ORANGE);
    cout << "\n\tЛучшие 5 попыток";
    setColor(WHITE);
    cout << "\n\n\tДата рекорда: ";
    setColor(SEA_WAVE);
    time_t timer = best5[0].timePoint;
    tm* time_ptr = localtime(&timer);
    map<int, string> weekDay = {{1, "Понедельник"}, {2, "Вторник"}, {3, "Среда"}, {4, "Четверг"}, {5, "Пятница"}, {6, "Суббота"}, {0, "Воскресенье"}};
    map<int, string> mon = {{0, "января"}, {1, "февраля"}, {2, "марта"}, {3, "апреля"}, {4, "мая"}, {5, "июня"}, {6, "июля"}, {7, "августа"}, {8, "сентября"}, {9, "октября"}, {10, "ноября"}, {11, "декабря"}};
    cout << weekDay[time_ptr->tm_wday] << ", " << time_ptr->tm_mday << " " << mon[time_ptr->tm_mon] << " " << time_ptr->tm_year + 1900 << " года";
    char buf[20];
    if(strftime(buf, sizeof(buf), ", %H:%M:%S", time_ptr)) cout << buf;
    setColor(WHITE);
    cout << "\n\tСреднее время решения: ";
    setColor(SEA_WAVE);
    cout << timeFormat(bestAvg5, 1) << endl;
    setColor(WHITE);

    for(int i = 0; i < 5; ++i) {
        cout << "\n\t" << i + 1 << ") " << best5[i].num1 << " x " << best5[i].num2 << " = ";
        setColor(GREEN);
        cout << setw(10) << prettifyAnswer(best5[i].num1 * best5[i].num2);
        setColor(WHITE);
        cout << "  ->  " << setw(5) << timeFormat(best5[i].solutionTime, 1);
        timer = best5[i].timePoint;
        time_ptr = localtime(&timer);
        if(strftime(buf, sizeof(buf), "%d.%m.%y  %H:%M", time_ptr)) {
            cout << setw(19) << buf;
        }
    }

    char choice;
    while(1) {
        choice = _getch();
        if(choice == '1' || choice == '2' || choice == '4' || choice == '0' ||
           (choice == 'n' || choice == -30 || choice == 'N' || choice == -110) ||
           (choice == 'l' || choice == -92 || choice == 'h' || choice == -32)) {
            buffer = choice;
            return 0;
        }
        if(choice == 27) {
            saveNewData();
            exit(0);
        }
        if(choice == 13) return 1;
    }
}

bool RecordsObserver::showBest20(char& buffer)
{
    system("cls");
    if(!has_bestAvg20()) {
        cout << "\n\tПопыток менее 20";
        return 0;
    }
    time_t timer = best20[0].timePoint;
    tm* time_ptr = localtime(&timer);
    char buf[20];
    int linesShowed = 4;

    setColor(ORANGE);
    cout << "\n\tЛучшие 20 попыток";
    setColor(WHITE);
    cout << "\n\n\tДата рекорда: ";
    setColor(SEA_WAVE);
    map<int, string> weekDay = {{1, "Понедельник"}, {2, "Вторник"}, {3, "Среда"}, {4, "Четверг"}, {5, "Пятница"}, {6, "Суббота"}, {0, "Воскресенье"}};
    map<int, string> mon = {{0, "января"}, {1, "февраля"}, {2, "марта"}, {3, "апреля"}, {4, "мая"}, {5, "июня"}, {6, "июля"}, {7, "августа"}, {8, "сентября"}, {9, "октября"}, {10, "ноября"}, {11, "декабря"}};
    cout << weekDay[time_ptr->tm_wday] << ", " << time_ptr->tm_mday << " " << mon[time_ptr->tm_mon] << " " << time_ptr->tm_year + 1900 << " года";
    if(strftime(buf, sizeof(buf), ", %H:%M:%S", time_ptr)) cout << buf;
    setColor(WHITE);
    cout << "\n\tСреднее время решения: ";
    setColor(SEA_WAVE);
    cout << timeFormat(bestAvg20, 1) << endl;

    setColor(GRAY);
    _setmode(_fileno(stdout), _O_U16TEXT);
    wcout << "\t   " << (wchar_t) 0x25B2;
    _setmode(_fileno(stdout), _O_TEXT);
    setColor(WHITE);
    for(int i = 0; i < linesShowed && i < best20.size(); ++i) {
        cout << "\n\t";
        writeAttemptLine(best20[i], i + 1);
    }
    if(best20.size() <= linesShowed) setColor(GRAY);
    setPosition(11, 5 + linesShowed + 1);
    _setmode(_fileno(stdout), _O_U16TEXT);
    wcout << (wchar_t) 0x25BC;
    _setmode(_fileno(stdout), _O_TEXT);
    setColor(WHITE);

    char choice;
    int startFrom = 0;
    while(1) {
        choice = _getch();
        if(choice == -32 && kbhit()) choice = _getch();

        if(choice == 'j' || choice == -82 || choice == 80) {  // down
            if(best20.size() - startFrom <= linesShowed) continue;
            startFrom++;
        }
        else if(choice == 'k' || choice == -85 || choice == 72) {  // up
            if(startFrom == 0) continue;
            startFrom--;
        }
        else if(choice == -111 || choice == 10) {  // ctrl + down
            if(best20.size() - startFrom <= linesShowed) continue;
            startFrom = max(0, (int) best20.size() - linesShowed);
        }
        else if(choice == -115 || choice == 11) {  // ctrl + up
            if(startFrom == 0) continue;
            startFrom = 0;
        }

        if(choice == '1' || choice == '5' || choice == '4' || choice == '0' ||
           (choice == 'n' || choice == 'N' || choice == -30 || choice == -110) ||
           (choice == 'l' || choice == -92 || choice == 'h' || choice == -32)) {
            buffer = choice;
            return 0;
        }
        if(choice == 27) {
            saveNewData();
            exit(0);
        }
        if(choice == 13) return 1;

        // output
        setPosition(11, 5);
        if(!startFrom) setColor(GRAY);
        _setmode(_fileno(stdout), _O_U16TEXT);
        wcout << (wchar_t) 0x25B2;
        _setmode(_fileno(stdout), _O_TEXT);
        setColor(WHITE);
        for(int i = startFrom; i < startFrom + linesShowed && i < best20.size(); ++i) {
            cout << "\n\t";
            writeAttemptLine(best20[i], i + 1);
        }
        setPosition(11, 5 + linesShowed + 1);
        if(best20.size() - startFrom <= linesShowed) setColor(GRAY);
        _setmode(_fileno(stdout), _O_U16TEXT);
        wcout << (wchar_t) 0x25BC;
        _setmode(_fileno(stdout), _O_TEXT);
        setColor(WHITE);
    }

    return 0;
}

bool RecordsObserver::showBest50(char& buffer)
{
    system("cls");
    if(!has_bestAvg50()) {
        cout << "\n\tПопыток менее 50";
        return 0;
    }
    time_t timer = best50[0].timePoint;
    tm* time_ptr = localtime(&timer);
    char buf[20];
    int linesShowed = 4;

    setColor(ORANGE);
    cout << "\n\tЛучшие 50 попыток";
    setColor(WHITE);
    cout << "\n\n\tДата рекорда: ";
    setColor(SEA_WAVE);
    map<int, string> weekDay = {{1, "Понедельник"}, {2, "Вторник"}, {3, "Среда"}, {4, "Четверг"}, {5, "Пятница"}, {6, "Суббота"}, {0, "Воскресенье"}};
    map<int, string> mon = {{0, "января"}, {1, "февраля"}, {2, "марта"}, {3, "апреля"}, {4, "мая"}, {5, "июня"}, {6, "июля"}, {7, "августа"}, {8, "сентября"}, {9, "октября"}, {10, "ноября"}, {11, "декабря"}};
    cout << weekDay[time_ptr->tm_wday] << ", " << time_ptr->tm_mday << " " << mon[time_ptr->tm_mon] << " " << time_ptr->tm_year + 1900 << " года";
    if(strftime(buf, sizeof(buf), ", %H:%M:%S", time_ptr)) cout << buf;
    setColor(WHITE);
    cout << "\n\tСреднее время решения: ";
    setColor(SEA_WAVE);
    cout << timeFormat(bestAvg50, 1) << endl;

    setColor(GRAY);
    _setmode(_fileno(stdout), _O_U16TEXT);
    wcout << "\t   " << (wchar_t) 0x25B2;
    _setmode(_fileno(stdout), _O_TEXT);
    setColor(WHITE);
    for(int i = 0; i < linesShowed && i < best50.size(); ++i) {
        cout << "\n\t";
        writeAttemptLine(best50[i], i + 1);
    }
    if(best50.size() <= linesShowed) setColor(GRAY);
    setPosition(11, 5 + linesShowed + 1);
    _setmode(_fileno(stdout), _O_U16TEXT);
    wcout << (wchar_t) 0x25BC;
    _setmode(_fileno(stdout), _O_TEXT);
    setColor(WHITE);

    char choice;
    int startFrom = 0;
    while(1) {
        choice = _getch();
        if(choice == -32 && kbhit()) choice = _getch();

        if(choice == 'j' || choice == -82 || choice == 80) {  // down
            if(best50.size() - startFrom <= linesShowed) continue;
            startFrom++;
        }
        else if(choice == 'k' || choice == -85 || choice == 72) {  // up
            if(startFrom == 0) continue;
            startFrom--;
        }
        else if(choice == -111 || choice == 10) {  // ctrl + down
            if(best50.size() - startFrom <= linesShowed) continue;
            startFrom = max(0, (int) best50.size() - linesShowed);
        }
        else if(choice == -115 || choice == 11) {  // ctrl + up
            if(startFrom == 0) continue;
            startFrom = 0;
        }

        if(choice == '1' || choice == '5' || choice == '2' || choice == '0' ||
           (choice == 'n' || choice == 'N' || choice == -30 || choice == -110) ||
           (choice == 'l' || choice == -92 || choice == 'h' || choice == -32)) {
            buffer = choice;
            return 0;
        }
        if(choice == 27) {
            saveNewData();
            exit(0);
        }
        if(choice == 13) return 1;

        // output
        setPosition(11, 5);
        if(!startFrom) setColor(GRAY);
        _setmode(_fileno(stdout), _O_U16TEXT);
        wcout << (wchar_t) 0x25B2;
        _setmode(_fileno(stdout), _O_TEXT);
        setColor(WHITE);
        for(int i = startFrom; i < startFrom + linesShowed && i < best50.size(); ++i) {
            cout << "\n\t";
            writeAttemptLine(best50[i], i + 1);
        }
        setPosition(11, 5 + linesShowed + 1);
        if(best50.size() - startFrom <= linesShowed) setColor(GRAY);
        _setmode(_fileno(stdout), _O_U16TEXT);
        wcout << (wchar_t) 0x25BC;
        _setmode(_fileno(stdout), _O_TEXT);
        setColor(WHITE);
    }

    return 0;
}

bool RecordsObserver::showBest100(char& buffer)
{
    system("cls");
    if(!has_bestAvg100()) {
        cout << "\n\tПопыток менее 100";
        return 0;
    }
    time_t timer = best100[0].timePoint;
    tm* time_ptr = localtime(&timer);
    char buf[20];
    int linesShowed = 4;

    setColor(ORANGE);
    cout << "\n\tЛучшие 100 попыток";
    setColor(WHITE);
    cout << "\n\n\tДата рекорда: ";
    setColor(SEA_WAVE);
    map<int, string> weekDay = {{1, "Понедельник"}, {2, "Вторник"}, {3, "Среда"}, {4, "Четверг"}, {5, "Пятница"}, {6, "Суббота"}, {0, "Воскресенье"}};
    map<int, string> mon = {{0, "января"}, {1, "февраля"}, {2, "марта"}, {3, "апреля"}, {4, "мая"}, {5, "июня"}, {6, "июля"}, {7, "августа"}, {8, "сентября"}, {9, "октября"}, {10, "ноября"}, {11, "декабря"}};
    cout << weekDay[time_ptr->tm_wday] << ", " << time_ptr->tm_mday << " " << mon[time_ptr->tm_mon] << " " << time_ptr->tm_year + 1900 << " года";
    if(strftime(buf, sizeof(buf), ", %H:%M:%S", time_ptr)) cout << buf;
    setColor(WHITE);
    cout << "\n\tСреднее время решения: ";
    setColor(SEA_WAVE);
    cout << timeFormat(bestAvg100, 1) << endl;

    setColor(GRAY);
    _setmode(_fileno(stdout), _O_U16TEXT);
    wcout << "\t   " << (wchar_t) 0x25B2;
    _setmode(_fileno(stdout), _O_TEXT);
    setColor(WHITE);
    for(int i = 0; i < linesShowed && i < best100.size(); ++i) {
        cout << "\n\t";
        writeAttemptLine(best100[i], i + 1);
    }
    if(best100.size() <= linesShowed) setColor(GRAY);
    setPosition(11, 5 + linesShowed + 1);
    _setmode(_fileno(stdout), _O_U16TEXT);
    wcout << (wchar_t) 0x25BC;
    _setmode(_fileno(stdout), _O_TEXT);
    setColor(WHITE);

    char choice;
    int startFrom = 0;
    while(1) {
        choice = _getch();
        if(choice == -32 && kbhit()) choice = _getch();

        if(choice == 'j' || choice == -82 || choice == 80) {  // down
            if(best100.size() - startFrom <= linesShowed) continue;
            startFrom++;
        }
        else if(choice == 'k' || choice == -85 || choice == 72) {  // up
            if(startFrom == 0) continue;
            startFrom--;
        }
        else if(choice == -111 || choice == 10) {  // ctrl + down
            if(best100.size() - startFrom <= linesShowed) continue;
            startFrom = max(0, (int) best100.size() - linesShowed);
        }
        else if(choice == -115 || choice == 11) {  // ctrl + up
            if(startFrom == 0) continue;
            startFrom = 0;
        }

        if(choice == '1' || choice == '5' || choice == '2' || choice == '4' ||
           (choice == 'n' || choice == -30 || choice == 'N' || choice == -110) ||
           (choice == 'l' || choice == -92 || choice == 'h' || choice == -32)) {
            buffer = choice;
            return 0;
        }
        if(choice == 27) {
            saveNewData();
            exit(0);
        }
        if(choice == 13) return 1;

        // output
        setPosition(11, 5);
        if(!startFrom) setColor(GRAY);
        _setmode(_fileno(stdout), _O_U16TEXT);
        wcout << (wchar_t) 0x25B2;
        _setmode(_fileno(stdout), _O_TEXT);
        setColor(WHITE);
        for(int i = startFrom; i < startFrom + linesShowed && i < best100.size(); ++i) {
            cout << "\n\t";
            writeAttemptLine(best100[i], i + 1);
        }
        setPosition(11, 5 + linesShowed + 1);
        if(best100.size() - startFrom <= linesShowed) setColor(GRAY);
        _setmode(_fileno(stdout), _O_U16TEXT);
        wcout << (wchar_t) 0x25BC;
        _setmode(_fileno(stdout), _O_TEXT);
        setColor(WHITE);
    }

    return 0;
}

bool RecordsObserver::showLast(char& buffer)
{
    system("cls");
    if(!last100.size()) {
        cout << "\n\tNo attempts yet";
        return 0;
    }
    time_t timer = time(nullptr);
    tm* time_ptr = localtime(&timer);
    char buf[20];
    int linesShowed = 6;
    vector<Attempt> lastAttempts(last100.size());
    queue<Attempt> last100_copy(last100);
    for(int i = lastAttempts.size() - 1; ~i; --i) {
        lastAttempts[i] = last100_copy.front();
        last100_copy.pop();
    }

    setColor(ORANGE);
    cout << "\n\tПоследние 100 попыток\n\n\t   ";
    setColor(GRAY);
    _setmode(_fileno(stdout), _O_U16TEXT);
    wcout << (wchar_t) 0x25B2;
    _setmode(_fileno(stdout), _O_TEXT);
    setColor(WHITE);
    for(int i = 0; i < linesShowed && i < lastAttempts.size(); ++i) {
        cout << "\n\t";
        writeAttemptLine(lastAttempts[i], i + 1);
    }
    if(lastAttempts.size() <= linesShowed) setColor(GRAY);
    setPosition(11, 3 + linesShowed + 1);
    _setmode(_fileno(stdout), _O_U16TEXT);
    wcout << (wchar_t) 0x25BC;
    _setmode(_fileno(stdout), _O_TEXT);
    setColor(WHITE);

    /////////////// avg 5, 10, 15, 20
    int totalTimeAmount_last10 = totalTimeAmount_last5,
        totalTimeAmount_last15 = totalTimeAmount_last5;
    if(last100.size() >= 10) {
        for(int i = 5; i < 10; ++i) {
            totalTimeAmount_last10 += lastAttempts[i].solutionTime;
            totalTimeAmount_last15 += lastAttempts[i].solutionTime;
        }
    }
    if(last100.size() >= 15) {
        for(int i = 10; i < 15; ++i) {
            totalTimeAmount_last15 += lastAttempts[i].solutionTime;
        }
    }
    int fraction; // дробная часть

    setPosition(40, 1);
    cout << "Avg 5:  ";
    setColor(BEIGE);
    if(last100.size() < 5) cout << "...";
    else {
        if((double) totalTimeAmount_last5 / 5 < 10) cout << " ";
        cout << totalTimeAmount_last5 / 5;
        fraction = ((int) (((double) totalTimeAmount_last5 / 5) * 100)) % 100;
        if(fraction % 10 == 0) fraction /= 10;
        if(fraction) cout << "." << fraction;
    }
    setColor(WHITE);

    setPosition(40, 2);
    cout << "Avg 10: ";
    setColor(BEIGE);
    if(last100.size() < 10) cout << "...";
    else {
        if((double) totalTimeAmount_last10 / 10 < 10) cout << " ";
        cout << totalTimeAmount_last10 / 10;
        fraction = ((int) (((double) totalTimeAmount_last10 / 10) * 100)) % 100;
        if(fraction % 10 == 0) fraction /= 10;
        if(fraction) cout << "." << fraction;
    }
    setColor(WHITE);

    setPosition(56, 1);
    cout << "Avg 15: ";
    setColor(BEIGE);
    if(last100.size() < 15) cout << "...";
    else {
        if((double) totalTimeAmount_last15 / 15 < 10) cout << " ";
        cout << totalTimeAmount_last15 / 15;
        double avg = round(((double) totalTimeAmount_last15 / 15) * 100) / 100;
        fraction = ((int) (avg * 100)) % 100;
        if(fraction % 10 == 0) fraction /= 10;
        if(fraction) cout << "." << fraction;
    }
    setColor(WHITE);

    setPosition(56, 2);
    cout << "Avg 20: ";
    setColor(BEIGE);
    if(last100.size() < 20) cout << "...";
    else {
        if((double) totalTimeAmount_last20 / 20 < 10) cout << " ";
        cout << totalTimeAmount_last20 / 20;
        fraction = ((int) (((double) totalTimeAmount_last20 / 20) * 100)) % 100;
        if(fraction % 10 == 0) fraction /= 10;
        if(fraction) cout << "." << fraction;
    }
    setColor(WHITE);
    ///////////////

    char choice;
    int startFrom = 0;
    string request;
    while(1) {
        nextMove:
        choice = _getch();
        if(choice == -32 && kbhit()) choice = _getch();

        if(choice == 'n' || choice == -30 || choice == 'j' || choice == -82 || choice == 80) {  // down
            if(lastAttempts.size() - startFrom <= linesShowed) continue;
            startFrom++;
        }
        else if(choice == 'N' || choice == -110 || choice == 'k' || choice == -85 || choice == 72) {  // up
            if(startFrom == 0) continue;
            startFrom--;
        }
        else if(choice == -111 || choice == 10) {  // ctrl + down, ctrl + j
            if(lastAttempts.size() - startFrom <= linesShowed) continue;
            startFrom = max(0, (int) lastAttempts.size() - linesShowed);
        }
        else if(choice == -115 || choice == 11) {  // ctrl + up, ctrl + k
            if(startFrom == 0) continue;
            startFrom = 0;
        }
        else if(choice == '/' || choice == 46) {
            request = "/";
            setPosition(8, 2);
            cout << request;
            while(1) {
                choice = _getch();
                if(choice == 32) continue; // space
                if(choice == 8) { // backspace
                    request.pop_back();
                    setPosition(8 + request.size(), 2);
                    cout << ' ';
                    if(request == "") goto nextMove;
                    setPosition(8 + request.size(), 2);
                }
                else if(choice == 13) { // enter
                    if(request.size() == 1) {
                        setPosition(8, 2);
                        cout << ' ';
                        goto nextMove;
                    }
                    break;
                }
                else {
                    if(!(choice == '<' && request.find("<") != string::npos ||
                       choice == '>' && request.find(">") != string::npos ||
                       choice == '=' && request.find("=") != string::npos ||
                       choice == '.' && count(request.begin(), request.end(), '.') >= 2)) {
                        request += choice;
                        cout << choice;
                    }
                }
            }
            int timeRestriction1 = 0, timeRestriction2 = 0, pointsAmount = 0;
            bool below = 0, same = 0, more = 0;
            bool anyDigits = 0;
            if(request == "/moda") {
                map<int, int> m;
                multimap<int, int> mm;
                for(int i = 0; i < lastAttempts.size(); ++i) m[lastAttempts[i].solutionTime]++;
                for(auto it = m.begin(); it != m.end(); ++it) mm.insert(make_pair(it->second, it->first));

                setPosition(8, 3);
                for(int i = 0; i < linesShowed + 2; ++i) {
                    cout << string(65, ' ') << "\n\t";
                }
                setPosition(8, 2);
                setColor(GRAY);
                cout << "[q, l] - обратно\n\n\t";

                setColor(WHITE);
                multimap<int, int>::iterator it;
                int i = 0;
                for(it = --mm.end(); it != mm.begin() && i < 6; --it, ++i) {
                    if(!i) setColor(ORANGE);
                    cout << it->second << "c " << (char) 26 << " " << it->first << " раз";
                    if(it->first % 10 == 2 || it->first % 10 == 3 || it->first % 10 == 4 &&
                       !(it->first >= 12 && it->first <= 14)) cout << "а";
                    cout << "\n\t";
                    if(!i) setColor(WHITE);
                }
                if(i < 6) {
                    cout << it->second << "c " << (char) 26 << " " << it->first << " раз";
                    if(it->first % 10 == 2 || it->first % 10 == 3 || it->first % 10 == 4 &&
                       !(it->first >= 12 && it->first <= 14)) cout << "а";
                    cout << "\n\t";
                }
                else {
                    i = 0;
                    while(it != mm.begin() && i < 6) {
                        setPosition(26, 4 + i);
                        cout << it->second << "c " << (char) 26 << " " << it->first << " раз";
                        if(it->first % 10 == 2 || it->first % 10 == 3 || it->first % 10 == 4 &&
                           !(it->first >= 12 && it->first <= 14)) cout << "а";
                        --it; ++i;
                    }
                    if(i < 6) {
                        cout << it->second << "c " << (char) 26 << " " << it->first << " раз";
                        if(it->first % 10 == 2 || it->first % 10 == 3 || it->first % 10 == 4 &&
                           !(it->first >= 12 && it->first <= 14)) cout << "а";
                    }
                    else {
                        i = 0;
                        while(it != mm.begin() && i < 6) {
                            setPosition(44, 4 + i);
                            cout << it->second << "c " << (char) 26 << " " << it->first << " раз";
                            if(it->first % 10 == 2 || it->first % 10 == 3 || it->first % 10 == 4 &&
                              !(it->first >= 12 && it->first <= 14)) cout << "а";
                            --it; ++i;
                        }
                        if(i < 6) {
                            cout << it->second << "c " << (char) 26 << " " << it->first << " раз";
                            if(it->first % 10 == 2 || it->first % 10 == 3 || it->first % 10 == 4 &&
                              !(it->first >= 12 && it->first <= 14)) cout << "а";
                        }
                    }
                }
                while(1) {
                    choice = _getch();
                    if(choice == 'q' || choice == -87 || choice == 'l' || choice == -92) {
                        setPosition(8, 2);
                        cout << string(32, ' ');
                        goto normalOutput;
                    }
                    if(choice == '1' || choice == '5' || choice == '2' || choice == '4' || choice == '0' ||
                       choice == 'h' || choice == -32) {
                        buffer = choice;
                        return 0;
                    }
                    if(choice == 27) {
                        saveNewData();
                        exit(0);
                    }
                    if(choice == 13) return 1;
                }
            }
            for(int i = 1; i < request.size(); ++i) {
                if(request[i] == '<') below = 1;
                else if(request[i] == '>') more = 1;
                else if(request[i] == '=') same = 1;
                else if(request[i] == '.') pointsAmount++;
                else if(isdigit(request[i])) {
                    anyDigits = 1;
                    if(pointsAmount < 2) timeRestriction1 = timeRestriction1*10 + (request[i] - 48);
                    else timeRestriction2 = timeRestriction2*10 + (request[i] - 48);
                    if(timeRestriction1 >= 1000 || timeRestriction2 >= 1000) {
                        setPosition(8, 2);
                        cout << string(request.size(), ' ');
                        goto nextMove;
                    }
                }
                else {
                    setPosition(8, 2);
                    cout << string(request.size(), ' ');
                    goto nextMove;
                }
            }
            if(pointsAmount == 2 && timeRestriction2 == 0) timeRestriction2 = 1e4;
            if((!below && !same && !more || below && more || !anyDigits) && pointsAmount != 2 ||
               (below || same || more) && (pointsAmount == 1 || pointsAmount == 2)) // если все вместе: и точки, и знаки сравнения
            {
                setPosition(8, 2);
                cout << string(request.size(), ' ');
                goto nextMove;
            }
            vector<Attempt> suitableAttempts;
            vector<int> attemptNo;
            for(int i = 0; i < lastAttempts.size(); ++i) {
                int st = lastAttempts[i].solutionTime;
                if(below && st < timeRestriction1 ||
                   same && st == timeRestriction1 ||
                   more && st > timeRestriction1 ||
                   pointsAmount == 2 && st >= timeRestriction1 && st <= timeRestriction2) {
                        suitableAttempts.push_back(lastAttempts[i]);
                        attemptNo.push_back(i);
                }
            }
            setPosition(8, 2);
            setColor(GRAY);
            request.erase(0, 1);
            if(pointsAmount == 2) {
                if(timeRestriction1 == 0) {
                    _setmode(_fileno(stdout), _O_U16TEXT);
                    wcout << "(-" << (wchar_t) 0x221E << ", ";
                    _setmode(_fileno(stdout), _O_TEXT);
                }
                else cout << "[" << timeRestriction1 << ", ";
                if(timeRestriction2 == 1e4) {
                    _setmode(_fileno(stdout), _O_U16TEXT);
                    wcout << "+" << (wchar_t) 0x221E << ") ";
                    _setmode(_fileno(stdout), _O_TEXT);
                }
                else cout << timeRestriction2 << "] ";
            }
            else cout << request << ' ';
            int amount = suitableAttempts.size();
            cout << (char) 26 << ' ' << amount << " попыт";
            if(amount % 10 == 1 && !(amount >= 11 && amount < 20)) cout << "ка";
            else if(amount % 10 == 2 || amount % 10 == 3 || amount % 10 == 4 && !(amount >= 11 && amount < 20)) cout << "ки";
            else cout << "ок";
            setColor(WHITE);

            setPosition(11, 3);
            setColor(GRAY);
            _setmode(_fileno(stdout), _O_U16TEXT);
            wcout << (wchar_t) 0x25B2;
            _setmode(_fileno(stdout), _O_TEXT);
            setColor(WHITE);
            setPosition(8, 4);
            for(int i = 0; i < linesShowed && i < suitableAttempts.size(); ++i) {
                writeAttemptLine(suitableAttempts[i], attemptNo[i] + 1);
                cout << "\n\t";
            }
            if(suitableAttempts.size() <= linesShowed) {
                setPosition(8, 4 + suitableAttempts.size());
                for(int i = suitableAttempts.size(); i < linesShowed; ++i) {
                    cout << string(65, ' ') << "\n\t";
                }
                setPosition(11, 3 + linesShowed + 1);
                setColor(GRAY);
                _setmode(_fileno(stdout), _O_U16TEXT);
                wcout << (wchar_t) 0x25BC;
                _setmode(_fileno(stdout), _O_TEXT);
                setColor(WHITE);
            }
            else {
                setPosition(11, 3 + linesShowed + 1);
                setColor(WHITE);
                _setmode(_fileno(stdout), _O_U16TEXT);
                wcout << (wchar_t) 0x25BC;
                _setmode(_fileno(stdout), _O_TEXT);
            }
            int starter = 0;
            while(1) {
                choice = _getch();
                if(choice == -32 && kbhit()) choice = _getch();

                if(choice == 'q' || choice == -87 || choice == 'l' || choice == -92) {
                    setPosition(8, 2);
                    cout << string(32, ' ');
                    break;
                }
                if(choice == 'n' || choice == -30 || choice == 'j' || choice == -82 || choice == 80) {  // down
                    if(suitableAttempts.size() - starter <= linesShowed) continue;
                    starter++;
                }
                else if(choice == 'N' || choice == -110 || choice == 'k' || choice == -85 || choice == 72) {  // up
                    if(starter == 0) continue;
                    starter--;
                }
                else if(choice == -111 || choice == 10) {  // ctrl + down, ctrl + j
                    if(suitableAttempts.size() - starter <= linesShowed) continue;
                    starter = max(0, (int) suitableAttempts.size() - linesShowed);
                }
                else if(choice == -115 || choice == 11) {  // ctrl + up, ctrl + k
                    if(starter == 0) continue;
                    starter = 0;
                }

                if(choice == '1' || choice == '5' || choice == '2' || choice == '4' || choice == '0' ||
                   choice == 'h' || choice == -32) {
                    buffer = choice;
                    return 0;
                }
                if(choice == 27) {
                    saveNewData();
                    exit(0);
                }
                if(choice == 13) return 1;

                setPosition(11, 3);
                if(!starter) setColor(GRAY);
                _setmode(_fileno(stdout), _O_U16TEXT);
                wcout << (wchar_t) 0x25B2;
                _setmode(_fileno(stdout), _O_TEXT);
                setColor(WHITE);
                for(int i = starter; i < starter + linesShowed && i < suitableAttempts.size(); ++i) {
                    cout << "\n\t";
                    writeAttemptLine(suitableAttempts[i], attemptNo[i] + 1);
                }
                setPosition(11, 3 + linesShowed + 1);
                if(suitableAttempts.size() - starter <= linesShowed) setColor(GRAY);
                _setmode(_fileno(stdout), _O_U16TEXT);
                wcout << (wchar_t) 0x25BC;
                _setmode(_fileno(stdout), _O_TEXT);
                setColor(WHITE);
            }
        }

        if(choice == '1' || choice == '5' || choice == '2' || choice == '4' || choice == '0' ||
           choice == 'h' || choice == -32) {
            buffer = choice;
            return 0;
        }
        if(choice == 27) {
            saveNewData();
            exit(0);
        }
        if(choice == 13) return 1;

        // output
        normalOutput:
        setPosition(11, 3);
        if(!startFrom) setColor(GRAY);
        _setmode(_fileno(stdout), _O_U16TEXT);
        wcout << (wchar_t) 0x25B2;
        _setmode(_fileno(stdout), _O_TEXT);
        setColor(WHITE);
        for(int i = startFrom; i < startFrom + linesShowed && i < lastAttempts.size(); ++i) {
            cout << "\n\t";
            writeAttemptLine(lastAttempts[i], i + 1);
        }
        setPosition(11, 3 + linesShowed + 1);
        if(lastAttempts.size() - startFrom <= linesShowed) setColor(GRAY);
        _setmode(_fileno(stdout), _O_U16TEXT);
        wcout << (wchar_t) 0x25BC;
        _setmode(_fileno(stdout), _O_TEXT);
        setColor(WHITE);
    }
}

void RecordsObserver::writeAttemptLine(Attempt attempt, int no)
{
    cout << no;
    if     (no < 10)  cout << ".   ";
    else if(no < 100) cout << ".  ";
    else              cout << ". ";
    cout << attempt.num1 << " x " << attempt.num2 << " = ";
    setColor(GREEN);
    cout << setw(10) << prettifyAnswer(attempt.num1 * attempt.num2);
    setColor(WHITE);
    cout << "  ->  " << setw(5) << timeFormat(attempt.solutionTime, 1);
    time_t timer = attempt.timePoint;
    tm* time_ptr = localtime(&timer);
    setColor(SEA_WAVE);
    char buf[20];
    if(strftime(buf, sizeof(buf), "%d.%m.%y  %H:%M", time_ptr)) {
        cout << setw(19) << buf;
    }
    setColor(WHITE);
}

void RecordsObserver::writeRecordsInConsole()
{
    system("cls");
    setPosition(49, 1);
    setColor(ORANGE);
    cout << "Решено за все время: " << solvingsAmount;
    setColor(WHITE);
    setPosition(49, 3); cout << "Текущие решения";
    setPosition(49, 4);
    cout << "Среднее из 5: ";
    setColor(BEIGE);
    cout << ((solvingsAmount >= 5) ? timeFormat((double) totalTimeAmount_last5 / last5.size()) : "...");
    setColor(WHITE);
    setPosition(49, 5);
    cout << "Среднее из 20: ";
    setColor(BEIGE);
    cout << ((solvingsAmount >= 20) ? timeFormat((double) totalTimeAmount_last20 / last20.size()) : "...");
    setColor(WHITE);
    setPosition(49, 6);
    cout << "Среднее из 50: ";
    setColor(BEIGE);
    cout << ((solvingsAmount >= 50) ? timeFormat((double) totalTimeAmount_last50 / last50.size()) : "...");
    setColor(WHITE);
    setPosition(49, 7);
    cout << "Среднее из 100: ";
    setColor(BEIGE);
    cout << ((solvingsAmount >= 100) ? timeFormat((double) totalTimeAmount_last100 / last100.size()) : "...");

    setColor(ORANGE);
    setPosition(5, 1); cout << "Рекорды";
    setColor(WHITE);
    setPosition(5, 3);
    cout << "Best attempt: ";
    if(newRecord_single) setColor(GREEN);
    else setColor(BEIGE);
    cout << ((bestOne.solutionTime) ? to_string(bestOne.solutionTime) : "...");
    if(newRecord_single && ~differenceWithPrev_single) diffFormat(differenceWithPrev_single);
    setColor(WHITE);
    setPosition(5, 4);
    cout << "Avg 5: ";
    if(newRecord_avg5) setColor(GREEN);
    else setColor(BEIGE);
    cout << ((bestAvg5) ? timeFormat(bestAvg5) : "...");
    if(newRecord_avg5 && differenceWithPrev_bestAvg5 != -1) diffFormat(differenceWithPrev_bestAvg5);
    setColor(WHITE);
    setPosition(5, 5);
    cout << "Avg 20: ";
    if(newRecord_avg20) setColor(GREEN);
    else setColor(BEIGE);
    cout << ((bestAvg20) ? timeFormat(bestAvg20) : "...");
    if(newRecord_avg20 && differenceWithPrev_bestAvg20 != -1) diffFormat(differenceWithPrev_bestAvg20);
    setColor(WHITE);
    setPosition(5, 6);
    cout << "Avg 50: ";
    if(newRecord_avg50) setColor(GREEN);
    else setColor(BEIGE);
    cout << ((bestAvg50) ? timeFormat(bestAvg50) : "...");
    if(newRecord_avg50 && differenceWithPrev_bestAvg50 != -1) diffFormat(differenceWithPrev_bestAvg50);
    setColor(WHITE);
    setPosition(5, 7);
    cout << "Avg 100: ";
    if(newRecord_avg100) setColor(GREEN);
    else setColor(BEIGE);
    cout << ((bestAvg100) ? timeFormat(bestAvg100) : "...");
    if(newRecord_avg100 && differenceWithPrev_bestAvg100 != -1) diffFormat(differenceWithPrev_bestAvg100);
    setColor(WHITE);

    setColor(238); // бежевые столбики
    for(int i = 1; i <= 7; ++i) {
        setPosition(46, i);
        cout << "|";
    }
    for(int i = 1; i <= 7; ++i) {
        setPosition(29, i);
        cout << "|";
    }
    setColor(WHITE);
}

void RecordsObserver::refreshRecordsInConsole()
{
    setPosition(70, 1);
    setColor(ORANGE);
    cout << solvingsAmount << " ";

    if(bestOne.solutionTime) {
        setPosition(19, 3);
        cout << "          "; // 10 spaces
        setPosition(19, 3);
        if(newRecord_single) setColor(GREEN);
        else setColor(BEIGE);
        cout << bestOne.solutionTime;
        if(newRecord_single && ~differenceWithPrev_single) diffFormat(differenceWithPrev_single);
    }
    if(bestAvg5) {
        setPosition(12, 4);
        cout << "                 "; // 17 spaces
        setPosition(12, 4);
        if(newRecord_avg5) setColor(GREEN);
        else setColor(BEIGE);
        cout << timeFormat(bestAvg5);
        if(newRecord_avg5 && differenceWithPrev_bestAvg5 != -1) diffFormat(differenceWithPrev_bestAvg5);
    }
    if(bestAvg20) {
        setPosition(13, 5);
        cout << "                "; // 16 spaces
        setPosition(13, 5);
        if(newRecord_avg20) setColor(GREEN);
        else setColor(BEIGE);
        cout << timeFormat(bestAvg20);
        if(newRecord_avg20 && differenceWithPrev_bestAvg20 != -1) diffFormat(differenceWithPrev_bestAvg20);
    }
    if(bestAvg50) {
        setPosition(13, 6);
        cout << "                "; // 16 spaces
        setPosition(13, 6);
        if(newRecord_avg50) setColor(GREEN);
        else setColor(BEIGE);
        cout << timeFormat(bestAvg50);
        if(newRecord_avg50 && differenceWithPrev_bestAvg50 != -1) diffFormat(differenceWithPrev_bestAvg50);
    }
    if(bestAvg100) {
        setPosition(14, 7);
        cout << "               "; // 15 spaces
        setPosition(14, 7);
        if(newRecord_avg100) setColor(GREEN);
        else setColor(BEIGE);
        cout << timeFormat(bestAvg100);
        if(newRecord_avg100 && differenceWithPrev_bestAvg100 != -1) diffFormat(differenceWithPrev_bestAvg100);
    }

    if(last5.size() == 5) {
        setPosition(63, 4);
        setColor(BEIGE);
        cout << timeFormat((double) totalTimeAmount_last5 / 5, 1);
    }
    if(last20.size() == 20) {
        setPosition(64, 5);
        setColor(BEIGE);
        cout << timeFormat((double) totalTimeAmount_last20 / 20, 1);
    }
    if(last50.size() == 50) {
        setPosition(64, 6);
        setColor(BEIGE);
        cout << timeFormat((double) totalTimeAmount_last50 / 50, 1);
    }
    if(last100.size() == 100) {
        setPosition(65, 7);
        setColor(BEIGE);
        cout << timeFormat((double) totalTimeAmount_last100 / 100, 1);
    }

    setColor(WHITE);
}

string RecordsObserver::timeFormat(double dTime, bool addSpaces)
{
    dTime = round(dTime * 100) / 100;
    int mins = dTime / 60;
    double secs = dTime - 60 * mins;

    ostringstream oss;
    if(mins) {
        oss << mins << ":";
        if(floor(secs) < 10) oss << "0";
    }
    oss << secs;

    if(addSpaces) {
        int stringSize = oss.str().size();
        if(stringSize <= 7) oss << string(7 - stringSize, ' ');
    }

    return oss.str();
}

template<typename T>
void RecordsObserver::diffFormat(T diff)
{
    string sDiff;
    if(typeid(T) == typeid(int)) sDiff = to_string(diff);
    else sDiff = timeFormat(diff);
    string arrow(1, 25);
    setColor(DARK_GREEN);
    cout << " (" + arrow + " " + sDiff + ")";
}

string RecordsObserver::prettifyAnswer(int answer)
{
    string pretty = to_string(answer / 1000000) + " ";
    int thousands = (answer % 1000000) / 1000;
    if(thousands < 10) pretty += "00";
    else if(thousands < 100) pretty += "0";
    pretty += to_string(thousands) + " ";
    int units = answer % 1000;
    if(units < 10) pretty += "00";
    else if(units < 100) pretty += "0";
    pretty += to_string(units);
    return pretty;
}

istream& operator>>(istream& stream, Attempt& attempt)
{
    stream >> attempt.solutionTime >> attempt.num1 >> attempt.num2 >> attempt.timePoint;
    return stream;
}

ostream& operator<<(ostream& stream, Attempt& attempt)
{
    stream << attempt.solutionTime << ' ' << attempt.num1 << ' ' << attempt.num2 << ' ' << attempt.timePoint << '\n';
    return stream;
}

void RecordsObserver::setPosition(int x, int y)
{
    static COORD coord;
    coord.X = x;
    coord.Y = y;
    if(!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord)) {
        cout << GetLastError();
        exit(-1);
    }
}

void RecordsObserver::setColor(int color)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD) color);
}
