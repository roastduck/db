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
    ASSERT_THAT(linkedMap.pop(), Eq(1));
    ASSERT_THAT(linkedMap.pop(), Eq(2));
    ASSERT_THAT(linkedMap.pop(), Eq(3));
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

TEST_F(LinkedHashMapTest, testUpdateOrderWithOp)
{
    linkedMap["a"] = 1;
    linkedMap["b"] = 2;
    linkedMap["c"] = 3;
    linkedMap["b"] = 4;
    ASSERT_THAT(linkedMap.pop(), Eq(1));
    ASSERT_THAT(linkedMap.pop(), Eq(3));
    ASSERT_THAT(linkedMap.pop(), Eq(4));
    ASSERT_TRUE(linkedMap.empty());
}

TEST_F(LinkedHashMapTest, testUpdateOrderWithAt)
{
    linkedMap["a"] = 1;
    linkedMap["b"] = 2;
    linkedMap["c"] = 3;
    linkedMap.at("b") = 4;
    ASSERT_THAT(linkedMap.pop(), Eq(1));
    ASSERT_THAT(linkedMap.pop(), Eq(3));
    ASSERT_THAT(linkedMap.pop(), Eq(4));
    ASSERT_TRUE(linkedMap.empty());
}

TEST_F(LinkedHashMapTest, testPop)
{
    linkedMap["a"] = 1;
    linkedMap.pop();
    ASSERT_THROW(linkedMap.at("a"), std::out_of_range);
}

