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
     *
     * @param void
     * @return void
     */
    void writeTagsInteractive();
    /*
     * Get entire moveText part of the PGN to be written to file.
     *
     * @param void
     * @return string of moveText moves.
     */
    std::string getMoveText();

private:
    std::ofstream outfile;
    std::string moveText;
};


#endif //CHESS_VR_RECORDER_H
