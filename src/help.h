#pragma once
extern int nConsoleWidth, nConsoleHeight;

int pageWidth, columnWidth, columnHeight;
enum Type {KEY_VALUE, TEXT, UNSPECIFIED} type;

const int RIGHT_INDENT_FOR_PAGE_NUMBER = 8;
const int SIDE_INDENT_FOR_MANUAL_TEXT = 8;
const int SPACE_BETWEEN_COLUMNS = 4;

struct Paragraph
{
    string action, outcome, dash;
    Position position;
    vector<string> lines;

    Paragraph(string action_ = "", string outcome_ = "", string dash_ = "-");
    void Print(Type type);
};

struct Page
{
    string heading;
    vector<Paragraph> paragraphs;
    Type type;

    Page() : heading(""), type(UNSPECIFIED), paragraphs({}) {}
    Page(string heading_, Type type_);
    void AddParagraph(Paragraph paragraph) { paragraphs.push_back(paragraph); }
};

class Help
{
    vector<Page> pages;
    int currentPageNumber;
    map<string, vector<int>> transferableWords;

    void ShowPage(int pageNum);
    int CheckValidity();
    int FindBiggestNumber(vector<int>& arr, int lessThan, int moreThan = 0);
    void ConsiderTransferableWords(vector<string>& words);
public:
    Help();
    bool ScrollingPages(char& buffer);
};
