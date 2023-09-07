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

template<typename EnumClass>
std::underlying_type_t<EnumClass> to_underlying(EnumClass enumValue)
{
    return static_cast<std::underlying_type_t<EnumClass>>(enumValue);
}