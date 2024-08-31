#include <catch2/catch_test_macros.hpp>
#include "myxml/buffer.hpp"
#include "myxml/xmlfile.hpp"

TEST_CASE("String Buffer", "[buffer]")
{
    SECTION("Simple String Buffer")
    {
        std::string str = "Hello, world!";
        myxml::StringBuffer sb = std::move(str);
        REQUIRE(sb.Peek() == 'H');
        REQUIRE(sb.PeekN(3) == "Hel");
        REQUIRE(sb.TakeN(3) == "Hel");
        REQUIRE(sb.AfterN(1) == 'o');
        REQUIRE(sb.AfterNM(1, 2) == "o,");
    }
}