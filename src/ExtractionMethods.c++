#include <ExtractionMethods.h++>

#include <istream>
#include <sstream>
#include <string>


bool const extractToKeyword ( std::istream &istream , std::string const &keyword ) {
    std::stringstream line;
    std::string temp;
    do {
        std::getline ( istream , temp );
        line = std::stringstream ( temp );
        line >> temp;
    } while (temp != keyword && !istream.eof ( ) );
    return !istream.eof ( );
}

static constexpr char const *const keywords[] = {
    "begin",    // begins a declaration
    "end",      // ends a declaration
    "ref",      // marks a reference
    "#",        // marks a comment
    "req",      // marks a requisite, can be modified by the flags "pre" and "con"
    "reqs",     // marks a reference to a requisite group
    "hours",    // indicates the number of hours in a course
    "name",     // indicates that the rest of the line contains the name of the course. If multiple name keywords are present,
                // the contents are concatinated in the order seen.
    "desc",     // indicates that the rest of the line contains the description of the course. If multiple desc keywords are
                // present, the contents are concatinated in the order seen
    "semester", // a semester in a plan. Followed by a reference to the semester then a list of course references finishing the line
};

std::vector<std::string> getKeywords()
{
    std::vector<std::string> result;
    for ( std::unsigned_integral auto i = 0U; i < sizeof(keywords) / sizeof(char *); i++)
    {
        result.push_back(keywords[i]);
    }
    result.shrink_to_fit();
    return result;
}

void interpretComment(std::istream &, Serial &, bool &) { return; }
void interpretEnd(std::istream &, Serial &, bool &endFlag)
{
    endFlag = true;
}

void extract(std::vector<Keyword> usedKeywords, std::istream &inputstream, Serial &serial)
{
    std::stringstream line;
    std::string firstWord;
    do
    {
        std::getline(inputstream, firstWord);
        line = std::stringstream(firstWord);
        line >> firstWord;
    } while (firstWord != usedKeywords[0].word);
    bool end = false;
    usedKeywords[0].action(line, serial, end);
    while (!end)
    {
        std::getline(inputstream, firstWord);
        line = std::stringstream(firstWord);
        line >> firstWord;
        for (Keyword keyword : usedKeywords)
        {
            if (firstWord == keyword.word)
            {
                keyword.action(line, serial, end);
                break;
            }
        }
    }
}