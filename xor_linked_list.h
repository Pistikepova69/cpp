#ifndef __XOR_LINKED_LIST__
#define __XOR_LINKED_LIST__

#include <cassert>
#include <concepts>
#include <initializer_list>
#include <type_traits>
#include <utility>

#include "iterator.h"
// #include "stack.h"
#include <cstdint>

namespace nig {

    template <typename T>
    class xor_linked_list;

    template <typename T, typename U>
    requires std::same_as<T, U> || std::same_as<T, std::remove_const_t<U>>
    class iterator_base<xor_linked_list<T>, U>;

    template <typename T>
    class xor_linked_list_node {
        using d_type = T;
        using node = xor_linked_list_node;
        d_type m_data{};
        node* m_ptr{};

        xor_linked_list_node() = default;

        xor_linked_list_node(const d_type& data)
            : m_data{data}, m_ptr{nullptr} {}

        xor_linked_list_node(const d_type& data, node* next)
            : m_data{data}, m_ptr{next} {}

        xor_linked_list_node(d_type&& data)
            : m_data{std::move(data)}, m_ptr{nullptr} {}

        xor_linked_list_node(d_type&& data, node* next)
            : m_data{std::move(data)}, m_ptr{next} {}

        xor_linked_list_node(const node& other)
            : m_data{other.m_data}, m_ptr{other.m_ptr} {}

        xor_linked_list_node(node&& other)
            : m_data{std::move(other.m_data)}, m_ptr{other.m_ptr} {}

        node& operator=(const node& other) {
            if(this == &other) return *this;
            m_data = other.m_data;
            m_ptr = other.m_ptr;
            return *this;
        }

        node& operator=(node&& other) {
            if(this == &other) return *this;
            m_data = std::move(other.m_data);
            m_ptr = other.m_ptr;
            return *this;
        }

        template <typename... Args>
        requires std::constructible_from<d_type, Args...>
        xor_linked_list_node(Args&&... args, node* ptr = nullptr)
            : m_data{std::forward<Args>(args)...}, m_ptr{ptr} {}

        ~xor_linked_list_node() {};

        friend class xor_linked_list<d_type>;

        friend class iterator_base<xor_linked_list<T>, T>;
        friend class iterator_base<xor_linked_list<T>,
                                   std::add_const_t<std::remove_const_t<T>>>;
    };

    template <typename T, typename U>
    requires std::same_as<T, U> || std::same_as<T, std::remove_const_t<U>>
    class iterator_base<xor_linked_list<T>, U> {
        using d_type = U;
        using iterator = iterator_base;
        using ptr_type = xor_linked_list_node<T>*;

        ptr_type data{};
        ptr_type previous{};
        ptr_type next{};

       public:
        iterator_base() = default;

        iterator_base(ptr_type ptr, ptr_type prev, ptr_type ne)
            : data{ptr}, previous{prev}, next{ne} {}

        iterator_base(ptr_type ptr, ptr_type prev = nullptr)
            : data{ptr}, previous{prev},
              next{(ptr_type) ((std::uintptr_t) ptr->m_ptr ^
                               (std::uintptr_t) prev)} {}

        iterator_base(const iterator& other)
            : data{other.data}, previous{other.previous}, next{other.next} {}

        iterator& operator=(const iterator& other) {
            if(this == &other) return *this;
            data = other.data;
            previous = other.previous;
            next = other.next;
            return *this;
        }

        iterator& operator++() {
            ptr_type temp = data;
            data = next;
            previous = temp;
            next = data ? (ptr_type) ((std::uintptr_t) data->m_ptr ^
                                      (std::uintptr_t) previous)
                        : nullptr;
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            data = next;
            previous = temp.data;
            next = data ? (ptr_type) ((std::uintptr_t) data->m_ptr ^
                                      (std::uintptr_t) previous)
                        : nullptr;
            return temp;
        }

        iterator& operator--() {
            ptr_type temp = data;
            data = previous;
            next = temp;
            previous = data ? (ptr_type) ((std::uintptr_t) data->m_ptr ^
                                          (std::uintptr_t) next)
                            : nullptr;
            return *this;
        }

