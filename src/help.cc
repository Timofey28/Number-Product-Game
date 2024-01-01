#include "help.h"
#include <cassert>
#include <conio.h>
using namespace std;

void setPosition(const Position& position);

Help::Help()
{
    assert(SPACE_BETWEEN_COLUMNS % 2 == 0);
    currentPageNumber = 1;
    pageWidth = nConsoleWidth - SIDE_INDENT_FOR_MANUAL_TEXT * 2;
    columnWidth = (nConsoleWidth - SIDE_INDENT_FOR_MANUAL_TEXT * 2 - SPACE_BETWEEN_COLUMNS) / 2,
    columnHeight = nConsoleHeight - 4;
    pages = {};
    Page page;

    ////////////////// Добавление руководства здесь ///////////////////
    vector<string> words_transfers = {
        "за-счи-ты-ва-ет-ся",
        "сле-ду-ю-щий",
        "ре-ше-ни-я",
        "при-ме-ров",
        "вклю-чи-тель-но",
        "",
    };

    page = Page("Общее руководство", TEXT);
    page.AddParagraph(Paragraph("Данная инструкция представляет из себя набор разделов, между которыми \
можно перемещаться с помощью стрелок влево/вправо, а также с помощью клавиш [j, k]. Для возвращения к \
решению примеров, нажмите клавишу Enter. Для выхода из программы из любого места необходимо нажать клавишу Esc, \
либо вручную закрыть окно приложения."));
    pages.push_back(page);

    page = Page("Во время решения примера", KEY_VALUE);
    page.AddParagraph(Paragraph("Backspace", "стереть последнюю введенную цифру"));
    page.AddParagraph(Paragraph("Пробел", "пауза/пуск, при остановке время не считается"));
    page.AddParagraph(Paragraph("n (next)", "следующий пример"));
    pages.push_back(page);

    page = Page("После решения примера", KEY_VALUE);
    page.AddParagraph(Paragraph("1", "лучший результат"));
    page.AddParagraph(Paragraph("5", "лучшее среднее из 5"));
    page.AddParagraph(Paragraph("2", "лучшее среднее из 20"));
    page.AddParagraph(Paragraph("4", "лучшее среднее из 50"));
    page.AddParagraph(Paragraph("0", "лучшее среднее из 100"));
    page.AddParagraph(Paragraph("l (last)", "последние 100 попыток"));
    page.AddParagraph(Paragraph("d (delete)", "не засчитывать последнюю попытку, если она была неудачной"));
    page.AddParagraph(Paragraph("Любая другая клавиша", "следующий пример", "=="));
    pages.push_back(page);

    page = Page("Перемещение между рекордами", KEY_VALUE);
    page.AddParagraph(Paragraph("n", "к следующему рекорду"));
    page.AddParagraph(Paragraph("N", "к предыдущему рекорду"));
    pages.push_back(page);

    page = Page("Перемещение по списку", KEY_VALUE);
    page.AddParagraph(Paragraph("j, n", "вниз"));
    page.AddParagraph(Paragraph("k, N", "вверх"));
    page.AddParagraph(Paragraph("Ctrl+j, Ctrl+down", "до конца вниз"));
    page.AddParagraph(Paragraph("Ctrl+k, Ctrl+up", "до начала вверх"));
    page.AddParagraph(Paragraph("Enter", "возвращение в главный режим"));
    pages.push_back(page);

    page = Page("Последние 100 попыток (часть I)", KEY_VALUE);
    page.AddParagraph(Paragraph("/moda", "наиболее часто встречающееся время решения"));
    page.AddParagraph(Paragraph("/=20", "показать все 20-секундные попытки"));
    page.AddParagraph(Paragraph("/<20", "попытки, меньшие 20 секунд"));
    page.AddParagraph(Paragraph("/<=20", "попытки, меньшие или равные 20 секунд"));
    page.AddParagraph(Paragraph("/>20", "попытки, большие 20 секунд"));
    page.AddParagraph(Paragraph("/>=20", "попытки, большие или равные 20 секунд"));
    page.AddParagraph(Paragraph("[q, l]", "вернуться обратно"));
    pages.push_back(page);

    page = Page("Последние 100 попыток (часть II)", KEY_VALUE);
    page.AddParagraph(Paragraph("/30..40", "попытки, решенные в интервале от 30 до 40 секунд включительно"));
    page.AddParagraph(Paragraph("/..30", "попытки, решенные до 30 секунд включительно"));
    page.AddParagraph(Paragraph("/30..", "попытки, решенные от 30 секунд и выше"));
    page.AddParagraph(Paragraph("[q, l]", "вернуться обратно"));
    pages.push_back(page);
    ///////////////////////////////////////////////////////////////////

    ConsiderTransferableWords(words_transfers);
    int result = CheckValidity();
    if(result) {
        cout << "Слишком много текста или слишком длинные слова на странице\n";
        cout << result << ": \"" << pages[result - 1].heading << "\"\n";
        exit(0);
    }
}

