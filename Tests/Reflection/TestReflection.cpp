/*
    Copyright (c) 2018-2020 Piotr Doan. All rights reserved.
*/

#include <cinttypes>
#include <TestHelpers.hpp>
#include <Reflection/Reflection.hpp>

class Undefined
{
};

class Empty
{

};

REFLECTION_TYPE_BEGIN(Empty)
REFLECTION_TYPE_END

class BaseAttribute : public Reflection::TypeAttribute
{
public:
    bool operator==(const BaseAttribute& other) const
    {
        return true;
    }
};

REFLECTION_TYPE(BaseAttribute, Reflection::TypeAttribute)

class TextAttribute : public Reflection::FieldAttribute
{
public:
    bool operator==(const TextAttribute& other) const
    {
        return true;
    }
};

REFLECTION_TYPE(TextAttribute, Reflection::FieldAttribute)

class Base
{
public:
    std::string textWithoutAttribute;
    const char* textPtrWithAttribute;
};

REFLECTION_TYPE_BEGIN(Base)
    REFLECTION_ATTRIBUTES(BaseAttribute())
    REFLECTION_FIELD(textWithoutAttribute)
    REFLECTION_FIELD(textPtrWithAttribute, TextAttribute())
REFLECTION_TYPE_END

class DerivedAttribute : public Reflection::TypeAttribute
{
public:
    constexpr DerivedAttribute(bool state) :
        state(state)
    {
    }

    const bool state;
};

REFLECTION_TYPE(DerivedAttribute, Reflection::TypeAttribute)

class CounterAttribute : public Reflection::FieldAttribute
{
public:
    constexpr CounterAttribute(bool state) :
        state(state)
    {
    }

    const bool state;
};

REFLECTION_TYPE(CounterAttribute, Reflection::FieldAttribute)

class Derived : public Base
{
public:
    int counter;
};

REFLECTION_TYPE_BEGIN(Derived, Base)
    REFLECTION_ATTRIBUTES(DerivedAttribute(false))
    REFLECTION_FIELD(counter, CounterAttribute(true))
REFLECTION_TYPE_END

class InnerAttribute : public Reflection::FieldAttribute
{
public:
    constexpr InnerAttribute(int counter) :
        counter(counter)
    {
    }

    const int counter;
};

REFLECTION_TYPE(InnerAttribute, Reflection::FieldAttribute)

class Inner
{
public:
    uint8_t value;
};

REFLECTION_TYPE_BEGIN(Inner)
    REFLECTION_FIELD(value, InnerAttribute(20))
REFLECTION_TYPE_END

class ToggleOnAttribute : public Reflection::FieldAttribute
{
public:
    const bool state = true;
};

REFLECTION_TYPE(ToggleOnAttribute, Reflection::FieldAttribute)

class ToggleOffAttribute : public Reflection::FieldAttribute
{
public:
    const bool state = false;
};

REFLECTION_TYPE(ToggleOffAttribute, Reflection::FieldAttribute)

class BranchedOne : public Derived
{
public:
    bool toggle;
    Inner inner;
};

REFLECTION_TYPE_BEGIN(BranchedOne, Derived)
    REFLECTION_FIELD(toggle, ToggleOnAttribute(), ToggleOffAttribute())
    REFLECTION_FIELD(inner)
REFLECTION_TYPE_END

class BranchedAttributeOne : public Reflection::TypeAttribute
{
public:
    constexpr BranchedAttributeOne(std::string_view modifier) :
        modifier(modifier)
    {
    }

    const std::string_view modifier;
};

REFLECTION_TYPE(BranchedAttributeOne, Reflection::TypeAttribute)

class BranchedAttributeTwo : public Reflection::TypeAttribute
{
public:
    constexpr BranchedAttributeTwo(std::string_view modifier) :
        modifier(modifier)
    {
    }

    const std::string_view modifier;
};

REFLECTION_TYPE(BranchedAttributeTwo, Reflection::TypeAttribute)

class LetterAttribute : public Reflection::FieldAttribute
{
public:
    constexpr LetterAttribute(std::string_view modifier) :
        modifier(modifier)
    {
    }

    const std::string_view modifier;
};

REFLECTION_TYPE(LetterAttribute, Reflection::FieldAttribute)

class BranchedTwo : public Derived
{
public:
    char letterOne;
    char letterTwo;
};

REFLECTION_TYPE_BEGIN(BranchedTwo, Derived)
    REFLECTION_ATTRIBUTES(BranchedAttributeOne("Small"), BranchedAttributeTwo("Big"))
    REFLECTION_FIELD(letterOne, LetterAttribute("Pretty"))
    REFLECTION_FIELD(letterTwo, LetterAttribute("Ugly"))