        iterator operator--(int) {
            iterator temp = *this;
            data = previous;
            next = temp.data;
            previous = data ? (ptr_type) ((std::uintptr_t) data->m_ptr ^
                                          (std::uintptr_t) next)
                            : nullptr;
            return temp;
        }

        bool operator==(const iterator& other) { return data == other.data; }

        bool operator!=(const iterator& other) { return data != other.data; }

        d_type& operator*() { return data->m_data; }

        d_type* operator->() { return &(data->m_data); }

        template <typename X = U>
        requires(!std::is_const_v<X> && std::is_const_v<std::add_const_t<U>>)
        operator iterator_base<xor_linked_list<T>, std::add_const_t<U>>()
            const {
            return iterator_base<xor_linked_list<T>, std::add_const_t<U>>(
                data, previous, next);
        }

        friend class xor_linked_list<T>;
    };

    template <typename T>
    class xor_linked_list {
       public:
        using d_type = T;
        using iterator = iterator_base<xor_linked_list, d_type>;
        using const_iterator =
            iterator_base<xor_linked_list,
                          std::add_const_t<std::remove_const_t<T>>>;

       private:
        using node = xor_linked_list_node<d_type>;

        node* head{};

        node* tail{};

       public:
        void clear() {
            if(head == tail) {
                delete head;
                head = tail = nullptr;
                return;
            }
            iterator it = begin();
            while(it != end()) {
                delete it.data;
                ++it;
            }
            head = tail = nullptr;
        }

        iterator begin() { return head; }

        iterator end() { return {nullptr, tail, nullptr}; }

        const_iterator begin() const { return head; }

        const_iterator end() const { return {nullptr, tail, nullptr}; }

        const_iterator cbegin() const { return head; }

        const_iterator cend() const { return {nullptr, tail, nullptr}; }

        template <typename U>
        requires std::constructible_from<d_type, U>
        void push_front(U&& value) {
            if(head) {
                node* temp = head;
                head = new node(std::forward<U>(value), head);
                temp->m_ptr = (node*) ((std::uintptr_t) temp->m_ptr ^
                                       (std::uintptr_t) head);
            }
            else {
                head = new node(std::forward<U>(value), nullptr);
                tail = head;
            }
        }

        template <typename... Args>
        requires std::constructible_from<d_type, Args...>
        void emplace_front(Args&&... args) {
            if(head) {
                node* temp = head;
                head = new node(std::forward<Args>(args)..., head);
                temp->m_ptr = (node*) ((std::uintptr_t) temp->m_ptr ^
                                       (std::uintptr_t) head);
            }
            else {
                head = new node(std::forward<Args>(args)..., nullptr);
                tail = head;
            }
        }

        template <typename U>
        requires std::constructible_from<d_type, U>
        void push_back(U&& value) {
            if(tail) {
                node* temp = tail;
                tail = new node(std::forward<U>(value), tail);
                temp->m_ptr = (node*) ((std::uintptr_t) temp->m_ptr ^
                                       (std::uintptr_t) tail);
            }
            else {
                tail = new node(std::forward<U>(value), nullptr);
                head = tail;
            }
        }

        template <typename... Args>
        requires std::constructible_from<d_type, Args...>
        void emplace_back(Args&&... args) {
            if(tail) {
                node* temp = tail;
                tail = new node(std::forward<Args>(args)..., tail);
                temp->m_ptr = (node*) ((std::uintptr_t) temp->m_ptr ^
                                       (std::uintptr_t) tail);
            }
            else {
                tail = new node(std::forward<Args>(args)..., nullptr);
                head = tail;
            }
        }

        xor_linked_list& operator=(const xor_linked_list& other) {
            if(this == &other) return *this;
            clear();
            for(const d_type& i : other) {
                push_back(i);
            }
            return *this;
        }

        xor_linked_list& operator=(xor_linked_list&& other) {
            if(this == &other) return *this;
            clear();
            std::swap(head, other.head);
            std::swap(tail, other.tail);
            return *this;
        }

        bool empty() { return !head; }

        void pop_front() {
            if(head == tail) {
                delete head;
                head = tail = nullptr;
                return;
            }
            node* temp = head;
            head = head->m_ptr;
            head->m_ptr =
                (node*) ((std::uintptr_t) head->m_ptr ^ (std::uintptr_t) temp);
            delete temp;
        }

