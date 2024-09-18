#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "myxml/element.hpp"
#include "myxml/document.hpp"

TEST_CASE("Print compacted", "[printable]")
{
    using namespace myxml::literals;
    auto root = myxml::element::parse("<root />");
    REQUIRE(root.str() == "<root />");

    root = myxml::element::parse("<root>Hello, world!</root>");
    REQUIRE(root.str() == "<root>Hello, world!</root>");

    auto doc = R"(<?xml version="1.0" encoding="UTF-8"?>
<root>
    <child>Value</child>
</root>
)"_doc;
    REQUIRE(doc.str() == R"(<?xml version="1.0" encoding="UTF-8"?><root><child>Value</child>)</root>)");
}

TEST_CASE("Set Config", "[printable]")
{
    using namespace myxml::literals;
    auto root = "<root hello=\"world\">Hell&lt;o, world!</root>"_doc;
    root.entity_encoding(false);
    REQUIRE(root.str() == "<root hello=\"world\">Hell<o, world!</root>");
    root.entity_encoding(true);
    REQUIRE(root.str() == "<root hello=\"world\">Hell&lt;o, world!</root>");
}
