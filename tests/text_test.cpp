#include <catch2/catch_test_macros.hpp>
#include "myxml/text.hpp"

TEST_CASE("text interface", "[text]")
{
    using namespace myxml;

    SECTION("trim")
    {
        text text("  \n\n Hello  \n \t ");
        REQUIRE(text.trimmed() == "Hello");
        REQUIRE(text.trim().str() == "Hello");
    }
}