REFLECTION_TYPE_END

bool TestTypes()
{
    // Check built-in reflection.
    TEST_TRUE(Reflection::IsReflected<Reflection::NullType>());
    TEST_TRUE(Reflection::Reflect<Reflection::NullType>().IsNullType());
    TEST_EQ(Reflection::Reflect<Reflection::NullType>().Name, "Reflection::NullType");
    TEST_FALSE(Reflection::Reflect<Undefined>().IsNullType());
    TEST_FALSE(Reflection::Reflect<Derived>().IsNullType());

    // Check reflection presence.
    TEST_FALSE(Reflection::IsReflected<Undefined>());
    TEST_TRUE(Reflection::IsReflected<Empty>());
    TEST_TRUE(Reflection::IsReflected<Base>());
    TEST_TRUE(Reflection::IsReflected<Derived>());
    TEST_TRUE(Reflection::IsReflected<Inner>());
    TEST_TRUE(Reflection::IsReflected<BranchedOne>());
    TEST_TRUE(Reflection::IsReflected<BranchedTwo>());

    // Check type name.
    TEST_EQ(Reflection::Reflect<Empty>().Name, "Empty");
    TEST_EQ(Reflection::Reflect<Base>().Name, "Base");
    TEST_EQ(Reflection::Reflect<Derived>().Name, "Derived");
    TEST_EQ(Reflection::Reflect<Inner>().Name, "Inner");
    TEST_EQ(Reflection::Reflect<BranchedOne>().Name, "BranchedOne");
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Name, "BranchedTwo");

    // Check type by value.
    TEST_FALSE(Reflection::Reflect(Undefined()).IsType<Empty>());
    TEST_TRUE(Reflection::Reflect(Undefined()).IsType<Undefined>());
    TEST_TRUE(Reflection::Reflect(Empty()).IsType<Empty>());
    TEST_TRUE(Reflection::Reflect(Base()).IsType<Base>());
    TEST_TRUE(Reflection::Reflect(Derived()).IsType<Derived>());
    TEST_TRUE(Reflection::Reflect(Inner()).IsType<Inner>());
    TEST_TRUE(Reflection::Reflect(BranchedOne()).IsType<BranchedOne>());
    TEST_TRUE(Reflection::Reflect(BranchedTwo()).IsType<BranchedTwo>());

    // Check base type presence.
    TEST_FALSE(Reflection::Reflect<Undefined>().HasBaseType());
    TEST_FALSE(Reflection::Reflect<Empty>().HasBaseType());
    TEST_FALSE(Reflection::Reflect<Base>().HasBaseType());
    TEST_TRUE(Reflection::Reflect<Derived>().HasBaseType());
    TEST_FALSE(Reflection::Reflect<Inner>().HasBaseType());
    TEST_TRUE(Reflection::Reflect<BranchedOne>().HasBaseType());
    TEST_TRUE(Reflection::Reflect<BranchedTwo>().HasBaseType());

    // Check base types.
    TEST_EQ(Reflection::Reflect<Derived>().GetBaseType().Name, "Base");
    TEST_EQ(Reflection::Reflect<BranchedOne>().GetBaseType().Name, "Derived");
    TEST_EQ(Reflection::Reflect<BranchedTwo>().GetBaseType().Name, "Derived");
    TEST_TRUE(Reflection::Reflect<Derived>().GetBaseType().IsType<Base>());
    TEST_TRUE(Reflection::Reflect<BranchedOne>().GetBaseType().IsType<Derived>());
    TEST_TRUE(Reflection::Reflect<BranchedTwo>().GetBaseType().IsType<Derived>());

    // Check type attribute presence.
    TEST_FALSE(Reflection::Reflect<Empty>().HasAttributes());
    TEST_TRUE(Reflection::Reflect<Base>().HasAttributes());
    TEST_TRUE(Reflection::Reflect<Derived>().HasAttributes());
    TEST_FALSE(Reflection::Reflect<Inner>().HasAttributes());
    TEST_FALSE(Reflection::Reflect<BranchedOne>().HasAttributes());
    TEST_TRUE(Reflection::Reflect<BranchedTwo>().HasAttributes());

    // Check type attribute count.
    TEST_EQ(Reflection::Reflect<Empty>().Attributes.Count, 0);
    TEST_EQ(Reflection::Reflect<Base>().Attributes.Count, 1);
    TEST_EQ(Reflection::Reflect<Derived>().Attributes.Count, 1);
    TEST_EQ(Reflection::Reflect<Inner>().Attributes.Count, 0);
    TEST_EQ(Reflection::Reflect<BranchedOne>().Attributes.Count, 0);
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Attributes.Count, 2);

    // Check type attribute names.
    TEST_EQ(Reflection::Reflect<Base>().Attribute<0>().Name, "BaseAttribute");
    TEST_EQ(Reflection::Reflect<Derived>().Attribute<0>().Name, "DerivedAttribute");
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Attribute<0>().Name, "BranchedAttributeOne");
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Attribute<1>().Name, "BranchedAttributeTwo");

    // Check type attribute types.
    TEST_FALSE(Reflection::Reflect<Base>().Attribute<0>().IsType<DerivedAttribute>());
    TEST_TRUE(Reflection::Reflect<Base>().Attribute<0>().IsType<BaseAttribute>());
    TEST_TRUE(Reflection::Reflect<Derived>().Attribute<0>().IsType<DerivedAttribute>());
    TEST_TRUE(Reflection::Reflect<BranchedTwo>().Attribute<0>().IsType<BranchedAttributeOne>());
    TEST_TRUE(Reflection::Reflect<BranchedTwo>().Attribute<1>().IsType<BranchedAttributeTwo>());

    // Check type attribute instances.
    TEST_EQ(Reflection::Reflect<Base>().Attribute<0>().Instance, BaseAttribute());
    TEST_EQ(Reflection::Reflect<Derived>().Attribute<0>().Instance.state, false);
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Attribute<0>().Instance.modifier, "Small");
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Attribute<1>().Instance.modifier, "Big");

    // Check member count.
    TEST_EQ(Reflection::Reflect<Empty>().Members.Count, 0);
    TEST_EQ(Reflection::Reflect<Base>().Members.Count, 2);
    TEST_EQ(Reflection::Reflect<Derived>().Members.Count, 1);
    TEST_EQ(Reflection::Reflect<Inner>().Members.Count, 1);
    TEST_EQ(Reflection::Reflect<BranchedOne>().Members.Count, 2);
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Members.Count, 2);

    // Check member names.
    TEST_EQ(Reflection::Reflect<Base>().Member<0>().Name, "textWithoutAttribute");
    TEST_EQ(Reflection::Reflect<Base>().Member<1>().Name, "textPtrWithAttribute");
    TEST_EQ(Reflection::Reflect<Derived>().Member<0>().Name, "counter");
    TEST_EQ(Reflection::Reflect<Inner>().Member<0>().Name, "value");
    TEST_EQ(Reflection::Reflect<BranchedOne>().Member<0>().Name, "toggle");
    TEST_EQ(Reflection::Reflect<BranchedOne>().Member<1>().Name, "inner");
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Member<0>().Name, "letterOne");
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Member<1>().Name, "letterTwo");

    // Check member types.
    TEST_FALSE(Reflection::Reflect<Base>().Member<0>().IsType<void>());
    TEST_TRUE(Reflection::Reflect<Base>().Member<0>().IsType<std::string>());
    TEST_TRUE(Reflection::Reflect<Base>().Member<1>().IsType<const char*>());
    TEST_TRUE(Reflection::Reflect<Derived>().Member<0>().IsType<int>());
    TEST_TRUE(Reflection::Reflect<BranchedOne>().Member<0>().IsType<bool>());
    TEST_TRUE(Reflection::Reflect<BranchedOne>().Member<1>().IsType<Inner>());
    TEST_TRUE(Reflection::Reflect<BranchedTwo>().Member<0>().IsType<char>());
    TEST_TRUE(Reflection::Reflect<BranchedTwo>().Member<1>().IsType<char>());

    // Check member pointers.
    TEST_EQ(Reflection::Reflect<Base>().Member<0>().Pointer, &Base::textWithoutAttribute);
    TEST_EQ(Reflection::Reflect<Base>().Member<1>().Pointer, &Base::textPtrWithAttribute);
    TEST_EQ(Reflection::Reflect<Derived>().Member<0>().Pointer, &Derived::counter);
    TEST_EQ(Reflection::Reflect<Inner>().Member<0>().Pointer, &Inner::value);
    TEST_EQ(Reflection::Reflect<BranchedOne>().Member<0>().Pointer, &BranchedOne::toggle);
    TEST_EQ(Reflection::Reflect<BranchedOne>().Member<1>().Pointer, &BranchedOne::inner);
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Member<0>().Pointer, &BranchedTwo::letterOne);
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Member<1>().Pointer, &BranchedTwo::letterTwo);

    // Check type attribute count.
    TEST_EQ(Reflection::Reflect<Base>().Member<0>().Attributes.Count, 0);
    TEST_EQ(Reflection::Reflect<Base>().Member<1>().Attributes.Count, 1);
    TEST_EQ(Reflection::Reflect<Derived>().Member<0>().Attributes.Count, 1);
    TEST_EQ(Reflection::Reflect<Inner>().Member<0>().Attributes.Count, 1);
    TEST_EQ(Reflection::Reflect<BranchedOne>().Member<0>().Attributes.Count, 2);
    TEST_EQ(Reflection::Reflect<BranchedOne>().Member<1>().Attributes.Count, 0);
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Member<0>().Attributes.Count, 1);
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Member<1>().Attributes.Count, 1);

    // Check type attribute names.
    TEST_EQ(Reflection::Reflect<Base>().Member<1>().Attribute<0>().Name, "TextAttribute");
    TEST_EQ(Reflection::Reflect<Derived>().Member<0>().Attribute<0>().Name, "CounterAttribute");
    TEST_EQ(Reflection::Reflect<Inner>().Member<0>().Attribute<0>().Name, "InnerAttribute");
    TEST_EQ(Reflection::Reflect<BranchedOne>().Member<0>().Attribute<0>().Name, "ToggleOnAttribute");
    TEST_EQ(Reflection::Reflect<BranchedOne>().Member<0>().Attribute<1>().Name, "ToggleOffAttribute");
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Member<0>().Attribute<0>().Name, "LetterAttribute");
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Member<1>().Attribute<0>().Name, "LetterAttribute");

    // Check type attribute types.
    TEST_TRUE(Reflection::Reflect<Base>().Member<1>().Attribute<0>().IsType<TextAttribute>());
    TEST_TRUE(Reflection::Reflect<Derived>().Member<0>().Attribute<0>().IsType<CounterAttribute>());
    TEST_TRUE(Reflection::Reflect<Inner>().Member<0>().Attribute<0>().IsType<InnerAttribute>());
    TEST_TRUE(Reflection::Reflect<BranchedOne>().Member<0>().Attribute<0>().IsType<ToggleOnAttribute>());
    TEST_TRUE(Reflection::Reflect<BranchedOne>().Member<0>().Attribute<1>().IsType<ToggleOffAttribute>());
    TEST_TRUE(Reflection::Reflect<BranchedTwo>().Member<0>().Attribute<0>().IsType<LetterAttribute>());
    TEST_TRUE(Reflection::Reflect<BranchedTwo>().Member<1>().Attribute<0>().IsType<LetterAttribute>());

    // Check type attribute instances.
    TEST_EQ(Reflection::Reflect<Base>().Member<1>().Attribute<0>().Instance, TextAttribute());
    TEST_EQ(Reflection::Reflect<Derived>().Member<0>().Attribute<0>().Instance.state, true);
    TEST_EQ(Reflection::Reflect<Inner>().Member<0>().Attribute<0>().Instance.counter, 20);
    TEST_EQ(Reflection::Reflect<BranchedOne>().Member<0>().Attribute<0>().Instance.state, true);
    TEST_EQ(Reflection::Reflect<BranchedOne>().Member<0>().Attribute<1>().Instance.state, false);
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Member<0>().Attribute<0>().Instance.modifier, "Pretty");
    TEST_EQ(Reflection::Reflect<BranchedTwo>().Member<1>().Attribute<0>().Instance.modifier, "Ugly");

    // Enumerate type attributes.
    {
        std::vector<std::string_view> expectedAttributes;
        std::vector<std::string_view> presentAttributes;

        Reflection::ForEach(Reflection::Reflect<Empty>().Attributes,
            [&presentAttributes](const auto& attribute)
            {
                presentAttributes.push_back(attribute.Name);
            }
        );

        TEST_EQ(presentAttributes, expectedAttributes);
    }

    {
        std::vector<std::string_view> expectedAttributes = { "DerivedAttribute" };
        std::vector<std::string_view> presentAttributes;

        Reflection::ForEach(Reflection::Reflect<Derived>().Attributes,
            [&presentAttributes](const auto& attribute)
            {
                presentAttributes.push_back(attribute.Name);
            }
        );

        TEST_EQ(presentAttributes, expectedAttributes);
    }

    {
        std::vector<std::string_view> expectedAttributes = { "BranchedAttributeOne", "BranchedAttributeTwo" };
        std::vector<std::string_view> presentAttributes;

        Reflection::ForEach(Reflection::Reflect<BranchedTwo>().Attributes,
            [&presentAttributes](const auto& attribute)
            {
                presentAttributes.push_back(attribute.Name);
            }
        );

        TEST_EQ(presentAttributes, expectedAttributes);
    }

    {
        std::vector<std::string_view> expectedAttributes = { "Small", "Big" };
        std::vector<std::string_view> presentAttributes;

        Reflection::ForEach(Reflection::Reflect<BranchedTwo>().Attributes,
            [&presentAttributes](const auto& attribute)
            {
                presentAttributes.push_back(attribute.Instance.modifier);
            }
        );

        TEST_EQ(presentAttributes, expectedAttributes);
    }

    // Enumerate members.
    {
        std::vector<std::string_view> expectedMembers;
        std::vector<std::string_view> presentMembers;

        Reflection::ForEach(Reflection::Reflect<Empty>().Members,
            [&presentMembers](const auto& member)
            {
                presentMembers.push_back(member.Name);
            }
        );

        TEST_EQ(presentMembers, expectedMembers);
    }

    {
        std::vector<std::string_view> expectedMembers = { "textWithoutAttribute", "textPtrWithAttribute" };
        std::vector<std::string_view> presentMembers;

        Reflection::ForEach(Reflection::Reflect<Base>().Members,
            [&presentMembers](const auto& member)
            {
                presentMembers.push_back(member.Name);
            }
        );

        TEST_EQ(presentMembers, expectedMembers);
    }

    {
        std::vector<std::string_view> expectedMembers = { "toggle", "inner" };
        std::vector<std::string_view> presentMembers;

        Reflection::ForEach(Reflection::Reflect<BranchedOne>().Members,
            [&presentMembers](const auto& member)
            {
                presentMembers.push_back(member.Name);
            }
        );

        TEST_EQ(presentMembers, expectedMembers);
    }

    // Enumerate member attributes.
    {
        std::vector<std::string_view> expectedAttributes;
        std::vector<std::string_view> presentAttributes;

        Reflection::ForEach(Reflection::Reflect<Base>().Member<0>().Attributes,
            [&presentAttributes](const auto& attribute)
            {
                presentAttributes.push_back(attribute.Name);
            }
        );

        TEST_EQ(presentAttributes, expectedAttributes);
    }

    {
        std::vector<std::string_view> expectedAttributes = { "ToggleOnAttribute", "ToggleOffAttribute" };
        std::vector<std::string_view> presentAttributes;

        Reflection::ForEach(Reflection::Reflect<BranchedOne>().Member<0>().Attributes,
            [&presentAttributes](const auto& attribute)
            {
                presentAttributes.push_back(attribute.Name);
            }
        );

        TEST_EQ(presentAttributes, expectedAttributes);
    }

    // Enumerate derived types from type.
    // TODO

    // Test reflection between compilation units.
    // TODO

    return true;
}

