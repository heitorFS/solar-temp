#ifndef INCLUDE_VALIDATIONS_
#define INCLUDE_VALIDATIONS_

#include <string>

using namespace std;

bool validate_letters(string& txt)
{
    size_t length = txt.length();
    for (int i = 0; i < length; ++i)
    {
        if (
            (txt[i] < 65 ||
            (txt[i] > 90 && txt[i] < 97) ||
            (txt[i] > 122 && txt[i] < 192)) && txt[i] != 32 && txt[i] != 39 && txt[i] != 45 && txt[i] != -61 && txt[i - 1] != -61
        )
            return false;
    }

    return true;
}

bool validate_numbers(string& txt)
{
    size_t length = txt.length();
    for (int i = 0; i < length; ++i)
    {
        if (
            txt[i] < 48 ||
            txt[i] > 57
        )
            return false;
    }

    return true;
}

bool validate_email(string& txt)
{
    size_t length = txt.length();
    size_t atLoc = length - 1;

    for (; txt[atLoc] != '@'; --atLoc);

    string str = txt.substr(0, atLoc);

    if (str[0] == '.' || str[str.length() - 1] == '.')
        return false;

    for (int i = 0; i < length; ++i)
    {
        if (
            (str[i] == '(' ||
            str[i] == ')' ||
            str[i] == '<' ||
            str[i] == '>' ||
            str[i] == ',' ||
            str[i] == ';' ||
            str[i] == ':' ||
            str[i] == '\\' ||            
            str[i] == '[' ||
            str[i] == ']' ||
            str[i] == '@') && (str[i - 1] != '"' || str[i + 1] != '"')
        )
            return false;
    }

    return true;
}

#endif // INCLUDE_VALIDATIONS_