#ifndef __VECTOR__
#define __VECTOR__

#include <cassert>
#include <concepts>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>

#include "iterator.h"

namespace nig {
    template <typename T>
    class vector;

    template <typename T, typename U>
    requires std::same_as<T, U> || std::same_as<T, std::remove_const_t<U>>
    class iterator_base<vector<T>, U> {
       public:
        using d_type = U;
        using v_type = vector<T>;
        using s_type = typename vector<T>::s_type;
        using diff_type = std::ptrdiff_t;

        // stl compatibility
        using iterator_category = std::random_access_iterator_tag;
        using iterator_concept = std::contiguous_iterator_tag;
        using value_type = std::remove_cv_t<U>;
        using difference_type = std::ptrdiff_t;
        using pointer = U*;
        using reference = U&;

       private:
        d_type* m_ptr{};
        using iterator = iterator_base;

       public:
        iterator& operator++() {
            ++m_ptr;
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++m_ptr;
            return temp;
        }

        iterator& operator--() {
            --m_ptr;
            return *this;
        }

        iterator operator--(int) {
            iterator temp = *this;
            --m_ptr;
            return temp;
        }

        d_type* operator->() const { return m_ptr; }

        d_type& operator*() const { return *m_ptr; }

        d_type& operator[](diff_type index) const { return m_ptr[index]; }

        iterator& operator+=(diff_type i) {
            m_ptr += i;
            return *this;
        }

        iterator& operator-=(diff_type i) {
            m_ptr -= i;
            return *this;
        }

        diff_type operator-(const iterator& other) const {
            return m_ptr - other.m_ptr;
        }

        iterator operator-(diff_type n) const { return {m_ptr - n}; }

        iterator operator+(diff_type n) const { return {m_ptr + n}; }

        friend iterator operator+(diff_type n, const iterator& it) {
            return {it.m_ptr + n};
        }

        iterator_base() = default;

        iterator_base(d_type* ptr) : m_ptr{ptr} {}

       private:
        friend class vector<T>;

       public:
        iterator_base(const iterator& it) : m_ptr{it.m_ptr} {}

        iterator_base(iterator&& it) : m_ptr{it.m_ptr} {}

        iterator& operator=(const iterator& it) {
            m_ptr = it.m_ptr;
            return *this;
        }

        iterator& operator=(iterator&& it) {
            m_ptr = it.m_ptr;
            return *this;
        }

        ~iterator_base() = default;

        bool operator==(const iterator& other) const {
            return m_ptr == other.m_ptr;
        }

        bool operator!=(const iterator& other) const {
            return m_ptr != other.m_ptr;
        }

        bool operator<(const iterator& other) const {
            return m_ptr < other.m_ptr;
        }

        bool operator>(const iterator& other) const {
            return m_ptr > other.m_ptr;
        }

        bool operator>=(const iterator& other) const {
            return !(*this < other);
        }

        bool operator<=(const iterator& other) const {
            return !(*this > other);
        }

        template <typename X = U>
        requires(!std::is_const_v<X>)
        operator iterator_base<vector<T>, const U>() const {
            return iterator_base<vector<T>, const U>(m_ptr);
        }

        explicit iterator_base(const reverse_iterator_base<vector<T>, U>& rit)
            : m_ptr{rit.operator->() + 1} {}
    };

    template <typename T, typename U>
    requires std::same_as<T, U> || std::same_as<T, std::remove_const_t<U>>
    class reverse_iterator_base<vector<T>, U> {
       public:
        using d_type = U;
        using v_type = vector<T>;
        using s_type = typename vector<T>::s_type;
        using diff_type = std::ptrdiff_t;

        // stl compatibility
        using iterator_category = std::random_access_iterator_tag;
        using iterator_concept = std::contiguous_iterator_tag;
        using value_type = std::remove_cv_t<U>;
        using difference_type = std::ptrdiff_t;
        using pointer = U*;
        using reference = U&;

       private:
        d_type* m_ptr{};
        using iterator = reverse_iterator_base;

       public:
        iterator& operator++() {
            --m_ptr;
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            --m_ptr;
            return temp;
        }

        iterator& operator--() {
            ++m_ptr;
            return *this;
        }

        iterator operator--(int) {
            iterator temp = *this;
            ++m_ptr;
            return temp;
        }

        d_type* operator->() const { return (m_ptr - 1); }

        d_type& operator*() const { return *(m_ptr - 1); }

        d_type& operator[](diff_type index) const { return m_ptr[-index - 1]; }

        iterator& operator+=(diff_type i) {
            m_ptr -= i;
            return *this;
        }

        iterator& operator-=(diff_type i) {
            m_ptr += i;
            return *this;
        }