bool Help::ScrollingPages(char& buffer)
{
    ShowPage(currentPageNumber);
    char choice;
    while(1) {
        choice = _getch();
        if(choice == -32 && kbhit()) choice = _getch();
        if(choice == '1' || choice == '5' || choice == '2' || choice == '4' ||
           choice == '0' || (choice == 'l' || choice == -92)) {
            buffer = choice;
            return 0;
        }
        if(choice == 13) return 0;
        if(choice == 27) return 1;

        if(pages.size() == 1) continue;
        if(choice == 77 || choice == 'k' || choice == -85) {  // arrow right || l
            currentPageNumber++;
            if(currentPageNumber > pages.size()) currentPageNumber = 1;
            ShowPage(currentPageNumber);
        }
        else if(choice == 75 || choice == 'j' || choice == -82) {  // arrow left || h
            currentPageNumber--;
            if(!currentPageNumber) currentPageNumber = pages.size();
            ShowPage(currentPageNumber);
        }
    }
}

void Help::ShowPage(int pageNum)
{
    assert(pageNum >= 1 && pageNum <= pages.size());
    system("cls");
    setColor(DARK_GREEN);
    cout << "\n\t" << pages[pageNum - 1].heading;
    setColor(HELP);
    cout << "\n\t" << string(nConsoleWidth - SIDE_INDENT_FOR_MANUAL_TEXT * 2, '-');

    for(auto& paragraph : pages[pageNum - 1].paragraphs) {
        paragraph.Print(pages[pageNum - 1].type);
    }

    setPosition(78 - (7 + RIGHT_INDENT_FOR_PAGE_NUMBER), 1);
    setColor(SEA_WAVE);
    cout << setw(7) << to_string(pageNum) + " / " + to_string(pages.size());
    setColor(WHITE);
}

