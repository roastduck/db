#ifndef TABLE_MGR_H_
#define TABLE_MGR_H_

#include <string>
#include <memory>
#include <unordered_map>
#include "Table.h"
#include "config.h"
#include "Optional.h"
#include "exception/NotNullException.h"
#include "exception/NoDBInUseException.h"
#include "exception/NoSuchThingException.h"
#include "exception/IDAlreadyUsedException.h"
#include "exception/RefereeNotPrimaryException.h"
#include "exception/ForeignKeyViolatedException.h"

class TableMgr
{
private:
    PageCache &cache;
    Table sysDbs, sysTables, sysCols, sysPriIdxes, sysNonClusIdxes, sysForeigns;

    Optional<std::string> curDb; /// Current DB in "use"
    std::unordered_map< std::string, std::unique_ptr<Table> > tables; /// All tables in `curDb`

    bool nameExists(Table &table, const std::vector<std::string> &col, const std::vector<std::string> &name);

    static std::string commaJoin(const std::vector<std::string> &strs);
    static std::vector<std::string> commaSep(const std::string &str);

public:
    TableMgr(PageCache &_cache);

    struct ForeignKey
    {
        std::string referee;
        Table::Index referrerCols, refereeCols;
    };

    /************************************/
    /* DB Managements                   */
    /************************************/

    /** Create a database
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

    /************************************/
    /* Table Managements                */
    /************************************/

    /** Create a table
     *  @throw : NoDBInUseException
     *  @throw : IDAlreadyUsedException
     *  @throw : RefereeNotPrimaryException
     */
    void createTable(
        const std::string &name,
        const Table::Cols &cols,
        const Optional<Table::Index> &primary = None(),
        const std::vector<Table::Index> &nonClus = {},
        const std::vector<ForeignKey> foreigns = {}
    );

    /** Drop a table
     *  @throw : NoDBInUseException
     */
    void dropTable(const std::string &name);

    /** Show all tables in a database
     *  @throw : NoDBInUseException
     */
    std::vector<Table::ColVal> showTables();

    /************************************/
    /* Queries                          */
    /************************************/

    /** INSERT INTO <tbName> VALUES <valueLists>
     *  @throw NotNullException
     */
    void insert(const std::string &tbName, std::vector<Table::ColL> valueLists);
};

#endif // TABLE_MGR_H_