        diff_type operator-(const iterator& other) const {
            return other.m_ptr - m_ptr;
        }

        iterator operator-(diff_type n) const { return {m_ptr + n}; }

        iterator operator+(diff_type n) const { return {m_ptr - n}; }

        friend iterator operator+(diff_type n, const iterator& it) {
            return {it.m_ptr - n};
        }

        reverse_iterator_base() = default;

        reverse_iterator_base(d_type* ptr) : m_ptr{ptr} {}

       private:
        friend class vector<T>;

       public:
        reverse_iterator_base(const iterator& it) : m_ptr{it.m_ptr} {}

        reverse_iterator_base(iterator&& it) : m_ptr{it.m_ptr} {}

        iterator& operator=(const iterator& it) {
            m_ptr = it.m_ptr;
            return *this;
        }

        iterator& operator=(iterator&& it) {
            m_ptr = it.m_ptr;
            return *this;
        }

        ~reverse_iterator_base() = default;

        bool operator==(const iterator& other) const {
            return m_ptr == other.m_ptr;
        }

        bool operator!=(const iterator& other) const {
            return m_ptr != other.m_ptr;
        }

        bool operator<(const iterator& other) const {
            return m_ptr > other.m_ptr;
        }

        bool operator>(const iterator& other) const {
            return m_ptr < other.m_ptr;
        }

        bool operator>=(const iterator& other) const {
            return !(*this > other);
        }

        bool operator<=(const iterator& other) const {
            return !(*this < other);
        }

        iterator_base<vector<T>, U> base() const {
            return iterator_base<vector<T>, U>(m_ptr);
        }

        template <typename X = U>
        requires(!std::is_const_v<X>)
        operator reverse_iterator_base<vector<T>, const U>() const {
            return reverse_iterator_base<vector<T>, const U>(m_ptr);
        }

        /*explicit reverse_iterator_base(const iterator_base<vector<T>, U>& it)
        : m_ptr{it.(operator->)()}{}*/
    };

    template <typename T>
    class vector {
       public:
        using d_type = T;
        using s_type = size_t;
        using iterator = iterator_base<vector, d_type>;
        using const_iterator =
            iterator_base<vector, std::add_const_t<std::remove_const_t<T>>>;
        using reverse_iterator = reverse_iterator_base<vector, T>;
        using const_reverse_iterator =
            reverse_iterator_base<vector,
                                  std::add_const_t<std::remove_const_t<T>>>;

        // stl
        using value_type = T;
        using allocator_type = void;
        using size_type = std::size_t;
        using difference_type = std::iter_difference_t<iterator>;
        using reference = T&;
        using const_reference = std::add_const_t<std::remove_const_t<T>>&;
        using pointer = T*;
        using const_pointer = std::add_const_t<std::remove_const_t<T>>*;

       private:
        s_type m_capacity{}, m_size{};
        d_type* m_data{};

        void change_capacity(s_type new_capacity) {
            d_type* temp_ptr =
                (d_type*) ::operator new(sizeof(d_type) * new_capacity);
            s_type lim = m_size > new_capacity ? new_capacity : m_size;
            for(s_type i = 0; i < lim; ++i) {
                new(temp_ptr + i) d_type(std::move(m_data[i]));
                m_data[i].~d_type();
            }
            if(new_capacity == 0) {
                for(s_type i = 0; i < m_size; ++i) {
                    m_data[i].~d_type();
                }
                ::operator delete(temp_ptr);
                temp_ptr = nullptr;
            }
            ::operator delete(m_data);
            m_data = temp_ptr;
            m_capacity = new_capacity;
        }

       public:
        s_type size() const { return m_size; }

        vector& reserve(s_type capacity) {
            if(capacity > m_capacity) {
                change_capacity(capacity);
            }
            return *this;
        }

        vector& resize(s_type size) {
            while(m_size > size) {
                pop_back();
            }
            reserve(size);
            for(s_type i = m_size; i < size; ++i) {
                new(m_data + i) d_type();
            }
            m_size = size;
            return *this;
        }

        vector& increase(s_type size) { return reserve(size + m_size); }

        s_type capacity() const { return m_capacity; }

        vector& shrink_to_fit() {
            change_capacity(m_size);
            return *this;
        }

        bool empty() const { return !m_size; }

        constexpr s_type max_size() const {
            return std::numeric_limits<s_type>::max();
        }

        d_type& operator[](s_type index) { return m_data[index]; }

        const d_type& operator[](s_type index) const { return m_data[index]; }

        d_type& front() { return m_data[0]; }

        const d_type& front() const { return m_data[0]; }

        d_type& back() { return m_data[m_size - 1]; }

        const d_type& back() const { return m_data[m_size - 1]; }

        d_type* data() { return m_data; }

