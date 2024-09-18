#include <sstream>
#include <catch2/catch_test_macros.hpp>
#include "myxml/cdata.hpp"

TEST_CASE("Simple", "[cdata]")
{
    SECTION("Constructor")
    {
        myxml::cdata cdata1("It is a CDATA <");
        std::string view("It is also a CDATA");
        myxml::cdata cdata2(view);
        std::string owner("It is the last CDATA");
        myxml::cdata cdata3(std::move(owner));
    }

    SECTION("Print")
    {
        myxml::cdata cdata("It is a CDATA <");
        std::stringstream sstream;
        sstream << cdata;
        REQUIRE(sstream.str() == "<![CDATA[It is a CDATA <]]>");
    }
}