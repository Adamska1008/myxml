#include <catch2/catch_test_macros.hpp>
#include "myxml/document.hpp"

TEST_CASE("Simple document", "[document]")
{
    SECTION("No decl")
    {
        std::string input = R"(<root>
    <child>Value</child>
</root>)";
        auto doc = myxml::Document::Parse(input);
        REQUIRE(doc->GetRoot()->GetName() == "root");
        REQUIRE(doc->GetRoot()->Elem("child")->GetName() == "child");
        REQUIRE(doc->GetRoot()->Elem("child")->FirstChild()->As<myxml::Text>().value()->ExportRaw() == "Value");
    }

    SECTION("With decl")
    {
        std::string input = R"(<?xml version="1.0" encoding="UTF-8"?>
<root>
    <child>Value</child>
</root>
)";
        auto doc = myxml::Document::Parse(input);
        REQUIRE(doc->GetDeclartion().version == "1.0");
        REQUIRE(doc->GetDeclartion().encoding == "UTF-8");
    }
}