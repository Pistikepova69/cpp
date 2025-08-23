#ifndef __STACK__
#define __STACK__

#include "vector.h"

namespace nig{

    template <typename T>
    class stack{
    public:
        using d_type = T;

    private:
        nig::vector<T> data;
    
    public:
        stack& operator=(const stack& other){
            this->data = other.data;
            return *this;
        }
        
        stack& operator=(stack&& other){
            this->data = other.data;
            return *this;
        }

        d_type& top(){
            return data[data.size() - 1];
        }
        

        size_t size(){
            return data.size();
        }

        bool empty(){
            return !size();
        }

        template<typename U>
        requires std::constructible_from<d_type, U>
        void push(U&& value){
            data.push_back(std::forward<U>(value));
        }
        
        template<typename... Args>
        requires std::constructible_from<d_type, Args...>
        void emplace(Args&&... args){
            data.emplace_back(std::forward<Args>(args)...);
        }

        void pop(){
            data.pop_back();
        }

        bool operator==(const stack& other){
            return data == other.data;
        }

        bool operator!=(const stack& other){
            return !(*this == other);
        }

    };

}

#endif