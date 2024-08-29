#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "myxml/element.hpp"

TEST_CASE("Formatted Export", "[exportable]")
{
    SECTION("Single")
    {
        auto root = myxml::Element::New("root");
        REQUIRE(root->ExportFormatted() == "<root />\n");
    }

    SECTION("Text")
    {
        auto root = myxml::Element::Parse("<root>Hello, world!</root>");
        REQUIRE(root->ExportFormatted() == "<root>\n    Hello, world!\n</root>\n");
    }
}

TEST_CASE("Raw Export", "[exportable]")
{
    SECTION("Single")
    {
        auto root = myxml::Element::Parse("<root />");
        REQUIRE(root->ExportRaw() == "<root />");
    }

    SECTION("Text")
    {
        auto root = myxml::Element::Parse("<root>Hello, world!</root>");
        REQUIRE(root->ExportRaw() == "<root>Hello, world!</root>");
    }
}