        const d_type* data() const { return m_data; }

        vector& clear() { return resize(0); }

        vector& destroy() {
            change_capacity(0);
            m_size = 0;
            return *this;
        }

        template <typename U>
        requires std::constructible_from<d_type, U>
        iterator insert(s_type index, U&& value) {
            if(m_size >= m_capacity) {
                change_capacity(m_capacity == 0 ? 1 : 2 * m_capacity);
            }
            if(index < m_size) {
                new(m_data + m_size) d_type(std::move(m_data[m_size - 1]));
                for(s_type i = m_size - 1; i > index; --i) {
                    m_data[i] = std::move(m_data[i - 1]);
                }
                m_data[index].~d_type();
            }
            new(m_data + index) d_type(std::forward<U>(value));
            ++m_size;
            return {m_data + index};
        }

        template <typename U>
        requires std::constructible_from<d_type, U>
        iterator insert(const iterator& it, U&& value) {
            s_type index = it - begin();
            insert(index, std::forward<U>(value));
            return {m_data + index};
        }

        template <typename U>
        requires std::constructible_from<d_type, U>
        reverse_iterator insert(const reverse_iterator& rit, U&& value) {
            return reverse_iterator(
                insert(rit.base() - 1, std::forward<U>(value)) + 1);
        }

        template <typename U>
        requires std::constructible_from<d_type, U>
        const_iterator insert(const const_iterator& it, U&& value) {
            s_type index = it - cbegin();
            insert(index, std::forward<U>(value));
            return {m_data + index};
        }

        template <typename U>
        requires std::constructible_from<d_type, U>
        const_reverse_iterator insert(const const_reverse_iterator& rit,
                                      U&& value) {
            return const_reverse_iterator(
                insert(rit.base() - 1, std::forward<U>(value)) + 1);
        }

        template <typename U>
        requires std::constructible_from<d_type, U>
        vector& push_back(U&& value) {
            if(m_size >= m_capacity) {
                change_capacity(m_capacity == 0 ? 1 : 2 * m_capacity);
            }
            new(m_data + m_size) d_type(std::forward<U>(value));
            ++m_size;
            return *this;
        }

        vector& pop_back() {
            --m_size;
            m_data[m_size].~d_type();
            if(m_size * 4 < m_capacity) {
                change_capacity(2 > m_capacity / 2 ? 2 : m_capacity / 2);
            }
            return *this;
        }

        void swap(vector& other) {
            d_type* temp_data;
            s_type temp_size;

            temp_size = other.m_capacity;
            other.m_capacity = m_capacity;
            m_capacity = temp_size;

            temp_size = other.m_size;
            other.m_size = m_size;
            m_size = temp_size;

            temp_data = other.m_data;
            other.m_data = m_data;
            m_data = temp_data;
        }

        template <typename... Args>
        requires std::constructible_from<d_type, Args...>
        iterator emplace(s_type index, Args&&... args) {
            if(m_size >= m_capacity) {
                change_capacity(m_capacity == 0 ? 1 : 2 * m_capacity);
            }
            if(index < m_size) {
                new(m_data + m_size) d_type(std::move(m_data[m_size - 1]));
                for(s_type i = m_size - 1; i > index; --i) {
                    m_data[i] = std::move(m_data[i - 1]);
                }
            }
            m_data[index].~d_type();
            new(m_data + index) d_type(std::forward<Args>(args)...);
            ++m_size;
            return {m_data + index};
        }

        template <typename... Args>
        requires std::constructible_from<d_type, Args...>
        iterator emplace(const iterator& it, Args&&... args) {
            s_type index = it - begin();
            emplace(index, std::forward<Args>(args)...);
            return {m_data + index};
        }

        template <typename... Args>
        requires std::constructible_from<d_type, Args...>
        const_iterator emplace(const const_iterator& it, Args&&... args) {
            s_type index = it - cbegin();
            emplace(index, std::forward<Args>(args)...);
            return {m_data + index};
        }

        template <typename... Args>
        requires std::constructible_from<d_type, Args...>
        reverse_iterator emplace(const reverse_iterator& rit, Args&&... args) {
            return reverse_iterator(
                emplace(rit.base() - 1, std::forward<Args>(args)...) + 1);
        }

        template <typename... Args>
        requires std::constructible_from<d_type, Args...>
        const_reverse_iterator emplace(const const_reverse_iterator& rit,
                                       Args&&... args) {
            return reverse_iterator(
                emplace(rit.base() - 1, std::forward<Args>(args)...) + 1);
        }

        template <typename... Args>
        requires std::constructible_from<d_type, Args...>
        vector& emplace_back(Args&&... args) {
            if(m_size >= m_capacity) {
                change_capacity(m_capacity == 0 ? 1 : 2 * m_capacity);
            }
            new(m_data + m_size) d_type(std::forward<Args>(args)...);
            ++m_size;
            return *this;
        }

