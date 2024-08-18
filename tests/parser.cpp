#include <catch2/catch_test_macros.hpp>

#include "parser.hpp"

TEST_CASE("Parsing elements name", "[parser]")
{
    
}

TEST_CASE("Parsing a simple xml string", "[parser]")
{
    std::string tooEasy = "<root></root>";
    auto elem = myxml::Parser(tooEasy).ParseElement();
    REQUIRE(elem != std::nullopt);
}