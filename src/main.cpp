#include <string>
#include <cstdlib>
#include <iostream>
#include <argp.h>
#include <unistd.h>
#include "TableMgr.h"
#include "io/Input.h"
#include "io/Output.h"
#include "io/Console.h"
#include "io/makepath.h"
#include "filesystem/PageCache.h"
#include "filesystem/FilePageMgr.h"

#define DEFAULT_DB_PATH "./.db"

class NullStream : public std::ostream
{
private:
    class NullBuffer : public std::streambuf
    {
    public:
        int overflow(int c) { return c; }
    } m_nb;
public:
    NullStream() : std::ostream(&m_nb) {}
} nullStream;

class CmdArg
{
private:
    static constexpr const char *doc =
        "Run `db` directly to enter a human friendly interactive interface.\n"
        "Run `db < input.sql > output.csv` to input command from a SQL file, "
        "and output results to a CSV file.";

    static constexpr argp_option options[] = {
        {
            "db-path",   'd',    "PATH_TO_DB",  0 /* Options */,
            "Directory where all the DB files lie (default = " DEFAULT_DB_PATH ")",
            0 /* Group ID */
        },
        {
            "database",  'D',    "DB_NAME",     0,
            "The database to use (equivalent to the \"USE\" statement)",
            0
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
        case 'D':
            if (!arg)
            {
                std::cerr << "ERROR: Wrong argument for -d, a path required" << std::endl;
                exit(-1);
            }
            dbUse = arg;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
        }
        return 0;
    }

public:
    static std::string dbPath, dbUse;

    static void parse(int argc, char **argv)
    {
        dbPath = DEFAULT_DB_PATH;

        argp argpConf = {
            options,
            parseOpt,
            NULL /* Arg doc */,
            doc,
            NULL /* Children */,
            NULL /* Help filter */,
            NULL /* Domain */
        };
        argp_parse(&argpConf, argc, argv, ARGP_NO_ARGS, NULL, NULL);
    }
};
constexpr const char *CmdArg::doc;
constexpr argp_option CmdArg::options[];
std::string CmdArg::dbPath, CmdArg::dbUse;

int main(int argc, char **argv)
{
    CmdArg::parse(argc, argv);
    std::clog << "Welcome to DB" << std::endl;

    if (makePath(CmdArg::dbPath))
        std::clog << "Using DB files in " << CmdArg::dbPath << std::endl;
    else
    {
        std::cerr << "ERROR: Cannot access or create DB directory " << CmdArg::dbPath << std::endl;
        return -1;
    }

    std::clog << std::endl;

    PageMgr *pageMgr = new FilePageMgr(CmdArg::dbPath);
    PageCache *cache = new PageCache(*pageMgr);
    TableMgr mgr(*cache);
    if (!CmdArg::dbUse.empty())
    {
        mgr.use(CmdArg::dbUse);
        std::clog << "Using database " << CmdArg::dbUse << std::endl << std::endl;
    }
    if (isatty(fileno(stdin)))
    {
        Output output(std::cout, std::clog, std::cerr);
        Input input(mgr, output);
        Console console(input);
        console.mainLoop();
    } else
    {
        Output output(std::cout, nullStream, std::cerr);
        output.setCsvMode(true);
        Input input(mgr, output);
        input.parse(std::cin);
    }
    delete cache; // Write cache into files (should before `delete pageMgr`)
    delete pageMgr; // Close files
    return 0;
}