        vector() = default;

        vector(s_type capacity) : m_capacity{capacity}, m_size{0} {
            change_capacity(capacity);
        }

        vector(const std::initializer_list<d_type>& init)
            : m_capacity{init.size()} {
            change_capacity(init.size());
            s_type i = 0;
            for(auto it = init.begin(); it < init.end(); ++it, ++i) {
                new(m_data + i) d_type(*it);
            }
            m_size = init.size();
        }

        vector(std::initializer_list<d_type>&& init) : m_capacity{init.size()} {
            change_capacity(init.size());
            s_type i = 0;
            for(auto it = init.begin(); it < init.end(); ++it, ++i) {
                new(m_data + i) d_type(std::move(*it));
            }
            m_size = init.size();
        }

        vector(const vector& other) : m_capacity{other.m_capacity} {
            change_capacity(other.m_capacity);
            for(s_type i = 0; i < other.m_size; ++i) {
                new(m_data + i) d_type(other[i]);
            }
            m_size = other.m_size;
        }

        vector(vector&& other)
            : m_capacity{other.m_capacity}, m_size{other.m_size},
              m_data{other.m_data} {
            other.m_data = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }

        vector& operator=(const vector& other) {
            if(this != &other) {
                destroy();
                change_capacity(other.m_capacity);
                m_size = other.m_size;
                for(s_type i = 0; i < other.m_size; ++i) {
                    new(m_data + i) d_type(other[i]);
                }
            }
            return *this;
        }

        vector& operator=(vector&& other) {
            if(this != &other) {
                destroy();
                swap(other);
            }
            return *this;
        }

        ~vector() { destroy(); }

        bool operator==(const vector& other) const {
            if(m_size == other.m_size) {
                for(s_type i = 0; i < m_size; ++i) {
                    if(m_data[i] != other.m_data[i]) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }

        bool operator!=(const vector& other) const { return !(*this == other); }

        // insert range?
        // append range?
        iterator begin() { return {m_data}; }

        iterator end() { return {m_data + m_size}; }

        const_iterator begin() const { return {m_data}; }

        const_iterator end() const { return {m_data + m_size}; }

        const_iterator cbegin() const { return {m_data}; }

        const_iterator cend() const { return {m_data + m_size}; }

        reverse_iterator rend() { return {m_data}; }

        reverse_iterator rbegin() { return {m_data + m_size}; }

        const_reverse_iterator rend() const { return {m_data}; }

        const_reverse_iterator rbegin() const { return {m_data + m_size}; }

        const_reverse_iterator crbegin() const { return {m_data + m_size}; }

        const_reverse_iterator crend() const { return {m_data}; }

        vector(s_type size, const d_type& element)
            : m_capacity{size}, m_size{} {
            change_capacity(size);
            m_size = size;
            for(unsigned int i = 0; i < size; ++i) {
                new(m_data + i) d_type(element);
            }
        }

        vector& operator=(const std::initializer_list<d_type>& init) {
            destroy();
            m_size = 0;
            m_capacity = 0;
            change_capacity(init.size());
            for(s_type i = 0; i + init.begin() < init.end(); ++i) {
                push_back(*(init.begin() + i));
            }
            return *this;
        }

        vector& operator=(std::initializer_list<d_type>&& init) {
            destroy();
            m_size = 0;
            m_capacity = 0;
            change_capacity(init.size());
            for(s_type i = 0; i + init.begin() < init.end(); ++i) {
                push_back(std::move(*(init.begin() + i)));
            }
            return *this;
        }

        vector(const iterator& b, const iterator& e) {
            for(auto it = b; it != e; ++it) {
                push_back(*it);
            }
        }

        vector(const const_iterator& b, const const_iterator& e) {
            for(auto it = b; it != e; ++it) {
                push_back(*it);
            }
        }

        vector(const reverse_iterator& b, const reverse_iterator& e) {
            for(auto it = b; it != e; ++it) {
                push_back(*it);
            }
        }

        vector(const const_reverse_iterator& b,
               const const_reverse_iterator& e) {
            for(auto it = b; it != e; ++it) {
                push_back(*it);
            }
        }

        iterator erase(const_iterator it) {
            std::ptrdiff_t index = it - cbegin();
            if(it >= cend()) {
                return end();
            }
            else {
                for(iterator i = begin() + index; i != end(); ++i) {
                    *(i - 1) = std::move(*i);
                }
            }
            pop_back();
            return {m_data + index};
        }

        iterator erase(s_type index) { return erase(begin() + index); }

        // assign
    };
}  // namespace nig

#endif