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
    class composite_node;

    // Element, Text are Node.
    class node : public std::enable_shared_from_this<node>, public printable
    {
    public:
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<node, T>>>
        std::shared_ptr<T> next()
        {
            for (auto it = this->next_sibiling(); it != nullptr; it = it->next_sibiling())
            {
                if (auto cast = it->as<T>(); cast)
                {
                    return cast;
                }
            }
            return nullptr;
        }

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<node, T>>>
        std::shared_ptr<T> prev()
        {
            for (auto it = this->prev_sibiling(); it != nullptr; it = it->prev_sibiling())
            {
                if (auto cast = it->as<T>(); cast)
                {
                    return cast;
                }
            }
            return nullptr;
        }

        virtual ~node() = default;
        std::shared_ptr<composite_node> _parent;
        std::shared_ptr<node> _prev;
        std::shared_ptr<node> _next;

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<node, T>>>
        std::shared_ptr<T> as()
        {
            return std::dynamic_pointer_cast<T>(this->shared_from_this());
        }

        /* Query */
        std::shared_ptr<node> next_sibiling();
        std::shared_ptr<node> prev_sibiling();

        /** Implement Export */
        virtual void entity_encoding(bool) override;
        virtual void platform_specific_newline(bool) override;
    };

    // Element are Composite Node.
    class composite_node : public node
    {
    private:
        std::shared_ptr<node> firstChild;
        std::shared_ptr<node> lastChild;
        std::map<std::string, std::weak_ptr<element_impl>, std::less<>> nameToElemBuffer;

    public:
        virtual ~composite_node() = default;

        /* Query */
        std::shared_ptr<node> first_child();
        const std::shared_ptr<node> &first_child() const;
        std::shared_ptr<node> last_child();
        const std::shared_ptr<node> &last_child() const;
        std::shared_ptr<element_impl> first_elem(std::string_view name);

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<node, T>>>
        std::shared_ptr<T> first()
        {
            for (auto it = this->first_child(); it != nullptr; it = it->next_sibiling())
            {
                if (auto cast = it->as<T>(); cast)
                {
                    return cast;
                }
            }
            return nullptr;
        }

        /* Manipulate */
        std::shared_ptr<node> push_front(const std::shared_ptr<node> &);
        std::shared_ptr<node> push_back(const std::shared_ptr<node> &);
        void unlink(const std::shared_ptr<node> &);

        /** Implement Export */
        virtual void entity_encoding(bool) override;
        virtual void platform_specific_newline(bool) override;
    };
}
