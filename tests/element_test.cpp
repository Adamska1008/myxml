#include <catch2/catch_test_macros.hpp>
#include "myxml/element.hpp"

TEST_CASE("Element Impl", "[element]")
{
    using namespace myxml;

    auto root = element_impl::_new("root");
    auto child = element_impl::_new("child");
    auto sibiling = element_impl::_new("sibiling");

    SECTION("GetName")
    {
        REQUIRE(root->name == "root");
    }

    SECTION("Basic Insertion")
    {
        root->push_front(child);
        REQUIRE(root->first<element_impl>()->name == "child");
        REQUIRE(root->last_child()->as<myxml::element_impl>()->name == "child");
    }

    SECTION("Get child by name after insert it")
    {
        root->push_front(child);
        // Unbuffered
        REQUIRE(root->first_elem("child")->name == "child");
        // Buffered
        REQUIRE(root->first_elem("child")->name == "child");
    }

    SECTION("Multi child")
    {
        root->push_back(child);
        root->push_back(sibiling);
        REQUIRE(root->first_elem("child")->name == "child");
        REQUIRE(root->first_elem("child")->next<element_impl>()->name == "sibiling");
        REQUIRE(root->first_elem("sibiling")->prev<element_impl>()->name == "child");
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