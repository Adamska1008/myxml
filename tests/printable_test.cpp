#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "myxml/element.hpp"

// TEST_CASE("Formatted Export", "[exportable]")
// {
//     SECTION("Single")
//     {
//         auto root = myxml::element_impl::_new("root");
//         REQUIRE(root->str() == "<root />\n");
//     }

//     SECTION("Text")
//     {
//         auto root = myxml::element_impl::parse("<root>Hello, world!</root>");
//         REQUIRE(root->str() == "<root>\n    Hello, world!\n</root>\n");
//     }
// }

TEST_CASE("Raw Export", "[exportable]")
{
    auto root = myxml::element_impl::parse("<root />");
    REQUIRE(root->str() == "<root />");

    root = myxml::element_impl::parse("<root>Hello, world!</root>");
    REQUIRE(root->str() == "<root>Hello, world!</root>");
}