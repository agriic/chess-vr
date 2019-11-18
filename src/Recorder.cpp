#include "Recorder.h"
#include <fstream>
#include <time.h>
#include <chrono>
#include <string>
#include "Log.hpp"

Recorder::Recorder()
{
    outfile.open("game.pgn");
    aic::Log(aic::DBG) << "Opened file for writing.";
}

void Recorder::writeTagsInteractive()
{
    char data[100];
    std::cout << "Please enter event name: " << std::endl;
    std::cin >> data;
    outfile << "[Event \""
            << data
            << "\"]" << std::endl;

    std::cout << "Please enter city name:" << std::endl;
    std::cin >> data;
    outfile << "[Site \"" << data << ", ";
    std::cout << "Please enter region name:" << std::endl;
    std::cin >> data;
    outfile << data << " ";
    std::cout << "Please center country code:" << std::endl;
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

    std::cout << "Please enter WHITE player's name in format: Surname, Name" << std::endl;
    std::cin >> data;
    outfile << "[White \"" << data << "\"]" << std::endl;

    std::cout << "Please enter BLACK player's name in format: Surname, Name" << std::endl;
    std::cin >> data;
    outfile << "[Black \"" << data << "\"]" << std::endl;

    std::cout << "Please enter round result:" << std::endl;
    std::cin >> data;
    outfile << "[Result \"" << data << "\"]" << std::endl;

    outfile << std::endl << std::endl;
}

std::string Recorder::getMoveText()
{
    return moveText;
}

Recorder::~Recorder()
{
    outfile.close();
    aic::Log(aic::DBG) << "File closed";
}
