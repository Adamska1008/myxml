#pragma once
#include <memory>
#include <optional>
#include <map>

#include "myxml/printable.hpp"

namespace myxml
{
    // defined in element.hpp
    class element_impl;
    // defined in text.hpp
    class text_impl;
    // defined below
    class CompositeNode;

    // Element, Text are Node.
    class Node : public std::enable_shared_from_this<Node>, public printable
    {
    private:
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<Node, T>>>
        std::shared_ptr<T> Next()
        {
            for (auto it = this->NextSibiling(); it != nullptr; it = it->NextSibiling())
            {
                if (auto cast = it->As<T>(); cast)
                {
                    return cast;
                }
            }
            return nullptr;
        }

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<Node, T>>>
        std::shared_ptr<T> Prev()
        {
            for (auto it = this->PrevSibiling(); it != nullptr; it = it->PrevSibiling())
            {
                if (auto cast = it->As<T>(); cast)
                {
                    return cast;
                }
            }
            return nullptr;
        }

    public:
        virtual ~Node() = default;
        std::shared_ptr<CompositeNode> parent;
        std::shared_ptr<Node> prev;
        std::shared_ptr<Node> next;

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<Node, T>>>
        std::shared_ptr<T> As()
        {
            return std::dynamic_pointer_cast<T>(this->shared_from_this());
        }

        /* Query */
        std::shared_ptr<Node> NextSibiling();
        std::shared_ptr<Node> PrevSibiling();
        std::shared_ptr<element_impl> NextElem();
        std::shared_ptr<element_impl> PrevElem();
        std::shared_ptr<text_impl> NextText();
        std::shared_ptr<text_impl> PrevText();

        /** Implement Export */
        virtual void entity_encoding(bool) override;
        virtual void platform_specific_newline(bool) override;
    };

    // Element are Composite Node.
    class CompositeNode : public Node
    {
    private:
        std::shared_ptr<Node> firstChild;
        std::shared_ptr<Node> lastChild;
        std::map<std::string, std::weak_ptr<element_impl>, std::less<>> nameToElemBuffer;

    public:
        virtual ~CompositeNode() = default;

        /* Query */
        std::shared_ptr<Node> FirstChild();
        const std::shared_ptr<Node> &FirstChild() const;
        std::shared_ptr<Node> LastChild();
        const std::shared_ptr<Node> &LastChild() const;
        std::shared_ptr<element_impl> Elem(std::string_view name);
        std::shared_ptr<element_impl> FirstElem();
        std::shared_ptr<text_impl> FirstText();

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<Node, T>>>
        std::shared_ptr<T> First()
        {
            for (auto it = this->FirstChild(); it != nullptr; it = it->NextSibiling())
            {
                if (auto cast = it->As<T>(); cast)
                {
                    return cast;
                }
            }
            return nullptr;
        }

        /* Manipulate */
        std::shared_ptr<Node> InsertAtFront(const std::shared_ptr<Node> &);
        std::shared_ptr<Node> InsertAtEnd(const std::shared_ptr<Node> &);
        void Unlink(const std::shared_ptr<Node> &);

        /** Implement Export */
        virtual void entity_encoding(bool) override;
        virtual void platform_specific_newline(bool) override;
    };
}
