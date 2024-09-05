#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "myxml/element.hpp"

TEST_CASE("Raw Export", "[exportable]")
{
    auto root = myxml::element_impl::parse("<root />");
    REQUIRE(root->str() == "<root />");

    root = myxml::element_impl::parse("<root>Hello, world!</root>");
    REQUIRE(root->str() == "<root>Hello, world!</root>");
}