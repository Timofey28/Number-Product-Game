#ifndef RECORDSOBSERVER_H
#define RECORDSOBSERVER_H


struct Attempt
{
    int solutionTime;
    int num1, num2;
    time_t timePoint;
    friend std::istream& operator>>(std::istream& stream, Attempt& attempt);
    friend std::ostream& operator<<(std::ostream& stream, Attempt& attempt);
};

class RecordsObserver
{
    int solvingsAmount;
    std::array<Attempt, 5> best5;
    std::array<Attempt, 20> best20;
    std::array<Attempt, 50> best50;
    std::array<Attempt, 100> best100;
    double bestAvg5, bestAvg20, bestAvg50, bestAvg100;
    int differenceWithPrev_single;
    double differenceWithPrev_bestAvg5, differenceWithPrev_bestAvg20, differenceWithPrev_bestAvg50, differenceWithPrev_bestAvg100;
    Attempt bestOne;
    std::queue<Attempt> last5, last20, last50, last100;
    int totalTimeAmount_last5, totalTimeAmount_last20, totalTimeAmount_last50, totalTimeAmount_last100;
    bool newRecord_avg5, newRecord_avg20, newRecord_avg50, newRecord_avg100, newRecord_single;
    enum colors {RED = 4, ORANGE = 6, GRAY = 8, GREEN = 10, DARK_GREEN = 2, SEA_WAVE = 11, BEIGE = 14, WHITE = 15} color;

    void refreshBests();
    void setPosition(int x, int y);
    void setColor(int color);
    std::string timeFormat(double dTime, bool addSpaces = 0);
    std::string prettifyAnswer(int answer);
    template<typename T> void diffFormat(T diff);
    void saveNewData();
public:
    RecordsObserver();
    ~RecordsObserver() {saveNewData();}
    void addAttempt(int num1, int num2, int solutionTime);
    void writeRecordsInConsole();
    void refreshRecordsInConsole();
    bool showSingle(char& buffer);
    bool showBest5(char& buffer);
    bool showBest20(char& buffer);
    bool showBest50(char& buffer);
    bool showBest100(char& buffer);
    bool showLast(char& buffer);
    bool has_bestAvg5() {return bestAvg5;}
    bool has_bestAvg20() {return bestAvg20;}
    bool has_bestAvg50() {return bestAvg50;}
    bool has_bestAvg100() {return bestAvg100;}
    bool has_single() {return bestOne.solutionTime;}
};

#endif // RECORDSOBSERVER_H
