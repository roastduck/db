#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <cctype>
#include <string>
#include "Input.h"

class Console
{
private:
    Input &input;
    std::string buf;

    /** trim from start (in place)
     */
    static void ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) {
            return !std::isspace(c);
        }));
    }

    /** trim from end (in place)
     */
    static inline void rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int c) {
            return !std::isspace(c);
        }).base(), s.end());
    }

    /** trim from both ends (in place)
     */
    static inline void trim(std::string &s)
    {
        ltrim(s);
        rtrim(s);
    }

public:
    Console(Input &_input) : input(_input) {}

    /** Input command(s) into a buffer, which can be incomplete.
     *  If the buffer contains complete command(s), exceute it and return true,
     *  otherwise return false
     */
    bool addCommand(const std::string &cmd);

    /** Main interative loop
     */
    void mainLoop();
};

#endif // CONSOLE_H_
