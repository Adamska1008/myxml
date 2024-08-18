#include <catch2/catch_test_macros.hpp>

#include "parser.hpp"

TEST_CASE("Parsing simple xml strings", "[parser]")
{
    SECTION("Simple")
    {
        std::string tooEasy = "<root></root>";
        auto elem = myxml::Parser(tooEasy).ParseElement().value();
        REQUIRE(elem->GetName() == "root");
    }

    SECTION("text")
    {
        std::string withText = "<root>Hello</root>";
        auto elem = myxml::Parser(withText).ParseElement().value();
        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->GetText().value().Export() == "Hello");
    }
}