#include <string>
#include "filesystem/LinkedHashMap.h"

#include "gmock/gmock.h"

using namespace testing;

class LinkedHashMapTest : public Test
{
public:
    LinkedHashMap<std::string, int> linkedMap;
};

TEST_F(LinkedHashMapTest, testFIFO)
{
    linkedMap["a"] = 1;
    linkedMap["b"] = 2;
    linkedMap["c"] = 3;
    ASSERT_THAT(linkedMap.pop().second, Eq(1));
    ASSERT_THAT(linkedMap.pop().second, Eq(2));
    ASSERT_THAT(linkedMap.pop().second, Eq(3));
    ASSERT_TRUE(linkedMap.empty());
}

TEST_F(LinkedHashMapTest, testAccessWithOp)
{
    linkedMap["a"] = 1;
    ASSERT_THAT(linkedMap["a"], Eq(1));
}

TEST_F(LinkedHashMapTest, testAccessWithAt)
{
    linkedMap["a"] = 1;
    ASSERT_THAT(linkedMap.at("a"), Eq(1));
}

TEST_F(LinkedHashMapTest, testOutOfRange)
{
    ASSERT_THROW(linkedMap.at("a"), std::out_of_range);
}

TEST_F(LinkedHashMapTest, testAccessWithFind)
{
    linkedMap["a"] = 1;
    ASSERT_THAT(linkedMap.find("a").ok(), Eq(1));
}

TEST_F(LinkedHashMapTest, testNone)
{
    ASSERT_TRUE(!linkedMap.find("a").isOk());
}

TEST_F(LinkedHashMapTest, testUpdateOrderWithOp)
{
    linkedMap["a"] = 1;
    linkedMap["b"] = 2;
    linkedMap["c"] = 3;
    linkedMap["b"] = 4;
    ASSERT_THAT(linkedMap.pop().second, Eq(1));
    ASSERT_THAT(linkedMap.pop().second, Eq(3));
    ASSERT_THAT(linkedMap.pop().second, Eq(4));
    ASSERT_TRUE(linkedMap.empty());
}

TEST_F(LinkedHashMapTest, testUpdateOrderWithAt)
{
    linkedMap["a"] = 1;
    linkedMap["b"] = 2;
    linkedMap["c"] = 3;
    linkedMap.at("b") = 4;
    ASSERT_THAT(linkedMap.pop().second, Eq(1));
    ASSERT_THAT(linkedMap.pop().second, Eq(3));
    ASSERT_THAT(linkedMap.pop().second, Eq(4));
    ASSERT_TRUE(linkedMap.empty());
}

TEST_F(LinkedHashMapTest, testPop)
{
    linkedMap["a"] = 1;
    auto popped = linkedMap.pop();
    ASSERT_THAT(popped.first, Eq("a"));
    ASSERT_THAT(popped.second, Eq(1));
    ASSERT_THROW(linkedMap.at("a"), std::out_of_range);
}

