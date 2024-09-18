#include <catch2/catch_test_macros.hpp>
#include "myxml/document.hpp"

TEST_CASE("Simple document", "[document]")
{
    using namespace myxml;
    SECTION("No decl")
    {
        std::string input = R"(<root>
    <child>Value</child>
</root>)";
        auto doc = document::parse(input);
        REQUIRE(doc.root().name() == "root");
        REQUIRE(doc.first_elem("child").value().name() == "child");
        REQUIRE(doc.first_elem().value().name() == "child");
        REQUIRE(doc.first_text().value().str() == "\n    ");
        REQUIRE(doc.first_elem("child").value().first_text().value().str() == "Value");
    }

    SECTION("Manipulate")
    {
        using namespace myxml::literals;
        auto doc = "<root/>"_doc;
        auto child = "<child></child>"_elem;
        doc.push_front(child);
        REQUIRE(doc.first_elem().value().name() == "child");
        doc.pop_front();
        REQUIRE(doc.first_elem() == std::nullopt);
        doc.push_front(child);
        doc.remove_first_element("child");
        REQUIRE(doc.first_elem() == std::nullopt);
        doc.push_front(child);
        doc.pop_back();
        REQUIRE(doc.first_elem() == std::nullopt);
        auto cd = cdata("Hello");
        doc.push_back(cd);
        REQUIRE(doc.first_cdata().value().str() == "<![CDATA[Hello]]>\n");
    }

    SECTION("With decl")
    {
        std::string input = R"(<?xml version="1.0" encoding="UTF-8"?>
<root>
    <child>Value</child>
</root>
)";
        auto doc = document::parse(input);
        REQUIRE(doc.get_declaration().version == "1.0");
        REQUIRE(doc.get_declaration().encoding == "UTF-8");
    }
}

TEST_CASE("Custom String Literal", "[document]")
{
    using namespace myxml::literals;
    auto doc = "<root></root>"_doc;
    REQUIRE(doc.root().name() == "root");
}