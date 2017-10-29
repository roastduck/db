#ifndef TABLE_H_
#define TABLE_H_

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "Type.h"
#include "Column.h"
#include "ListPage.h"
#include "BitmapPage.h"
#include "filesystem/PageCache.h"

class Table
{
private:
    PageCache &cache;
    std::unordered_map<std::string, Column> cols;

    std::string dataFile, freeListFile;
    std::vector<ListPage> dataPages; // Keep these metadata of pages in memory reduces overhead
    std::vector<BitmapPage> freeListPages;

public:
    enum ConDir // Constraint Direction
    {
        EQ, // ==
        NE, // !=
        LT, // <
        LE, // <=
        GT, // >
        GE  // >=
    };

    struct ConLiteral
    {
        ConDir dir;
        std::string pivot;
    };

    struct ConValue
    {
        ConDir dir;
        std::unique_ptr<Type> pivot;
    };

    typedef std::unordered_map< std::string, ConValue > Cons;
    typedef std::unordered_map< std::string, std::unique_ptr<Type> > ColVal;

private:
    Cons genConstraints(const std::unordered_map<std::string, ConLiteral> &literals);
    ColVal genVals(const std::unordered_map<std::string, std::string> &literals);

    ListPage &getDataPage(int pageID);
    BitmapPage &getFreeListPage(int pageID);

    int newDataPage();

    bool meetCons(ListPage &page, int rank, const Cons &cons);

    void insert(const ColVal &vals);
    void remove(const Cons &constraints);
    std::vector<ColVal> select(const std::vector<std::string> &targets, const Cons &constraints);

public:
    Table(PageCache &_cache, const std::string &_tableName, const std::unordered_map<std::string, Column> &_cols);

    /** Insert a map of (column name, literal) into the table
     */
    void insert(const std::unordered_map<std::string, std::string> &literals)
    {
        insert(genVals(literals));
    }

    /** Delete records that meet the constraint
     */
    void remove(const std::unordered_map<std::string, ConLiteral> &constraints)
    {
        remove(genConstraints(constraints));
    }

    /** Select records
     */
    std::vector<ColVal> select(
        const std::vector<std::string> &targets,
        const std::unordered_map<std::string, ConLiteral> &constraints
    )
    {
        return select(targets, genConstraints(constraints));
    }
};

#endif // TABLE_H_
