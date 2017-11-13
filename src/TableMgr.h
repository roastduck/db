#ifndef TABLE_MGR_H_
#define TABLE_MGR_H_

#include <string>
#include <memory>
#include <unordered_map>
#include "Table.h"
#include "Optional.h"
#include "exception/NoSuchThingException.h"
#include "exception/IDAlreadyUsedException.h"

class TableMgr
{
private:
    const int MAX_IDENTIFIER_LEN = 128;
    const int MAX_COLUMN_NUM = 32;

    PageCache &cache;
    Table sysDbs, sysTables, sysCols, sysPriIdxes, sysNonClusIdxes;

    Optional<std::string> db; /// Current selected db

    bool nameExists(Table &table, const std::string &col, const std::string &name);

public:
    TableMgr(PageCache &_cache);

    /************************************/
    /* DB Managements                   */
    /************************************/

    /** Create database
     *  @throw : IDAlreadyUsedException
     */
    void createDb(const std::string &name);

    /** Switch to a database
     *  @throw : NoSuchThingException
     */
    void use(const std::string &name);

    /** Drop a database
     *  @throw : NoSuchThingException
     */
    void dropDb(const std::string &name);

    /** Show all databases
     */
    std::vector<Table::ColVal> showDbs();
};

#endif // TABLE_MGR_H_
