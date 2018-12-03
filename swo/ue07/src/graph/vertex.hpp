#pragma once
#include <iostream>
#include <memory>

template <typename T>
class vertex_t;

template <typename T>
std::ostream & operator<<(std::ostream &os, const vertex_t<T> &vertex);

template <typename T>
class vertex_t
{
    template <typename _T>
    friend std::ostream & operator<<(std::ostream &os, const vertex_t<_T> &vertex);

public: // methods

    /**
     * Avoids unnecessary copies by forwarding
     * construction to make_shared.
     * @tparam _Constr The types of the arguments used by
     *                 one of the constructors for T.
     * @param args The T-Constructors arguments.
     */
    template <typename ..._Constr>
    explicit vertex_t(_Constr &&...args)
    : _value {std::make_shared<T>(std::forward<_Constr>(args)...)} { }

private: // members

    /**
     * The information held by a vertex.
     */
    std::shared_ptr<T> _value;
};

template <typename T>
std::ostream & operator<<(std::ostream &os, const vertex_t<T> &vertex)
{
    return os << *vertex._value;
}