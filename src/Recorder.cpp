#include "Recorder.hpp"
#include <fstream>
#include <time.h>
#include <chrono>
#include <string>
#include "Log.hpp"

namespace aic {

aic::Recorder::Recorder()
{
    outfile.open("game.pgn");
    aic::Log(aic::DBG) << "Opened file for writing.";
}

void aic::Recorder::writeTagsInteractive()
{
    char data[100];
    Log(DBG) << "Please enter event name: ";
    std::cin >> data;
    outfile << "[Event \""
            << data
            << "\"]" << std::endl;

    Log(DBG) << "Please enter city name:";
    std::cin >> data;
    outfile << "[Site \"" << data << ", ";
    Log(DBG) << "Please enter region name:";
    std::cin >> data;
    outfile << data << " ";
    Log(DBG) << "Please center country code:";
    std::cin >> data;
    outfile << data << "\"]" << std::endl;

    time_t t = time(NULL);
    tm* timePtr = localtime(&t);

    int year = (timePtr->tm_year)+1900;
    int month = (timePtr->tm_mon)+1;
    int day = (timePtr->tm_mday);

    outfile << "[Date \"" << year << "." << month << ".";
    if (day > 9) outfile << day; else outfile << "0" << day;
    outfile << "\"]" << std::endl;

    outfile << "[Round \"29\"]" << std::endl;

    Log(DBG) << "Please enter WHITE player's name in format: Surname, Name";
    std::cin >> data;
    outfile << "[White \"" << data << "\"]" << std::endl;

    Log(DBG) << "Please enter BLACK player's name in format: Surname, Name";
    std::cin >> data;
    outfile << "[Black \"" << data << "\"]" << std::endl;

    Log(DBG) << "Please enter round result:";
    std::cin >> data;
    outfile << "[Result \"" << data << "\"]" << std::endl;

    outfile << std::endl << std::endl;
}

std::string aic::Recorder::getMoveText()
{
    return moveText;
}

aic::Recorder::~Recorder()
{
    outfile.close();
    aic::Log(aic::DBG) << "File closed";
}

}
