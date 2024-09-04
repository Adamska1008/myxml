#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include "myxml/parser.hpp"
#include "myxml/xmlfile.hpp"

TEST_CASE("Parsing tag", "parser")
{
    std::string open = "<tag>";
    auto tag = myxml::Parser(open).ParseElementTag();
    REQUIRE(tag->name == "tag");
    REQUIRE(tag->type == myxml::ElementTag::ClosingType::Open);

    std::string closed = "<tag/>";
    tag = myxml::Parser(closed).ParseElementTag();
    REQUIRE(tag->name == "tag");
    REQUIRE(tag->type == myxml::ElementTag::ClosingType::Closed);

    std::string closing = "</tag>";
    tag = myxml::Parser(closing).ParseElementTag();
    REQUIRE(tag->name == "tag");
    REQUIRE(tag->type == myxml::ElementTag::ClosingType::Closing);
}

TEST_CASE("Parsing simple xml elements", "[parser]")
{
    SECTION("Basic")
    {
        std::string tooEasy = R"(<root>
</root>)";
        auto elem = myxml::element_impl::parse(tooEasy);
        REQUIRE(elem->name == "root");
    }

    SECTION("Text")
    {
        std::string withText = R"(<root>
    <child>Hello, world!</child>
</root>)";
        auto elem = myxml::element_impl::parse(withText);
        REQUIRE(elem->name == "root");
        REQUIRE(elem->FirstText()->str() == "\n    ");
        REQUIRE(elem->FirstElem()->name == "child");
        REQUIRE(elem->FirstElem()->FirstText()->str() == "Hello, world!");
    }

    SECTION("Nested")
    {
        std::string nested = R"(<root>
    <parent>
        <child></child>
    </parent>
</root>)";
        auto elem = myxml::element_impl::parse(nested);
        REQUIRE(elem->name == "root");
        REQUIRE(elem->FirstText()->str() == "\n    ");
        auto parent = elem->FirstElem();
        REQUIRE(parent->name == "parent");
        REQUIRE(parent->FirstText()->str() == "\n        ");
        auto child = parent->FirstElem();
        REQUIRE(child->name == "child");
        REQUIRE(child->FirstChild() == nullptr);
    }

    SECTION("Mutli-Level")
    {
        std::string multiLevel = R"(<root>
    <item>First</item>
    <item>Second</item>
    <item>Third</item>
</root>)";
        auto elem = myxml::element_impl::parse(multiLevel);
        REQUIRE(elem->name == "root");
        REQUIRE(elem->FirstText()->str() == "\n    ");

        auto item1 = elem->FirstElem()->As<myxml::element_impl>();
        REQUIRE(item1->name == "item");
        REQUIRE(item1->FirstText()->str() == "First");

        // 验证第二个 <item> 节点
        auto item2 = item1->NextElem();
        REQUIRE(item2->name == "item");
        REQUIRE(item2->FirstText()->str() == "Second");

        // 验证第三个 <item> 节点
        auto item3 = item2->NextElem();
        REQUIRE(item3->name == "item");
        REQUIRE(item3->FirstText()->str() == "Third");

        // 验证 root 节点的最后文本
        REQUIRE(item3->NextText()->str() == "\n");
    }

    SECTION("Closed Element")
    {
        std::string closed = R"(<root>
    <empty />
</root>)";
        auto elem = myxml::element_impl::parse(closed);
        REQUIRE(elem->name == "root");
        REQUIRE(elem->FirstText()->str() == "\n    ");

        // 验证 <empty /> 节点
        auto emptyElement = elem->FirstChild()->next->As<myxml::element_impl>();
        REQUIRE(emptyElement->name == "empty");
        REQUIRE(emptyElement->FirstChild() == nullptr); // 自闭合标签没有子节点

        // 验证 root 节点的最后文本
        REQUIRE(emptyElement->NextText()->str() == "\n");
    }

    SECTION("Mixed")
    {
        std::string mixed = R"(<root>
    hello
    <child></child>
</root>)";
        auto elem = myxml::element_impl::parse(mixed);
        REQUIRE(elem->name == "root");
        REQUIRE(elem->FirstText()->str() == "\n    hello\n    ");

        // 验证 <child> 节点
        auto child = elem->FirstChild()->next->As<myxml::element_impl>();
        REQUIRE(child->name == "child");
        REQUIRE(child->FirstChild() == nullptr); // <child></child> 是空的，没有文本子节点

        // 验证 root 节点的最后文本
        REQUIRE(child->NextText()->str() == "\n");
    }

    SECTION("With attributes")
    {
        std::string attri = R"(<root hello="world"></root>)";
        auto elem = myxml::element_impl::parse(attri);
        REQUIRE(elem->name == "root");
        REQUIRE(elem->attributes["hello"] == "world");
    }

    SECTION("Empty With Attributes")
    {
        std::string attri = R"(<root hello="world"/>)";
        auto elem = myxml::element_impl::parse(attri);
        REQUIRE(elem->name == "root");
        REQUIRE(elem->attributes["hello"] == "world");
    }

    SECTION("Multiple Attributes")
    {
        std::string multipleAttributes = R"(<root attr1="value1" attr2="value2" attr3="value3"></root>)";
        auto elem = myxml::element_impl::parse(multipleAttributes);

        REQUIRE(elem->name == "root");
        REQUIRE(elem->attributes["attr1"] == "value1");
        REQUIRE(elem->attributes["attr2"] == "value2");
        REQUIRE(elem->attributes["attr3"] == "value3");
    }

    SECTION("Empty Attributes")
    {
        std::string emptyAttribute = R"(<root attr=""></root>)";
        auto elem = myxml::element_impl::parse(emptyAttribute);
        REQUIRE(elem->name == "root");
        REQUIRE(elem->attributes["attr"] == "");
    }

    SECTION("Attributes And Children")
    {
        std::string attributesAndChildren = R"(<root attr="value"><child>Text</child></root>)";
        auto elem = myxml::element_impl::parse(attributesAndChildren);

        // 检查根元素的名称和属性
        REQUIRE(elem->name == "root");
        REQUIRE(elem->attributes["attr"] == "value");

        // 检查根元素的第一个子元素
        auto child = elem->FirstElem();
        REQUIRE(child->name == "child");

        // 检查子元素的文本内容
        REQUIRE(child->FirstText()->str() == "Text");
    }

    SECTION("Decoding entity")
    {
        std::string root = R"(<root>
    &lt;&gt;
</root>)";
        auto elem = myxml::element_impl::parse(root);

        elem->entity_encoding(false);
        REQUIRE(elem->name == "root");
        REQUIRE(elem->FirstText()->str() == "\n    <>\n");

        elem->entity_encoding(true);
        REQUIRE(elem->FirstText()->str() == "\n    &lt;&gt;\n");
    }

    SECTION("CDATA")
    {
        std::string cdata = "<root><![CDATA[Hello!]]></root>";
        auto elem = myxml::element_impl::parse(cdata);

        REQUIRE(elem->FirstChild()->As<myxml::CData>()->str() == "<![CDATA[Hello!]]>\n");
    }

    SECTION("Newline Normalization")
    {
        std::string nl = "<root>hello\r\n</root>";
        auto elem = myxml::element_impl::parse(nl);

        REQUIRE(elem->FirstText()->str() == "hello\n");
        nl = "<root>hello\r</root>";

        elem = myxml::element_impl::parse(nl);
        REQUIRE(elem->FirstText()->str() == "hello\n");
    }

    SECTION("Simple File Buffer")
    {
        std::cout << std::filesystem::current_path() << std::endl;
        auto doc = myxml::document::load("tests/data/example.xml");
    }
}