        void pop_back() {
            if(head == tail) {
                delete head;
                head = tail = nullptr;
                return;
            }
            node* temp = tail;
            tail = tail->m_ptr;
            tail->m_ptr =
                (node*) ((std::uintptr_t) tail->m_ptr ^ (std::uintptr_t) temp);
            delete temp;
        }

        template <typename U>
        requires std::constructible_from<d_type, U>
        void insert(const_iterator pos, U&& value) {
            if(pos == begin()) {
                push_front(std::forward<U>(value));
            }
            else if(pos == --cend()) {
                push_back(std::forward<U>(value));
            }
            else {
                node* temp = new node(std::forward<U>(value),
                                      (node*) ((std::uintptr_t) pos.previous ^
                                               (std::uintptr_t) pos.data));
                pos.previous->m_ptr =
                    (node*) ((std::uintptr_t) pos.previous->m_ptr ^
                             (std::uintptr_t) pos.data ^ (std::uintptr_t) temp);
                pos.data->m_ptr = (node*) ((std::uintptr_t) pos.data->m_ptr ^
                                           (std::uintptr_t) pos.previous ^
                                           (std::uintptr_t) temp);
            }
        }

        template <typename... Args>
        requires std::constructible_from<d_type, Args...>
        void emplace(const_iterator pos, Args&&... args) {
            if(pos == begin()) {
                push_front(std::forward<Args>(args)...);
            }
            else if(pos == --cend()) {
                push_back(std::forward<Args>(args)...);
            }
            else {
                node* temp = new node(std::forward<Args>(args)...,
                                      (node*) ((std::uintptr_t) pos.previous ^
                                               (std::uintptr_t) pos.data));
                pos.previous->m_ptr =
                    (node*) ((std::uintptr_t) pos.previous->m_ptr ^
                             (std::uintptr_t) pos.data ^ (std::uintptr_t) temp);
                pos.data->m_ptr = (node*) ((std::uintptr_t) pos.data->m_ptr ^
                                           (std::uintptr_t) pos.previous ^
                                           (std::uintptr_t) temp);
            }
        }

        void erase(const_iterator pos) {
            if(pos == begin()) {
                pop_front();
            }
            else if(pos == --cend()) {
                pop_back();
            }
            else {
                pos.next->m_ptr = (node*) ((std::uintptr_t) pos.next->m_ptr ^
                                           (std::uintptr_t) pos.previous ^
                                           (std::uintptr_t) pos.data);
                pos.previous->m_ptr =
                    (node*) ((std::uintptr_t) pos.previous->m_ptr ^
                             (std::uintptr_t) pos.next ^
                             (std::uintptr_t) pos.data);
                delete pos.data;
            }
        }

        void swap(xor_linked_list& other) {
            node* temp = head;
            head = other.head;
            other.head = temp;
            temp = tail;
            tail = other.tail;
            other.tail = temp;
        }

        ~xor_linked_list() { clear(); }

        xor_linked_list() = default;

        xor_linked_list(const xor_linked_list& other) {
            for(const d_type& i : other) {
                push_back(i);
            }
        }

        xor_linked_list(xor_linked_list&& other) {
            head = other.head;
            other.head = nullptr;
            tail = other.tail;
            other.tail = nullptr;
        }

        xor_linked_list(const std::initializer_list<d_type>& init) {
            for(const d_type& i : init) {
                push_back(i);
            }
        }

        xor_linked_list(std::initializer_list<d_type>&& init) {
            for(const d_type& i : init) {
                push_back(std::move(i));
            }
        }

        xor_linked_list& operator=(const std::initializer_list<d_type>& init) {
            clear();
            for(const d_type& i : init) {
                push_back(i);
            }
            return *this;
        }

        xor_linked_list& operator=(std::initializer_list<d_type>&& init) {
            clear();
            for(const d_type& i : init) {
                push_back(std::move(i));
            }
            return *this;
        }

        d_type& front() { return *begin(); }

        d_type& back() { return *end(); }
    };

}  // namespace nig

#endif