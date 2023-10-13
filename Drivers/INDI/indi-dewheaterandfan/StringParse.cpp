#include "StringParse.h"

#include <defaultdevice.h>

StringParse::StringParse()
{
//    inputString = input;
    delim = ","; // CSV
}

StringParse::StringParse(std::string input)
{
    inputString = input;
    delim = ","; // CSV
}

StringParse::StringParse(std::string input, std::string delimiter)
{
    inputString = input;
    delim = delimiter;
}

StringParse::~StringParse()
{
}

void StringParse::SetInputString(std::string newString)
{
	inputString = newString;
}

void StringParse::SetDelimiter(std::string delimiter)
{
    delim = delimiter;
}

std::vector<std::string> StringParse::split()
{
    size_t pos_start = 0, pos_end, delim_len = delim.length();
    std::string token;
    std::vector<std::string> res;
    
    counter = 0;
    while ((pos_end = inputString.find(delim, pos_start)) != std::string::npos)
    {
        counter++;
        token = inputString.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (inputString.substr (pos_start));
    return res;
}


std::string StringParse::GetElement(int index)
{
    vector = split();
    if( counter == 0 )
    {
		std::string res = "NODELIMITER";
		return res;
//        return "Error, no delimiter found!";
    }
    if( index > counter || index < 0)
    {
		std::string res = "OUTOFRANGE";
		return res;
//        return "Error, index out of range!";
    }
    return vector[index];
}

