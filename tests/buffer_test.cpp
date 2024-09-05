#include <catch2/catch_test_macros.hpp>
#include "myxml/buffer.hpp"
#include "myxml/xmlfile.hpp"

TEST_CASE("String Buffer", "[buffer]")
{
    SECTION("Simple String Buffer")
    {
        myxml::string_buffer sb = "Hello, world!";
        REQUIRE(sb.peek() == 'H');
        REQUIRE(sb.peek_n(3) == "Hel");
        REQUIRE(sb.take_n(3) == "Hel");
        REQUIRE(sb.after_n(1) == 'o');
        REQUIRE(sb.after_n_m(1, 2) == "o,");
    }

    SECTION("Location", "[buffer]")
    {
        myxml::string_buffer sb = "Hello, world!\nLine2";

        REQUIRE(sb.take_n(3) == "Hel");
        REQUIRE(sb.cur_loc() == std::make_tuple(0, 3));
        REQUIRE(sb.take_n(4) == "lo, ");
        REQUIRE(sb.cur_loc() == std::make_tuple(0, 7));
        REQUIRE(sb.take_n(7) == "world!\n");
        REQUIRE(sb.cur_loc() == std::make_tuple(1, 0));
        REQUIRE(sb.take_n(3) == "Lin");
        REQUIRE(sb.cur_loc() == std::make_tuple(1, 3));
    }
}