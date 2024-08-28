#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "parser.hpp"

TEST_CASE("Parsing tag", "parser")
{
    std::string open = "<tag>";
    auto tag = myxml::Parser(open).ParseTag().value();
    REQUIRE(tag.name == "tag");
    REQUIRE(tag.type == myxml::ElementTag::ClosingType::Open);

    std::string closed = "<tag/>";
    tag = myxml::Parser(closed).ParseTag().value();
    REQUIRE(tag.name == "tag");
    REQUIRE(tag.type == myxml::ElementTag::ClosingType::Closed);

    std::string closing = "</tag>";
    tag = myxml::Parser(closing).ParseTag().value();
    REQUIRE(tag.name == "tag");
    REQUIRE(tag.type == myxml::ElementTag::ClosingType::Closing);
}

TEST_CASE("Parsing simple xml elements", "[parser]")
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
        REQUIRE(elem->FirstChild()->AsText().value()->ExportRaw() == "\n    ");
        auto child = elem->FirstChild()->next->AsElement().value();
        REQUIRE(child->GetName() == "child");
        REQUIRE(child->FirstChild()->AsText().value()->ExportRaw() == "Hello, world!");
        REQUIRE(child->next->AsText().value()->ExportRaw() == "\n");
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
        REQUIRE(elem->FirstChild()->AsText().value()->ExportRaw() == "\n    ");

        auto parent = elem->FirstChild()->next->AsElement().value();
        REQUIRE(parent->GetName() == "parent");
        REQUIRE(parent->FirstChild()->AsText().value()->ExportRaw() == "\n        ");

        auto child = parent->FirstChild()->next->AsElement().value();
        REQUIRE(child->GetName() == "child");
        REQUIRE(child->FirstChild() == nullptr); // 因为 <child></child> 是空的，没有文本子节点

        REQUIRE(parent->FirstChild()->next->next->AsText().value()->ExportRaw() == "\n    ");
        REQUIRE(elem->FirstChild()->next->next->AsText().value()->ExportRaw() == "\n");
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
        REQUIRE(elem->FirstChild()->AsText().value()->ExportRaw() == "\n    ");

        // 验证第一个 <item> 节点
        auto item1 = elem->FirstChild()->next->AsElement().value();
        REQUIRE(item1->GetName() == "item");
        REQUIRE(item1->FirstChild()->AsText().value()->ExportRaw() == "First");

        // 验证第二个 <item> 节点
        auto item2 = item1->next->next->AsElement().value();
        REQUIRE(item2->GetName() == "item");
        REQUIRE(item2->FirstChild()->AsText().value()->ExportRaw() == "Second");

        // 验证第三个 <item> 节点
        auto item3 = item2->next->next->AsElement().value();
        REQUIRE(item3->GetName() == "item");
        REQUIRE(item3->FirstChild()->AsText().value()->ExportRaw() == "Third");

        // 验证 root 节点的最后文本
        REQUIRE(item3->next->AsText().value()->ExportRaw() == "\n");
    }

    SECTION("Closed Element")
    {
        std::string closed = R"(<root>
    <empty />
</root>)";
        auto elem = myxml::Element::Parse(closed);
        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->FirstChild()->AsText().value()->ExportRaw() == "\n    ");

        // 验证 <empty /> 节点
        auto emptyElement = elem->FirstChild()->next->AsElement().value();
        REQUIRE(emptyElement->GetName() == "empty");
        REQUIRE(emptyElement->FirstChild() == nullptr); // 自闭合标签没有子节点

        // 验证 root 节点的最后文本
        REQUIRE(emptyElement->next->AsText().value()->ExportRaw() == "\n");
    }

    SECTION("Mixed")
    {
        std::string mixed = R"(<root>
    hello
    <child></child>
</root>)";
        auto elem = myxml::Element::Parse(mixed);
        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->FirstChild()->AsText().value()->ExportRaw() == "\n    hello\n    ");

        // 验证 <child> 节点
        auto child = elem->FirstChild()->next->AsElement().value();
        REQUIRE(child->GetName() == "child");
        REQUIRE(child->FirstChild() == nullptr); // <child></child> 是空的，没有文本子节点

        // 验证 root 节点的最后文本
        REQUIRE(child->next->AsText().value()->ExportRaw() == "\n");
    }

    SECTION("With attributes")
    {
        std::string attri = R"(<root hello="world"></root>)";
        auto elem = myxml::Element::Parse(attri);
        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->GetAttribute("hello") == "world");
    }

    SECTION("Empty With Attributes")
    {
        std::string attri = R"(<root hello="world"/>)";
        auto elem = myxml::Element::Parse(attri);
        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->GetAttribute("hello") == "world");
    }

    SECTION("Multiple Attributes")
    {
        std::string multipleAttributes = R"(<root attr1="value1" attr2="value2" attr3="value3"></root>)";
        auto elem = myxml::Element::Parse(multipleAttributes);

        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->GetAttribute("attr1") == "value1");
        REQUIRE(elem->GetAttribute("attr2") == "value2");
        REQUIRE(elem->GetAttribute("attr3") == "value3");
    }

    SECTION("Empty Attributes")
    {
        std::string emptyAttribute = R"(<root attr=""></root>)";
        auto elem = myxml::Element::Parse(emptyAttribute);
        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->GetAttribute("attr") == "");
    }

    SECTION("Attributes And Children")
    {
        std::string attributesAndChildren = R"(<root attr="value"><child>Text</child></root>)";
        auto elem = myxml::Element::Parse(attributesAndChildren);

        // 检查根元素的名称和属性
        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->GetAttribute("attr") == "value");

        // 检查根元素的第一个子元素
        auto child = elem->FirstChild()->AsElement().value();
        REQUIRE(child->GetName() == "child");

        // 检查子元素的文本内容
        REQUIRE(child->FirstChild()->AsText().value()->ExportRaw() == "Text");
    }

    SECTION("Decoding entity")
    {
        std::string root = R"(<root>
    &lt;&gt;
</root>)";
        auto elem = myxml::Element::Parse(root);

        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->FirstChild()->AsText().value()->ExportRaw() == "\n    <>\n");
    }
}