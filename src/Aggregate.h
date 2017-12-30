#ifndef AGGREGATE_H_
#define AGGREGATE_H_

#include <memory>
#include "Type.h"
#include "exception/InvalidTypeException.h"

/// Helper class for aggregating functions
class Aggregate
{
private:
    static void addSum(std::unique_ptr<Type> &prev, std::unique_ptr<Type> &&next);
    static void addMin(std::unique_ptr<Type> &prev, std::unique_ptr<Type> &&next);
    static void addMax(std::unique_ptr<Type> &prev, std::unique_ptr<Type> &&next);

    static void finAvg(std::unique_ptr<Type> &prev, int n);

public:
    enum AggType { NONE = 0, SUM = 1, AVG = 2, MAX = 3, MIN = 4 };

    /** Add a row to an aggregated value
     *  @param prev [in,out]: Aggregated value of all calculated rows
     *  @param next [in]: The new row to be added in
     *  @throw InvalidTypeException
     */
    static void add(AggType type, std::unique_ptr<Type> &prev, std::unique_ptr<Type> &&next);

    /** Finalize an aggregation
     *  @param prev [in,out]: Aggregated value of all calculated rows
     *  @param n [in]: # of aggregated rows
     */
    static void fin(AggType type, std::unique_ptr<Type> &prev, int n);
};

#endif // AGGREGATE_H_
