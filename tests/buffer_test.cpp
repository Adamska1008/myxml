#include <catch2/catch_test_macros.hpp>
#include "myxml/buffer.hpp"
#include "myxml/xmlfile.hpp"

TEST_CASE("String Buffer", "[buffer]")
{
    SECTION("Simple String Buffer")
    {
        myxml::StringBuffer sb = "Hello, world!";
        REQUIRE(sb.Peek() == 'H');
        REQUIRE(sb.PeekN(3) == "Hel");
        REQUIRE(sb.TakeN(3) == "Hel");
        REQUIRE(sb.AfterN(1) == 'o');
        REQUIRE(sb.AfterNM(1, 2) == "o,");
    }

    SECTION("Location", "[buffer]")
    {
        myxml::StringBuffer sb = "Hello, world!\nLine2";

        REQUIRE(sb.TakeN(3) == "Hel");
        REQUIRE(sb.CurrentLocation() == std::make_tuple(0, 3));
        REQUIRE(sb.TakeN(4) == "lo, ");
        REQUIRE(sb.CurrentLocation() == std::make_tuple(0, 7));
        REQUIRE(sb.TakeN(7) == "world!\n");
        REQUIRE(sb.CurrentLocation() == std::make_tuple(1, 0));
        REQUIRE(sb.TakeN(3) == "Lin");
        REQUIRE(sb.CurrentLocation() == std::make_tuple(1, 3));
    }
}