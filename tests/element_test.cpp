#include <catch2/catch_test_macros.hpp>
#include "myxml/element.hpp"

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
        root->InsertAtEnd(child);
        root->InsertAtEnd(sibiling);
        REQUIRE(root->Elem("child")->GetName() == "child");
        REQUIRE(root->Elem("child")->next->AsElement().value()->GetName() == "sibiling");
        REQUIRE(root->Elem("sibiling")->prev->AsElement().value()->GetName() == "child");
    }
}