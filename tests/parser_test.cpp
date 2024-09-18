#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include "myxml/parser.hpp"
#include "myxml/xmlfile.hpp"

TEST_CASE("Parsing tag", "parser")
{
    std::string open = "<tag>";
    auto tag = myxml::parser(open).parse_element_tag();
    REQUIRE(tag->name == "tag");
    REQUIRE(tag->type == myxml::element_tag::closing_type::Open);

    std::string closed = "<tag/>";
    tag = myxml::parser(closed).parse_element_tag();
    REQUIRE(tag->name == "tag");
    REQUIRE(tag->type == myxml::element_tag::closing_type::Closed);

    std::string closing = "</tag>";
    tag = myxml::parser(closing).parse_element_tag();
    REQUIRE(tag->name == "tag");
    REQUIRE(tag->type == myxml::element_tag::closing_type::Closing);
}

TEST_CASE("Parsing simple xml elements", "[parser]")
{
    using namespace myxml;
    SECTION("Basic")
    {
        std::string tooEasy = R"(<root>
</root>)";
        auto elem = element::parse(tooEasy);
        REQUIRE(elem.name() == "root");
    }

    SECTION("Text")
    {
        std::string withText = R"(<root>
    <child>Hello, world!</child>
</root>)";
        auto elem = element_impl::parse(withText);
        REQUIRE(elem->_name == "root");
        REQUIRE(elem->first<text_impl>()->str() == "\n    ");
        REQUIRE(elem->first<element_impl>()->_name == "child");
        REQUIRE(elem->first<element_impl>()->first<text_impl>()->str() == "Hello, world!");
    }

    SECTION("Nested")
    {
        std::string nested = R"(<root>
    <parent>
        <child></child>
    </parent>
</root>)";
        auto elem = element_impl::parse(nested);
        REQUIRE(elem->_name == "root");
        REQUIRE(elem->first<text_impl>()->str() == "\n    ");
        auto parent = elem->first<element_impl>();
        REQUIRE(parent->_name == "parent");
        REQUIRE(parent->first<text_impl>()->str() == "\n        ");
        auto child = parent->first<element_impl>();
        REQUIRE(child->_name == "child");
        REQUIRE(child->first_child() == nullptr);
    }

    SECTION("Mutli-Level")
    {
        std::string multiLevel = R"(<root>
    <item>First</item>
    <item>Second</item>
    <item>Third</item>
</root>)";
        auto elem = element_impl::parse(multiLevel);
        REQUIRE(elem->_name == "root");
        REQUIRE(elem->first<text_impl>()->str() == "\n    ");

        auto item1 = elem->first<element_impl>()->as<element_impl>();
        REQUIRE(item1->_name == "item");
        REQUIRE(item1->first<text_impl>()->str() == "First");

        // 验证第二个 <item> 节点
        auto item2 = item1->next<element_impl>();
        REQUIRE(item2->_name == "item");
        REQUIRE(item2->first<text_impl>()->str() == "Second");

        // 验证第三个 <item> 节点
        auto item3 = item2->next<element_impl>();
        REQUIRE(item3->_name == "item");
        REQUIRE(item3->first<text_impl>()->str() == "Third");

        // 验证 root 节点的最后文本
        REQUIRE(item3->next<text_impl>()->str() == "\n");
    }

    SECTION("Closed Element")
    {
        std::string closed = R"(<root>
    <empty />
</root>)";
        auto elem = element_impl::parse(closed);
        REQUIRE(elem->_name == "root");
        REQUIRE(elem->first<text_impl>()->str() == "\n    ");

        // 验证 <empty /> 节点
        auto emptyElement = elem->first_child()->next<element_impl>();
        REQUIRE(emptyElement->_name == "empty");
        REQUIRE(emptyElement->first_child() == nullptr); // 自闭合标签没有子节点

        // 验证 root 节点的最后文本
        REQUIRE(emptyElement->next<text_impl>()->str() == "\n");
    }

    SECTION("Mixed")
    {
        std::string mixed = R"(<root>
    hello
    <child></child>
</root>)";
        auto elem = element_impl::parse(mixed);
        REQUIRE(elem->_name == "root");
        REQUIRE(elem->first<text_impl>()->str() == "\n    hello\n    ");

        // 验证 <child> 节点
        auto child = elem->first_child()->next<element_impl>();
        REQUIRE(child->_name == "child");
        REQUIRE(child->first_child() == nullptr); // <child></child> 是空的，没有文本子节点

        // 验证 root 节点的最后文本
        REQUIRE(child->next<text_impl>()->str() == "\n");
    }

    SECTION("With attributes")
    {
        std::string attri = R"(<root hello="world"></root>)";
        auto elem = element_impl::parse(attri);
        REQUIRE(elem->_name == "root");
        REQUIRE(elem->_attributes["hello"] == "world");
    }

    SECTION("Empty With Attributes")
    {
        std::string attri = R"(<root hello="world"/>)";
        auto elem = element_impl::parse(attri);
        REQUIRE(elem->_name == "root");
        REQUIRE(elem->_attributes["hello"] == "world");
    }

    SECTION("Multiple Attributes")
    {
        std::string multipleAttributes = R"(<root attr1="value1" attr2="value2" attr3="value3"></root>)";
        auto elem = element_impl::parse(multipleAttributes);

        REQUIRE(elem->_name == "root");
        REQUIRE(elem->_attributes["attr1"] == "value1");
        REQUIRE(elem->_attributes["attr2"] == "value2");
        REQUIRE(elem->_attributes["attr3"] == "value3");
    }

    SECTION("Empty Attributes")
    {
        std::string emptyAttribute = R"(<root attr=""></root>)";
        auto elem = element_impl::parse(emptyAttribute);
        REQUIRE(elem->_name == "root");
        REQUIRE(elem->_attributes["attr"] == "");
    }

    SECTION("Attributes And Children")
    {
        std::string attributesAndChildren = R"(<root attr="value"><child>Text</child></root>)";
        auto elem = element::parse(attributesAndChildren);

        // 检查根元素的名称和属性
        REQUIRE(elem.name() == "root");
        REQUIRE(elem["attr"] == "value");

        // 检查根元素的第一个子元素
        auto child = *elem.first_elem();
        REQUIRE(child.name() == "child");

        // 检查子元素的文本内容
        REQUIRE(child.first_text().value().str() == "Text");
    }

    SECTION("Decoding entity")
    {
        std::string root = R"(<root>
    &lt;&gt;
</root>)";
        auto elem = element_impl::parse(root);

        elem->entity_encoding(false);
        REQUIRE(elem->_name == "root");
        REQUIRE(elem->first<text_impl>()->str() == "\n    <>\n");

        elem->entity_encoding(true);
        REQUIRE(elem->first<text_impl>()->str() == "\n    &lt;&gt;\n");
    }

    SECTION("CDATA")
    {
        std::string cdata = "<root><![CDATA[Hello!]]></root>";
        auto elem = element_impl::parse(cdata);

        REQUIRE(elem->first_child()->as<myxml::cdata_impl>()->str() == "<![CDATA[Hello!]]>");
    }

    SECTION("Newline Normalization")
    {
        std::string nl = "<root>hello\r\n</root>";
        auto elem = element_impl::parse(nl);

        REQUIRE(elem->first<text_impl>()->str() == "hello\n");
        nl = "<root>hello\r</root>";

        elem = element_impl::parse(nl);
        REQUIRE(elem->first<text_impl>()->str() == "hello\n");
    }

    SECTION("Simple File Buffer")
    {
        auto doc = document::load("tests/data/example1.xml");
        auto root = doc.root();

        auto elem = element::load("tests/data/example1.xml");
        REQUIRE(elem.name() == "root");
    }
}
