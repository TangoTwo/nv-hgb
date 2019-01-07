#pragma once

namespace swo {

template <typename T, typename Container>
class _iterator;

template <typename T, typename Container>
_iterator <T, Container> operator+(const _iterator <T, Container> &it, std::size_t n);

template <typename T, typename Container>
_iterator <T, Container> operator-(const _iterator <T, Container> &it, std::size_t n);

template <typename T, typename Container>
std::size_t operator-(const _iterator <T, Container> &lhs, const _iterator <T, Container> &rhs);

template <typename T, typename Container>
bool operator==(const _iterator <T, Container> &lhs, const _iterator <T, Container> &rhs);

template <typename T, typename Container>
bool operator!=(const _iterator <T, Container> &lhs, const _iterator <T, Container> &rhs);

template <typename T, typename Container>
bool operator<(const _iterator <T, Container> &lhs, const _iterator <T, Container> &rhs);

template <typename T, typename Container>
bool operator>(const _iterator <T, Container> &lhs, const _iterator <T, Container> &rhs);

template <typename T, typename Container>
bool operator>=(const _iterator <T, Container> &lhs, const _iterator <T, Container> &rhs);

template <typename T, typename Container>
bool operator<=(const _iterator <T, Container> &lhs, const _iterator <T, Container> &rhs);

template <typename T, typename Container>
class _iterator // LegacyRandomAccessIterator
{
    friend Container;

    friend _iterator operator+ <T>(const _iterator &it, std::size_t n);

    friend _iterator operator- <T>(const _iterator &it, std::size_t n);

    friend std::size_t operator- <T>(const _iterator &lhs, const _iterator &rhs);

    friend bool operator== <T>(const _iterator &lhs, const _iterator &rhs);

    friend bool operator!= <T>(const _iterator &lhs, const _iterator &rhs);

    friend bool operator< <T>(const _iterator &lhs, const _iterator &rhs);

    friend bool operator> <T>(const _iterator &lhs, const _iterator &rhs);

    friend bool operator>= <T>(const _iterator &lhs, const _iterator &rhs);

    friend bool operator<= <T>(const _iterator &lhs, const _iterator &rhs);

public: // methods
    _iterator(T &&) = delete;

    virtual _iterator &operator+=(std::size_t n);

    _iterator &operator-=(std::size_t n);

    T &operator[](std::size_t i);

    T &operator*();

protected: // methods
    _iterator(Container &parent, T *deq_it)
            : _parent { parent }, _current { deq_it }
    {}

protected: // members
    Container &_parent;

    T *_current;
};

template <typename T, typename Container>
_iterator <T, Container> &_iterator <T, Container>::operator+=(std::size_t n)
{
    return *this; // returns copy of itself by default (this is BAD!)
}

template <typename T, typename Container>
_iterator <T, Container> &_iterator <T, Container>::operator-=(std::size_t n)
{
    *this += -n;
    return *this;
}

template <typename T, typename Container>
T &_iterator <T, Container>::operator[](std::size_t i)
{
    return *(*this + i);
}

template <typename T, typename Container>
T &_iterator <T, Container>::operator*()
{
    assert(_current);
    return *_current;
}

template <typename T, typename Container>
_iterator <T, Container> operator+(const _iterator <T, Container> &it, std::size_t n)
{
    auto cpy = it;
    return cpy += n;
}

template <typename T, typename Container>
_iterator <T, Container> operator-(const _iterator <T, Container> &it, std::size_t n)
{
    auto cpy = it;
    return cpy -= n;
}

template <typename T, typename Container>
std::size_t operator-(const _iterator <T, Container> &lhs, const _iterator <T, Container> &rhs)
{
    auto lhs_cpy = lhs;
    decltype(lhs._parent.size()) size = 0;
    while (lhs_cpy != lhs) {
        size += 1;
        lhs_cpy += 1;
    }
    return size;
}

template <typename T, typename Container>
bool operator==(const _iterator <T, Container> &lhs, const _iterator <T, Container> &rhs)
{
    return lhs._current == rhs._current;
}

template <typename T, typename Container>
bool operator!=(const _iterator <T, Container> &lhs, const _iterator <T, Container> &rhs)
{
    return !(lhs == rhs);
}

template <typename T, typename Container>
bool operator<(const _iterator <T, Container> &lhs, const _iterator <T, Container> &rhs)
{
    return rhs - lhs > 0;
}

template <typename T, typename Container>
bool operator>(const _iterator <T, Container> &lhs, const _iterator <T, Container> &rhs)
{
    return rhs < lhs;
}

template <typename T, typename Container>
bool operator>=(const _iterator <T, Container> &lhs, const _iterator <T, Container> &rhs)
{
    return !(lhs < rhs);
}

template <typename T, typename Container>
bool operator<=(const _iterator <T, Container> &lhs, const _iterator <T, Container> &rhs)
{
    return !(lhs > rhs);
}

} // namespace swo