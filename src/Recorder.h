#include <string>
#include <fstream>

#ifndef CHESS_VR_RECORDER_H
#define CHESS_VR_RECORDER_H


class Recorder {


public:
    Recorder();

    ~Recorder();

    /*
     * Write PGN tags in interactive mode from user input.
     */
    void writeTagsInteractive();
    /*
     * Return entire moveText as single string.
     */
    std::string getMoveText();

private:
    std::ofstream outfile;
    std::string moveText;
};


#endif //CHESS_VR_RECORDER_H