int Help::CheckValidity()
{
    int counter = 1, where, from, cursorRow, cursorLinePos;
    string word, line = "";
    Position pos;

    for(auto& page : pages) {
        if(page.heading.size() > nConsoleWidth - 8 - RIGHT_INDENT_FOR_PAGE_NUMBER - 7 - 8) return counter;

        // вычисляем все строки + сохраняем в объект Paragraph
        if(page.type == TEXT) {
            for(auto& paragraph : page.paragraphs) {
                cursorRow = 0;
                cursorLinePos = 0;
                paragraph.lines = {};
                for(int l = 0, r = 0; r < paragraph.action.size();) {
                    where = 0;
                    l = (r == 0) ? 0 : r + 1;
                    r = paragraph.action.find(' ', l);
                    if(r == string::npos) r = paragraph.action.size();
                    word = paragraph.action.substr(l, r - l);
                    if(word.size() > pageWidth && transferableWords.find(word) == transferableWords.end()) return counter;

                    if(cursorLinePos + word.size() <= pageWidth) {
                        cursorLinePos += word.size() + 1;
                        if(line != "") line += " ";
                        line += word;
                    }
                    else {
                        if(cursorLinePos) {
                            if(transferableWords.find(word) != transferableWords.end() &&
                               FindBiggestNumber(transferableWords[word], pageWidth - cursorLinePos - 1))
                            {
                                where = FindBiggestNumber(transferableWords[word], pageWidth - cursorLinePos - 1);
                                line += " " + word.substr(0, where) + "-";
                                paragraph.lines.push_back(line);
                            }
                            else {
                                cursorRow++;
                                cursorLinePos = 0;
                                paragraph.lines.push_back(line);
                                line = "";
                            }
                        }
                        while(word.size() - where > pageWidth) {
                            from = where;
                            where = FindBiggestNumber(transferableWords[word], pageWidth + where);
                            line = word.substr(from, where - from) + "-";
                            paragraph.lines.push_back(line);
                            line = "";
                        }
                        cursorLinePos = word.size() - where;
                        line = word.substr(where);
                    }
                }
                if(line.size()) paragraph.lines.push_back(line);
            }
        }
        else if(page.type == KEY_VALUE) {
            for(auto& paragraph : page.paragraphs) {
                cursorRow = 0;
                cursorLinePos = paragraph.action.size() + paragraph.dash.size() + 2;
                paragraph.lines = {};
                line = " " + paragraph.dash;
                if(cursorLinePos - 1 > columnWidth) return counter;

                for(int l = 0, r = 0; r < paragraph.outcome.size();) {
                    where = 0;
                    l = (r == 0) ? 0 : r + 1;
                    r = paragraph.outcome.find(' ', l);
                    if(r == string::npos) r = paragraph.outcome.size();
                    word = paragraph.outcome.substr(l, r - l);
                    if(word.size() > columnWidth && transferableWords.find(word) == transferableWords.end()) return counter;
                    if(cursorLinePos >= columnWidth) {
                        cursorRow++;
                        cursorLinePos = 0;
                        paragraph.lines.push_back(line);
                        line = "";
                    }

                    if(cursorLinePos + word.size() <= columnWidth) {
                        cursorLinePos += word.size() + 1;
                        if(line != "") line += " ";
                        line += word;
                    }
                    else {
                        if(cursorLinePos) {
                            if(transferableWords.find(word) != transferableWords.end() &&
                               FindBiggestNumber(transferableWords[word], columnWidth - cursorLinePos))
                            {
                                where = FindBiggestNumber(transferableWords[word], columnWidth - cursorLinePos);
                                line += " " + word.substr(0, where) + "-";
                                paragraph.lines.push_back(line);
                            }
                            else {
                                cursorRow++;
                                cursorLinePos = 0;
                                paragraph.lines.push_back(line);
                                line = "";
                            }
                        }
                        while(word.size() - where > columnWidth) {
                            from = where;
                            where = FindBiggestNumber(transferableWords[word], columnWidth + where);
                            line = word.substr(from, where - from) + "-";
                            paragraph.lines.push_back(line);
                            line = "";
                        }
                        cursorLinePos = word.size() - where;
                        line = word.substr(where);
                    }
                }
                if(line.size()) paragraph.lines.push_back(line);
            }
        }

        // присваиваем каждому абзацу координаты вывода
        pos.x = SIDE_INDENT_FOR_MANUAL_TEXT;
        pos.y = 3;
        for(auto& paragraph : page.paragraphs) {
            if(pos.y + paragraph.lines.size() - 1 >= columnHeight + 3) {
                if(page.type == TEXT) return counter;
                else if(page.type == KEY_VALUE) {
                    if(pos.x == SIDE_INDENT_FOR_MANUAL_TEXT) {
                        pos.x += columnWidth + SPACE_BETWEEN_COLUMNS;
                        pos.y = 3;
                    }
                    else return counter;
                }
            }
            paragraph.position = pos;
            pos.y += paragraph.lines.size();
        }

        counter++;
    }

    return 0;
}

void Help::ConsiderTransferableWords(vector<string>& words)
{
    if(!words.size()) return;
    for(const auto& word : words) {
        if(word == "") continue;
        assert(word.find('-') != string::npos);
    }

    int index;
    for(auto& word : words) {
        if(word == "") continue;
        while(1) {
            index = word.find('-');
            if(index == string::npos) {
                assert(word.size() - transferableWords["."].back() <= columnWidth);
                break;
            }
            assert(transferableWords["."].size() ? index - transferableWords["."].back() < columnWidth : index < columnWidth);
            transferableWords["."].push_back(index);
            word.erase(index, 1);
        }
        transferableWords[word] = transferableWords["."];
        transferableWords[word + ","] = transferableWords["."];
        transferableWords["."].clear();
    }
    transferableWords.erase(transferableWords.find("."));
}

int Help::FindBiggestNumber(vector<int>& arr, int lessThan, int moreThan)
{
    for(int i = arr.size() - 1; ~i; --i) {
        if(arr[i] < lessThan && arr[i] > moreThan) return arr[i];
    }
    return 0;
}

Page::Page(string heading_, Type type_)
{
    heading = heading_;
    type = type_;
    paragraphs = {};

    assert(type != UNSPECIFIED);
}

Paragraph::Paragraph(string action_, string outcome_, string dash_)
{
    assert(action_ != "");
    action = action_;
    outcome = outcome_;
    dash = dash_;
    position = Position();
}

void Paragraph::Print(Type type)
{
    assert(type != UNSPECIFIED);
    Position pos(position);
    if(type == KEY_VALUE) {
        setPosition(pos);
        setColor(RED);
        cout << action;
        setColor(HELP);
        cout << lines[0];
        for(int i = 1; i < lines.size(); ++i) {
            pos.y++;
            setPosition(pos);
            cout << lines[i];
        }
    }
    else if(type == TEXT) {
        setColor(HELP);
        for(int i = 0; i < lines.size(); ++i) {
            setPosition(pos);
            cout << lines[i];
            pos.y++;
        }
    }
}
