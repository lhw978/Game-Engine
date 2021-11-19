/*
    Copyright (c) 2018-2021 Piotr Doan. All rights reserved.
    Software distributed under the permissive MIT License.
*/

#include <sstream>
#include <Common/Name.hpp>
#include <Reflection/Reflection.hpp>
#include <gtest/gtest.h>

/*
    Helpers
*/

class TestNameType
{
};

REFLECTION_TYPE(TestNameType);

/*
    Name
*/

TEST(Name, Empty)
{
    Common::Name nameEmpty;
    EXPECT_EQ(nameEmpty, Common::StringHash<Common::Name::HashType>(""));
}

TEST(Name, Basic)
{
    Common::Name nameOne = NAME_CONSTEXPR("One");
    Common::Name nameTwo = NAME_CONSTEXPR("Two");

    EXPECT_NE(nameOne, nameTwo);
    EXPECT_EQ(nameOne, Common::Name("One"));
    EXPECT_EQ(nameTwo, Common::Name("Two"));
}

TEST(Name, Copy)
{
    Common::Name nameOne = NAME_CONSTEXPR("One");
    Common::Name nameTwo = NAME_CONSTEXPR("Two");

    Common::Name nameOneCopy;
    nameOneCopy = nameOne;
    EXPECT_EQ(nameOneCopy, nameOne);

    Common::Name nameTwoCopy(nameTwo);
    EXPECT_EQ(nameTwoCopy, nameTwo);
}

TEST(Name, Identifier)
{
    Common::Name nameOne = NAME_CONSTEXPR("One");
    Common::Name nameTwo = NAME_CONSTEXPR("Two");

    Common::Name nameIdentifier(Common::StringHash<Common::Name::HashType>("One"));

    EXPECT_EQ(nameOne, nameIdentifier);
    EXPECT_NE(nameTwo, nameIdentifier);
}

TEST(Name, Hash)
{
    Common::Name nameTest = NAME_CONSTEXPR("TestNameType");
    EXPECT_EQ(nameTest.GetHash(), Reflection::StaticType<TestNameType>().Identifier);
}

TEST(Name, String)
{
    std::string string = Common::Name("TestNameType").GetString();

#ifdef NAME_REGISTRY_ENABLED
    std::string compare = "TestNameType";
#else
    std::stringstream stream;
    stream << "{";
    stream << Common::StringHash<Common::Name::HashType>("TestNameType");
    stream << "}";
    std::string compare = stream.str();
#endif

    EXPECT_EQ(string, compare);
}
