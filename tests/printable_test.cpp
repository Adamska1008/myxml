#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "myxml/element.hpp"

TEST_CASE("Print raw", "[printable]")
{
    auto root = myxml::element::parse("<root />");
    REQUIRE(root.str() == "<root />");

    root = myxml::element::parse("<root>Hello, world!</root>");
    REQUIRE(root.str() == "<root>Hello, world!</root>");
}

TEST_CASE("Set Config", "[printable]")
{
    using namespace myxml::literals;
    auto root = "<root>Hell&lt;o, world!</root>"_elem;
    root.entity_encoding(false);
    REQUIRE(root.str() == "<root>Hell<o, world!</root>");
    root.entity_encoding(true);
    REQUIRE(root.str() == "<root>Hell&lt;o, world!</root>");
}
