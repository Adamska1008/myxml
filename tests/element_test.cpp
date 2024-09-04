#include <catch2/catch_test_macros.hpp>
#include "myxml/element.hpp"

TEST_CASE("Element Impl", "[element]")
{
    auto root = myxml::element_impl::_new("root");
    auto child = myxml::element_impl::_new("child");
    auto sibiling = myxml::element_impl::_new("sibiling");

    SECTION("GetName")
    {
        REQUIRE(root->name == "root");
    }

    SECTION("Basic Insertion")
    {
        root->InsertAtFront(child);
        REQUIRE(root->FirstElem()->name == "child");
        REQUIRE(root->LastChild()->As<myxml::element_impl>()->name == "child");
    }

    SECTION("Get child by name after insert it")
    {
        root->InsertAtFront(child);
        // Unbuffered
        REQUIRE(root->Elem("child")->name == "child");
        // Buffered
        REQUIRE(root->Elem("child")->name == "child");
    }

    SECTION("Multi child")
    {
        root->InsertAtEnd(child);
        root->InsertAtEnd(sibiling);
        REQUIRE(root->Elem("child")->name == "child");
        REQUIRE(root->Elem("child")->NextElem()->name == "sibiling");
        REQUIRE(root->Elem("sibiling")->PrevElem()->name == "child");
    }

    SECTION("Overload []")
    {
        root->attributes["hello"] = "world";
        REQUIRE((*root)["hello"] == "world");
        REQUIRE(root->attributes["hello"] == "world");
        (*root)["hello"] = "bar";
        REQUIRE((*root)["hello"] == "bar");
    }
}

TEST_CASE("Custom String Literal", "[Element]")
{
    using namespace myxml::literals;
    auto elem = "<root></root>"_elem;
    REQUIRE(elem.name() == "root");
}