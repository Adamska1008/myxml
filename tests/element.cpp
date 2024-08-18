#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "element.hpp"

TEST_CASE("Element Functionality", "[element]")
{
    auto root = myxml::Element::New("root");
    auto child = myxml::Element::New("child");
    auto sibiling = myxml::Element::New("sibiling");

    SECTION("GetName")
    {
        REQUIRE(root->GetName() == "root");
    }

    SECTION("Basic Insertion")
    {
        root->InsertAtFront(child);
        REQUIRE(root->FirstChild()->GetName() == "child");
    }

    SECTION("Get child by name after insert it")
    {
        root->InsertAtFront(child);
        // Unbuffered
        REQUIRE(root->Child("child")->GetName() == "child");
        // Buffered
        REQUIRE(root->Child("child")->GetName() == "child");
    }

    SECTION("Multi child")
    {
        root->InsertAtFront(child);
        root->InsertAtEnd(sibiling);
        REQUIRE(root->Child("child")->GetName() == "child");
        REQUIRE(root->Child("child")->NextSibiling()->GetName() == "sibiling");
    }
}