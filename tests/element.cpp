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
        REQUIRE(root->LastChild()->GetName() == "child");
        REQUIRE(root->FirstChild()->Parent()->GetName() == "root");
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
        REQUIRE(root->Child("sibiling")->PrevSibiling()->GetName() == "child");
    }

    SECTION("Moving insertion")
    {
        myxml::Element s = *sibiling;
        root->InsertAtFront(child);
        root->InsertAtFront(move(s));
        REQUIRE(root->Child("sibiling")->GetName() == "sibiling");
    }
}