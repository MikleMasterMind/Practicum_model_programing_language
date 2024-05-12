#ifndef _STACK_H_
#define _STACK_H_

#include <vector>

namespace translator 
{
    template <class T>
    class Stack
    {
        std::vector<T> container;
        int top;
    public:
        Stack() : top(-1) {}
        void reset()
        {
            top = -1;
            container.clear();
            container.shrink_to_fit();
        }
        void push(T elem)
        {
            container.push_back(elem);
            top++;
        }
        T pop()
        {
            if (empty()) 
                throw "stack_is_empty";
            T buf = container[top--];
            container.pop_back();
            return buf;
        }
        bool empty() const 
        {
            return top == -1;
        }
    };
}

#endif // ! _STACK_H_