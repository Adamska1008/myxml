#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "parser.hpp"

TEST_CASE("Parsing tag", "parser")
{
    std::string open = "<tag>";
    auto tag = myxml::Parser(open).ParseTag().value();
    REQUIRE(tag.name == "tag");
    REQUIRE(tag.type == myxml::Tag::ClosingType::Open);

    std::string closed = "<tag/>";
    tag = myxml::Parser(closed).ParseTag().value();
    REQUIRE(tag.name == "tag");
    REQUIRE(tag.type == myxml::Tag::ClosingType::Closed);

    std::string closing = "</tag>";
    tag = myxml::Parser(closing).ParseTag().value();
    REQUIRE(tag.name == "tag");
    REQUIRE(tag.type == myxml::Tag::ClosingType::Closing);
}

TEST_CASE("Parsing simple xml strings", "[parser]")
{
    SECTION("Basic")
    {
        std::string tooEasy = R"(<root>
</root>)";
        auto elem = myxml::Element::Parse(tooEasy);
        REQUIRE(elem->GetName() == "root");
    }

    SECTION("Text")
    {
        std::string withText = R"(<root>
    <child>Hello, world!</child>
</root>)";
        auto elem = myxml::Element::Parse(withText);
        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->FirstChild()->AsText().value()->Export() == "\n    ");
        auto child = elem->FirstChild()->next->AsElement().value();
        REQUIRE(child->GetName() == "child");
        REQUIRE(child->FirstChild()->AsText().value()->Export() == "Hello, world!");
        REQUIRE(child->next->AsText().value()->Export() == "\n");
    }

    SECTION("Nested")
    {
        std::string nested = R"(<root>
    <parent>
        <child></child>
    </parent>
</root>)";
        auto elem = myxml::Element::Parse(nested);
        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->FirstChild()->AsText().value()->Export() == "\n    ");

        auto parent = elem->FirstChild()->next->AsElement().value();
        REQUIRE(parent->GetName() == "parent");
        REQUIRE(parent->FirstChild()->AsText().value()->Export() == "\n        ");

        auto child = parent->FirstChild()->next->AsElement().value();
        REQUIRE(child->GetName() == "child");
        REQUIRE(child->FirstChild() == nullptr); // 因为 <child></child> 是空的，没有文本子节点

        REQUIRE(parent->FirstChild()->next->next->AsText().value()->Export() == "\n    ");
        REQUIRE(elem->FirstChild()->next->next->AsText().value()->Export() == "\n");
    }

    SECTION("Mutli-Level")
    {
        std::string multiLevel = R"(<root>
    <item>First</item>
    <item>Second</item>
    <item>Third</item>
</root>)";
        auto elem = myxml::Element::Parse(multiLevel);
        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->FirstChild()->AsText().value()->Export() == "\n    ");

        // 验证第一个 <item> 节点
        auto item1 = elem->FirstChild()->next->AsElement().value();
        REQUIRE(item1->GetName() == "item");
        REQUIRE(item1->FirstChild()->AsText().value()->Export() == "First");

        // 验证第二个 <item> 节点
        auto item2 = item1->next->next->AsElement().value();
        REQUIRE(item2->GetName() == "item");
        REQUIRE(item2->FirstChild()->AsText().value()->Export() == "Second");

        // 验证第三个 <item> 节点
        auto item3 = item2->next->next->AsElement().value();
        REQUIRE(item3->GetName() == "item");
        REQUIRE(item3->FirstChild()->AsText().value()->Export() == "Third");

        // 验证 root 节点的最后文本
        REQUIRE(item3->next->AsText().value()->Export() == "\n");
    }

    SECTION("Closed Element")
    {
        std::string closed = R"(<root>
    <empty />
</root>)";
        auto elem = myxml::Element::Parse(closed);
        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->FirstChild()->AsText().value()->Export() == "\n    ");

        // 验证 <empty /> 节点
        auto emptyElement = elem->FirstChild()->next->AsElement().value();
        REQUIRE(emptyElement->GetName() == "empty");
        REQUIRE(emptyElement->FirstChild() == nullptr); // 自闭合标签没有子节点

        // 验证 root 节点的最后文本
        REQUIRE(emptyElement->next->AsText().value()->Export() == "\n");
    }

    SECTION("Mixed")
    {
        std::string mixed = R"(<root>
    hello
    <child></child>
</root>)";
        auto elem = myxml::Element::Parse(mixed);
        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->FirstChild()->AsText().value()->Export() == "\n    hello\n    ");

        // 验证 <child> 节点
        auto child = elem->FirstChild()->next->AsElement().value();
        REQUIRE(child->GetName() == "child");
        REQUIRE(child->FirstChild() == nullptr); // <child></child> 是空的，没有文本子节点

        // 验证 root 节点的最后文本
        REQUIRE(child->next->AsText().value()->Export() == "\n");
    }
}