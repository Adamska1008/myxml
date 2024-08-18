#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "element.hpp"

TEST_CASE("Element Functionality", "[element]")
{
    auto root = myxml::Element::New("root");
    auto child = myxml::Element::New("child");

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
        REQUIRE(root->FirstChild("child")->GetName() == "child");
        // Buffered
        REQUIRE(root->FirstChild("child")->GetName() == "child");
    }
}