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
        REQUIRE(root->FirstChild()->AsElement().value()->GetName() == "child");
        REQUIRE(root->LastChild()->AsElement().value()->GetName() == "child");
        REQUIRE(root->FirstChild()->AsElement().value()->GetName() == "root");
    }

    SECTION("Get child by name after insert it")
    {
        root->InsertAtFront(child);
        // Unbuffered
        REQUIRE(root->Elem("child")->GetName() == "child");
        // Buffered
        REQUIRE(root->Elem("child")->GetName() == "child");
    }

    SECTION("Multi child")
    {
        root->InsertAtFront(child);
        root->InsertAtEnd(sibiling);
        REQUIRE(root->Elem("child")->GetName() == "child");
        REQUIRE(root->Elem("child")->Next()->AsElement().value()->GetName() == "sibiling");
        REQUIRE(root->Elem("sibiling")->Prev()->AsElement().value()->GetName() == "child");
    }
}