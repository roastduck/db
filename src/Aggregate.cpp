#include <cassert>
#include "utils.h"
#include "Aggregate.h"

void Aggregate::addSum(std::unique_ptr<Type> &prev, std::unique_ptr<Type> &&next)
{
    assert(prev->getTypeID() == next->getTypeID());
    switch (next->getTypeID())
    {
    case Type::INT:
        asInt(prev).setVal(asInt(prev).getVal() + asInt(next).getVal());
        break;
    case Type::FLOAT:
        asFloat(prev).setVal(asFloat(prev).getVal() + asFloat(next).getVal());
    default:
        throw InvalidTypeException(prev->getTypeID(), "SUM or AVG");
    }
}

void Aggregate::addMin(std::unique_ptr<Type> &prev, std::unique_ptr<Type> &&next)
{
    if (*next < *prev)
        prev = std::move(next);
}

void Aggregate::addMax(std::unique_ptr<Type> &prev, std::unique_ptr<Type> &&next)
{
    if (*next > *prev)
        prev = std::move(next);
}

void Aggregate::finAvg(std::unique_ptr<Type> &prev, int n)
{
    double result;
    switch (prev->getTypeID())
    {
    case Type::INT:
        result = asInt(prev).getVal();
        break;
    case Type::FLOAT:
        result = asFloat(prev).getVal();
    default:
        throw InvalidTypeException(prev->getTypeID(), "AVG");
    }
    prev = std::unique_ptr<FloatType>(new FloatType());
    asFloat(prev).setVal(result / n);
}

void Aggregate::add(Aggregate::AggType type, std::unique_ptr<Type> &prev, std::unique_ptr<Type> &&next)
{
    if (next == nullptr)
        return;
    if (prev == nullptr)
    {
        prev = std::move(next);
        return;
    }
    switch (type)
    {
    case SUM:
    case AVG:
        addSum(prev, std::move(next));
        break;
    case MIN:
        addMin(prev, std::move(next));
        break;
    case MAX:
        addMax(prev, std::move(next));
        break;
    default:
        assert(false);
    }
}

void Aggregate::fin(Aggregate::AggType type, std::unique_ptr<Type> &prev, int n)
{
    assert(prev != nullptr);
    switch (type)
    {
    case AVG:
        finAvg(prev, n);
        break;
    case SUM:
    case MIN:
    case MAX:
        break;
    default:
        assert(false);
    }
}

