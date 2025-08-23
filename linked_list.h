#ifndef __LINKED_LIST__
#define __LINKED_LIST__

#include <cassert>
#include <utility>
#include <limits>
#include <initializer_list>
#include <concepts>
#include <type_traits>
#include <iterator>
#include "iterator.h"
//#include "stack.h"
#include <iostream>


namespace nig{

    template<typename T>
    class linked_list;

    template<typename T, typename U>
    requires std::same_as<T, U> || std::same_as<T, std::remove_const_t<U>>
    class iterator_base<linked_list<T>, U>;

    
    template <typename T>
    class linked_list_node{
        using d_type = T;
        using node = linked_list_node;
        d_type m_data{};
        node* m_next{};
        
        linked_list_node() = default;
        
        linked_list_node(const d_type& data)
        : m_data{data}, m_next{nullptr}{}
        
        linked_list_node(const d_type& data, node* next)
        : m_data{data}, m_next{next}{}
        
        linked_list_node(d_type&& data)
        : m_data{std::move(data)}, m_next{nullptr}{}
        
        linked_list_node(d_type&& data, node* next)
        : m_data{std::move(data)}, m_next{next}{}
        
        linked_list_node(const node& other)
        : m_data{other.m_data}, m_next{other.m_next}{}
        
        linked_list_node(node&& other)
        : m_data{std::move(other.m_data)}, m_next{other.m_next}{}
        
        node& operator=(const node& other){
            if(this == &other){}
                return *this;
            m_data = other.m_data;
            m_next = other.m_next;
        }
        
        node& operator=(node&& other){
            if(this == &other)
                return *this;
            m_data = std::move(other.m_data);
            m_next = other.m_next;
            return *this;
        }

        template <typename... Args>
        requires std::constructible_from<d_type, Args...>
        linked_list_node(Args&&... args, node* ptr = nullptr) :m_data{std::forward<Args>(args)...}, m_next{ptr} {}
    
        
        ~linked_list_node() = default;

        friend class linked_list<d_type>;

        friend class iterator_base<linked_list<T>, T>;
        friend class iterator_base<linked_list<T>, std::add_const_t<std::remove_const_t<T>>>;
            
    };



    template<typename T, typename U>
    requires std::same_as<T, U> || std::same_as<T, std::remove_const_t<U>>
    class iterator_base<linked_list<T>, U>{
        using d_type = U;
        using iterator = iterator_base;
        using ptr_type = linked_list_node<T>*;

        ptr_type data{};


    public:

        iterator_base() = default;
        iterator_base(ptr_type ptr) : data{ptr}{}
        iterator_base(const iterator& other) : data{other.data}{}

        iterator& operator=(const iterator& other){
            if(this == &other)
                return *this;
            data = other.data;
            return *this;
        }


        iterator& operator++(){
            data = data->m_next;
            return *this;
        }

        iterator operator++(int){
            ptr_type temp = this->data;
            data = data->m_next;
            return temp;
        }

        bool operator==(const iterator& other){
            return data == other.data;
        }

        bool operator!=(const iterator& other){
            return data != other.data;
        }

        d_type& operator*(){
            return data->m_data;
        }

        d_type* operator->(){
            return &(data->m_data);
        }

        template <typename X = U>
        requires (!std::is_const_v<X>)
        operator iterator_base<linked_list<T>, std::add_const_t<U>>() const{
            return iterator_base<linked_list<T>, std::add_const_t<U>>(data);
        }

        friend class linked_list<T>;

    };


    template<typename T>
    class linked_list{

        
        public:
        using d_type = T;
        using iterator = iterator_base<linked_list, d_type>;
        using const_iterator = iterator_base<linked_list, std::add_const_t<std::remove_const_t<T>>>;
        
        
    private:
        
        using node = linked_list_node<d_type>;
        
        node head{};
        
        
    public:
        

        void clear(){
            node* current = &head;
            node* next = current->m_next;
            while(current = next){
                next = next->m_next;
                delete current;
            }
            head.m_next = nullptr;
        }

        iterator begin(){
            return head.m_next;
        }

        iterator before_begin(){
            return &head;
        }

        iterator end(){
            return nullptr;
        }

        const_iterator begin() const{
            return head.m_next;
        }

        const_iterator before_begin() const{
            return const_cast<node*>(&head);
        }

        const_iterator end() const{
            return nullptr;
        }

        const_iterator cbegin() const{
            return head.m_next;
        }

        const_iterator cbefore_begin() const{
            return const_cast<node*>(&head);
        }

        const_iterator cend() const{
            return nullptr;
        }

        template<typename U>
        requires std::constructible_from<d_type, U>
        void push_front(U&& value){
            head.m_next = new node(std::forward<U>(value), head.m_next);
        }

        template <typename... Args>
        requires std::constructible_from<d_type, Args...>
        void emplace_front(Args&&... args){
            head.m_next = new node(std::forward<Args>(args)..., head.m_next);
        }

        linked_list& operator=(const linked_list& other){
            if(this == & other)
                return *this;
            clear();
            node* this_current = &head;
            node* other_current = other.head.m_next;
            while(other_current){
                this_current->m_next = new node(other_current->m_data);
                this_current = this_current->m_next;
                other_current = other_current->m_next;
            }
            return *this;
        }

        linked_list& operator=(linked_list&& other){
            if(this == &other)
                return *this;
            clear();
            std::swap(head.m_next, other.head.m_next);
            return *this;
        }

        bool empty(){
            return !head.m_next;
        }

        template<typename U>
        requires std::constructible_from<d_type, U>
        void insert_after(const_iterator pos, U&& value){
            pos.data->m_next = new node(std::forward<U>(value), pos.data->m_next);
        }

        template<typename... Args>
        requires std::constructible_from<d_type, Args...>
        void emplace_after(const_iterator pos, Args&&... args){
            pos.data->m_next = new node(std::forward<Args>(args)..., pos.data->m_next);
        }

        void pop_front(){
            node* temp = head.m_next;
            head.m_next = head.m_next->m_next;
            delete temp;
        }

        void erase_after(const_iterator pos){
            node* temp = pos.data->m_next;
            pos.data->m_next = pos.data->m_next->m_next;
            delete temp;
        }

        void swap(linked_list& other){
            node* temp = head.m_next;
            head.m_next = other.head.m_next;
            other.head.m_next = temp;
        }

        ~linked_list(){
            clear();
        }

        linked_list() = default;

        linked_list(const linked_list& other){
            node* current = &head;
            for(const d_type& i : other){
                current->m_next = new node(std::move(i));
                current = current->m_next;
            }
        }

        linked_list(linked_list&& other){
            head.m_next = other.head.m_next;
            other.head.m_next = nullptr;
        }

        linked_list(const std::initializer_list<d_type>& init){
            node* current = &head;
            for(const d_type& i : init){
                current->m_next = new node(i);
                current = current->m_next;
            }
        }

        linked_list& operator=(const std::initializer_list<d_type>& init){
            clear();
            node* current = &head;
            for(const d_type& i : init){
                current->m_next = new node(i);
                current = current->m_next;
            }
            return *this;
        }

        d_type& front(){
            return *begin();
        }

    };



}



#endif