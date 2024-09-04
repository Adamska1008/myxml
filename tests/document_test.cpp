#include <catch2/catch_test_macros.hpp>
#include "myxml/document.hpp"

TEST_CASE("Simple document", "[document]")
{
    SECTION("No decl")
    {
        std::string input = R"(<root>
    <child>Value</child>
</root>)";
        auto doc = myxml::document::parse(input);
        REQUIRE(doc.get_root()->GetName() == "root");
        REQUIRE(doc.first_elem("child")->GetName() == "child");
        REQUIRE(doc.first_elem("child")->FirstText()->ExportRaw() == "Value");
    }

    SECTION("With decl")
    {
        std::string input = R"(<?xml version="1.0" encoding="UTF-8"?>
<root>
    <child>Value</child>
</root>
)";
        auto doc = myxml::document::parse(input);
        REQUIRE(doc.get_declaration().version == "1.0");
        REQUIRE(doc.get_declaration().encoding == "UTF-8");
    }
}

TEST_CASE("Custom String Literal", "[document]")
{
    using namespace myxml::literals;
    auto doc = "<root></root>"_doc;
    REQUIRE(doc.get_root()->GetName() == "root");
}