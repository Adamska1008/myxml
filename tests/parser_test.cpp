#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "parser.hpp"

TEST_CASE("Parsing tag", "parser")
{
    std::string open = "<tag>";
    auto tag = myxml::Parser(open).ParseTag().value();
    REQUIRE(tag.name == "tag");
    REQUIRE(tag.type == myxml::Tag::ClosingType::Open);

    std::string closed = "<tag/>";
    tag = myxml::Parser(closed).ParseTag().value();
    REQUIRE(tag.name == "tag");
    REQUIRE(tag.type == myxml::Tag::ClosingType::Closed);

    std::string closing = "</tag>";
    tag = myxml::Parser(closing).ParseTag().value();
    REQUIRE(tag.name == "tag");
    REQUIRE(tag.type == myxml::Tag::ClosingType::Closing);
}

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
        // REQUIRE(elem->FirstChild().value().Export() == "Hello");
        {
            auto text = std::dynamic_pointer_cast<myxml::Text>(elem->FirstChild());
            REQUIRE(text->Export() == "Hello");
        }
    }

    SECTION("Nested")
    {
        std::string nested = "<root><child>hello</child></root>";
        auto elem = myxml::Element::Parse(nested);
        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->Elem("child")->GetName() == "child");
        REQUIRE(elem->Elem("child")->FirstChild()->AsText().value()->Export() == "hello");
    }

    SECTION("Mixed")
    {
        std::string mixed = "<root>hello<child></child></root>";
        auto elem = myxml::Element::Parse(mixed);
        REQUIRE(elem->GetName() == "root");
        REQUIRE(elem->Elem("child")->GetName() == "child");
        auto type = elem->FirstChild()->Type();
        auto text = elem->FirstChild()->AsElement();
        // REQUIRE(text.value()->Export() == "hello");
    }
}