#include <string>
#include <cstdlib>
#include <iostream>
#include <argp.h>
#include "TableMgr.h"
#include "io/Input.h"
#include "io/Output.h"
#include "io/Console.h"
#include "io/makepath.h"
#include "filesystem/PageCache.h"
#include "filesystem/FilePageMgr.h"

#define DEFAULT_DB_PATH "./.db"

class CmdArg
{
private:
    static constexpr argp_option options[] = {
        {
            "db-path",   'd',    "PATH_TO_DB",  OPTION_ARG_OPTIONAL,
            "Directory where all the DB files lie (default = " DEFAULT_DB_PATH ")",
            0 /* Group ID */
        },
        { 0, 0, 0, 0, 0, 0 } // Sentinel
    };

    static error_t parseOpt(int key, char *arg, struct argp_state*)
    {
        switch (key)
        {
        case 'd':
            if (!arg)
            {
                std::cerr << "ERROR: Wrong argument for -d, a path required" << std::endl;
                exit(-1);
            }
            dbPath = arg;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
        }
        return 0;
    }

public:
    static std::string dbPath;

    static void parse(int argc, char **argv)
    {
        dbPath = DEFAULT_DB_PATH;

        argp argpConf = {
            options,
            parseOpt,
            NULL /* Arg doc */,
            NULL /* Doc */,
            NULL /* Children */,
            NULL /* Help filter */,
            NULL /* Domain */
        };
        argp_parse(&argpConf, argc, argv, ARGP_NO_ARGS, NULL, NULL);
    }
};
constexpr argp_option CmdArg::options[];
std::string CmdArg::dbPath;

int main(int argc, char **argv)
{
    CmdArg::parse(argc, argv);
    std::cout << "Welcome to DB" << std::endl;

    if (makePath(CmdArg::dbPath))
        std::cout << "Using DB files in " << CmdArg::dbPath << std::endl;
    else
    {
        std::cerr << "ERROR: Cannot access or create DB directory " << CmdArg::dbPath << std::endl;
        return -1;
    }

    std::cout << std::endl;

    PageMgr *pageMgr = new FilePageMgr(CmdArg::dbPath);
    PageCache *cache = new PageCache(*pageMgr);
    TableMgr mgr(*cache);
    Output output(std::cout, std::cerr);
    Input input(mgr, output);
    Console console(input);
    console.mainLoop();
    delete cache; // Write cache into files (should before `delete pageMgr`)
    delete pageMgr; // Close files
    return 0;
}

