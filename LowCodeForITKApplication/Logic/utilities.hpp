#pragma once
#include <memory>

class ToNonOwningPointer
{
  public:
    template <typename T>
    T *operator()(const std::unique_ptr<T> &ptr)
    {
        return ptr.get();
    }
};