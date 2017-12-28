#ifndef TABLE_MGR_H_
#define TABLE_MGR_H_

#include <string>
#include <memory>
#include <unordered_map>
#include "Table.h"
#include "config.h"
#include "Optional.h"
#include "filesystem/PairHash.h"
#include "exception/NotNullException.h"
#include "exception/NoDBInUseException.h"
#include "exception/NoSuchThingException.h"
#include "exception/IDAlreadyUsedException.h"
#include "exception/TooManyFieldsException.h"
#include "exception/TooManyIndexesException.h"
#include "exception/DuplicateIndexException.h"
#include "exception/RefereeNotPrimaryException.h"
#include "exception/ForeignKeyViolatedException.h"
#include "exception/ValueListLengthNotMatchException.h"

class TableMgr
{
public:
    // Field names in system tables
    // We are defining these because they might be output
    static constexpr const char *DB = "Database";
    static constexpr const char *TABLE = "Table";
    static constexpr const char *FIELD = "Field";
    static constexpr const char *TYPE = "Type";
    static constexpr const char *LENGTH = "Length";
    static constexpr const char *NOT_NULL = "Not null";
    static constexpr const char *IS_PRI = "Primary key";

private:
    // Field names in system tables
    PageCache &cache;
    Table sysDbs, sysTables, sysCols, sysPriIdxes, sysNonClusIdxes, sysForeigns;

    Optional<std::string> curDb; /// Current DB in "use"
    std::unordered_map< std::string, std::unique_ptr<Table> > tables; /// All tables in `curDb`

    std::pair< std::string, std::vector<Table::ConLiteral> > genEquCon(const std::string &col, const std::string &literal);
    Table::ConsL genEquConsL(const Table::ColL &row);

    bool nameExists(Table &table, const std::vector<std::string> &col, const std::vector<std::string> &name);

    void checkFieldInCons(const std::string &tbName, const Table::ConsL &cons, const Table::OuterCons &oCons);

public:
    TableMgr(PageCache &_cache);

    struct ForeignKey
    {
        std::string referee;
        Table::Index referrerCols, refereeCols;
    };

    typedef std::unordered_map< std::pair<std::string, std::string>, Table::OuterCons, PairHash<std::string, std::string> > OuterConsMap;

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
     *  @throw : NotNullException
     *  @throw : NoDBInUseException
     *  @throw : NoSuchThingException
     *  @throw : IDAlreadyUsedException
     *  @throw : TooManyFieldsException
     *  @throw : RefereeNotPrimaryException
     */
    void createTable(
        const std::string &name,
        const std::vector< std::pair<std::string, Column> > &cols,
        const Optional<Table::Index> &primary = None(),
        const std::vector<Table::Index> &nonClus = {},
        const std::vector<ForeignKey> foreigns = {}
    );

    /** Drop a table
     *  @throw : NoSuchThingException
     */
    void dropTable(const std::string &name);

    /** Show all tables in a database
     *  @throw : NoDBInUseException
     */
    std::vector<Table::ColVal> showTables();

    /** DESC <tbName>
     *  @throw : NoSuchThingException
     */
    std::vector<Table::ColVal> desc(const std::string &name);

    /************************************/
    /* Index Managements                */
    /************************************/

    // If no DB in use in queries, just throw NoSuchThingException

    /** CREATE INDEX <tbName> '(' <colName> ')'
     *  @throw : NoSuchThingException
     *  @throw : TooManyIndexesException
     */
    void createIndex(const std::string &tbName, const Table::Index &colName);

    /** DROP INDEX <tbName> '(' <colName> ')'
     *  @throw : NoSuchThingException
     */
    void dropIndex(const std::string &tbName, const Table::Index &colName);

    /************************************/
    /* Queries                          */
    /************************************/

    // If no DB in use in queries, just throw NoSuchThingException

    /** INSERT INTO <tbName> VALUES <valueLists>
     *  @throw : NoSuchThingException
     *  @throw : NotNullException
     *  @throw : ValueListLengthNotMatchException
     *  @throw : ForeignKeyViolatedException
     */
    void insert(const std::string &tbName, const std::vector< std::vector< Optional<std::string> > > &valueLists);

    /** DELETE FROM <tbName> WHERE <whereClause>
     *  @throw : NoSuchThingException
     *  @throw : ForeignKeyViolatedException
     */
    void remove(const std::string &tbName, const Table::ConsL &cons, const Table::OuterCons &oCons = {});

    /** UPDATE <tbName> SET <setClause> WHERE <whereClause>
     *  @throw : NoSuchThingException
     *  @throw : ForeignKeyViolatedException
     *  @throw : NotNullException
     */
    void update(
        const std::string &tbName, const Table::ColL &setClause, const Table::ConsL &cons, const Table::OuterCons &oCons = {}
    );

    /** SELECT <selector> FROM <tableList> WHERE <whereClause>
     *  @throw : NoSuchThingException
     */
    std::vector<Table::ColVal> select(
        std::unordered_map< std::string, Table::Index > targets, /// table -> columns
        const std::vector< std::string > &tableList,
        const std::unordered_map< std::string, Table::ConsL > &innerCons, /// table -> constraints
        const OuterConsMap &outterCons = {}
    );
};

#endif // TABLE_MGR_H_
