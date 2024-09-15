#include <catch2/catch_test_macros.hpp>
#include "myxml/buffer.hpp"
#include "myxml/xmlfile.hpp"
#include <iostream>

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
        REQUIRE(sb.peek_n(11) == std::nullopt);
        REQUIRE(sb.take_n(11) == std::nullopt);
        REQUIRE(sb.take_n(10) == "lo, world!");
        REQUIRE(sb.take() == std::nullopt);
        REQUIRE(sb.take_n(2) == std::nullopt);
        REQUIRE(sb.peek() == std::nullopt);
        REQUIRE(sb.peek_n(2) == std::nullopt);
    }

    SECTION("Take test")
    {
        myxml::string_buffer sb = "Hello";
        REQUIRE(sb.take() == 'H');
        REQUIRE(sb.take() == 'e');
        REQUIRE(sb.take() == 'l');
        REQUIRE(sb.take() == 'l');
        REQUIRE(sb.take() == 'o');
        REQUIRE(sb.take() == std::nullopt);
    }

    SECTION("Location")
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