constexpr char BaseMemberName[] = "textWithoutAttribute";
constexpr char DerivedAttributeName[] = "DerivedAttribute";
constexpr char DerivedMemberName[] = "counter";
constexpr char DerivedMemberAttributeName[] = "CounterAttribute";

bool TestExperimental()
{
    // Find type members and attributes by name.
    // String literal needs to be a static variable passed via template argument to
    // be allowed in constexpr evaluation. This limitation will be lifted in C++20.
    // Disabled for now because of issues with compilation on Clang (GCC/MSVC is fine).
    /*
    TEST_EQ(Reflection::Reflect<Base>().FindMember<BaseMemberName>().Name, "textWithoutAttribute");
    TEST_EQ(Reflection::Reflect<Derived>().FindAttribute<DerivedAttributeName>().Name, "DerivedAttribute");
    TEST_EQ(Reflection::Reflect<Derived>().FindAttribute<DerivedAttributeName>().Instance.state, false);
    TEST_EQ(Reflection::Reflect<Derived>().FindMember<DerivedMemberName>().FindAttribute<DerivedMemberAttributeName>().Instance.state, true);
    */

    return true;
}

bool TestCreate()
{
    // TODO: Instantiate derived using its type identifier.
    // Base* instance = Base::create(Derived::Type);
    // TEST_EQ(GetType(instance) == Derived::Type);

    return true;
}

bool TestCast()
{
    // TODO: Test casting from one type to another.

    return true;
}

bool TestSuper()
{
    // TODO: Test super typedef.

    return true;
}

int main()
{
    TEST_RUN(TestTypes);
    TEST_RUN(TestExperimental);
    TEST_RUN(TestCreate);
    TEST_RUN(TestCast);
    TEST_RUN(TestSuper);
}
