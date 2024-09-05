// All example codes in document
#include <optional>
#include <catch2/catch_test_macros.hpp>
#include "myxml/document.hpp"
#include "myxml/element.hpp"

TEST_CASE("docs/examples", "[doc]")
{
    SECTION("1")
    {
        using namespace myxml;
        std::string xml = "<root/>";
        document doc = document::parse(xml);
        std::string path = "tests/data/example.xml";
        doc = document::load(path);
        // get root elem
        std::optional<element> elem = doc.root().first_elem();
        // or directly query elem in root
        elem = doc.first_elem();
        // or query by name
        elem = doc.first_elem("elem");
    }
}