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
        REQUIRE(doc.first_elem("child").name() == "child");
        REQUIRE(doc.first_elem("child").first_text().str() == "Value");
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