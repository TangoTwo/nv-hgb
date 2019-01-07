#pragma once

#include <initializer_list>
#include <memory>
#include <functional>
#include "_iterator.hpp"

namespace swo {

template <typename T>
class deque;

template <typename T>
bool operator==(deque <T> const &lhs, deque <T> const &rhs);

template <typename T>
bool operator!=(deque <T> const &lhs, deque <T> const &rhs);

template <typename T>
bool operator<(deque <T> const &lhs, deque <T> const &rhs);

template <typename T>
bool operator<=(deque <T> const &lhs, deque <T> const &rhs);

template <typename T>
bool operator>(deque <T> const &lhs, deque <T> const &rhs);

template <typename T>
bool operator>=(deque <T> const &lhs, deque <T> const &rhs);

template <typename T>
void swap(deque <T> &lhs, deque <T> &rhs);

template <typename T>
class deque final
{
    friend class _iterator <T, deque <T>>;

public: // typedefs
    using value_type = T;
    using reference = T &;
    using size_type = std::size_t;

public: // nested classes

    class iterator : public _iterator <T, deque <T>>
    {
        friend class deque<T>;

    public:
        _iterator <T, deque <T>> &operator+=(std::size_t n) override;

    protected:
        iterator(deque <T> &parent, T *deq_it)
            : _iterator<T, deque<T>>(parent, deq_it) {}
    };

public: // methods

    deque();

    explicit deque(size_type capacity);

    deque(size_type capacity, const T &value);

    deque(const deque &other);

    deque(deque &&other) noexcept;

    deque(std::initializer_list <T> il);

    ~deque();

    deque &operator=(const deque &other);

    deque &operator=(deque &&other) noexcept;

    deque &operator=(std::initializer_list <T> il);

    reference operator[](size_type pos);

    reference at(size_type pos);

    reference back();

    reference front();

    iterator begin() noexcept;

    iterator end() noexcept;

    bool empty() const noexcept;

    size_type size() const noexcept;

    void clear() noexcept;

    void push_back(const T &value);

    void push_back(T &&value);

    void pop_back();

    void push_front(const T &value);

    void push_front(T &&value);

    void pop_front();

    void resize(size_type count);

    void swap(deque &other) noexcept;

private: // methods

    void _deallocate();

    void _assert_not_null(const T *p, const std::string &message);

    T *_next(const T *it) const;

    T *_previous(const T *it) const;

private: // constants
    const static size_type _INITIAL_CAPACITY { 5 };
private: // members

    /**
     * The allocation method to use for the deque buffer.
     */
    std::allocator <T> _alloc {};

    /**
     * A pointer to the first element in the buffer.
     */
    T *_buffer { nullptr };

    /**
     * The begin of the container. Off-The-End pointer
     * for empty deques.
     */
    T *_begin { nullptr };

    /**
     * Apointer to the last element in the container.
     */
    T *_end { nullptr };

    size_type _capacity { 0 };
};

template <typename T>
_iterator <T, deque <T>> &deque <T>::iterator::operator+=(std::size_t n)
{
    if (n > 0) {
        while (n != 0) {
            n--;
            this->_current = this->_parent._next(this->_current);
        }
    } else {
        while (n != 0) {
            n++;
            this->_current = this->_parent._previous(this->_current);
        }
    }
    return *this;
}

template <typename T>
deque <T>::deque() : deque(_INITIAL_CAPACITY)
{}

template <typename T>
deque <T>::deque(size_type capacity)
        : _capacity { capacity }, _buffer { _alloc.allocate(capacity) }
{}

template <typename T>
deque <T>::deque(size_type capacity, const T &value)
        : deque(capacity)
{
    _begin = _buffer;
    _end = _begin + capacity - 1;
    std::uninitialized_fill(_begin, _end + capacity, value);
}

template <typename T>
deque <T>::deque(const deque &other)
{
    _capacity = other._capacity;
    _buffer = _alloc.allocate(other._capacity);
    std::copy(other._buffer, other._buffer + _capacity, _buffer);
    _begin = _buffer + (other._begin - other._buffer);
    _end = _buffer + (other._end - other._buffer);
}

template <typename T>
deque <T>::deque(deque &&other) noexcept
{
    swap(other);
}

template <typename T>
deque <T>::deque(std::initializer_list <T> il)
        : deque(il.size())
{
    _begin = _buffer;
    _end = _begin + _capacity - 1;
    std::uninitialized_copy(std::cbegin(il), std::cend(il), _begin);
}

template <typename T>
deque <T>::~deque()
{
    _deallocate();
}

template <typename T>
deque <T> &deque <T>::operator=(const deque &other)
{
    if (this != &other) {
        // first destruct
        _deallocate();
        // then copy other deque
        _capacity = other._capacity;
        _buffer = _alloc.allocate(other._capacity);
        std::copy(other._buffer, other._buffer + _capacity, _buffer);
        _begin = _buffer + (other._begin - other._buffer);
        _end = _buffer + (other._end - other._buffer);
    }
    return *this;
}

template <typename T>
deque <T> &deque <T>::operator=(deque &&other) noexcept
{
    swap(other);
    other._deallocate();
    return *this;
}

template <typename T>
deque <T> &deque <T>::operator=(std::initializer_list <T> il)
{
    // first destructor
    _deallocate();
    // then copy initializer list
    _buffer = _alloc.allocate(il.size());
    _capacity = il.size();
    _begin = _buffer;
    _end = _begin + _capacity - 1;
    std::uninitialized_copy(std::cbegin(il), std::cend(il), _begin);
    return *this;
}

template <typename T>
auto deque <T>::operator[](size_type pos) -> reference
{
    size_type index = 0;
    auto it = _begin;
    while (index < pos) {
        it = _next(it);
        index += 1;
    }
    return *it;
}

template <typename T>
auto deque <T>::at(size_type pos) -> reference
{
    if (pos >= size() || pos < 0) {
        throw std::invalid_argument("Index out of bounds.");
    }
    return (*this)[pos];
}

template <typename T>
auto deque <T>::back() -> reference
{
    T *elem = _end;
    _assert_not_null(elem, "Can not back() an empty container.");
    return *elem;
}

template <typename T>
auto deque <T>::front() -> reference
{
    T *elem = _begin;
    _assert_not_null(elem, "Can not front() an empty container.");
    return *elem;
}

template <typename T>
bool deque <T>::empty() const noexcept
{
    return _begin == nullptr;
}

template <typename T>
auto deque <T>::size() const noexcept -> size_type
{
    auto it = _begin;
    size_type size = it == nullptr ? 0 : 1;
    while (it != _end) {
        size += 1;
        it = _next(it);
    }
    return size;
}

template <typename T>
void deque <T>::clear() noexcept
{
    auto it = _begin;
    while (it != _end) {
        _alloc.destroy(it);
        it = _next(it);
    }
    _begin = nullptr;
    _end = nullptr;
}

template <typename T>
void deque <T>::push_back(const T &value)
{
    push_back(std::move(value));
}

template <typename T>
void deque <T>::push_back(T &&value)
{
    if (empty()) {
        _begin = _buffer;
        _end = _buffer;
    } else {
        auto it = _next(_end);
        if (it == _begin) {
            resize(_capacity * 2);
        }
        _end = _next(_end);
    }
    _alloc.construct(_end, std::move(value));
}

template <typename T>
void deque <T>::pop_back()
{
    if (!empty()) {
        _alloc.destroy(_end);
        if (_begin == _end) {
            _begin = _end = nullptr;
        } else {
            _end = _previous(_end);
        }
    }
}

template <typename T>
void deque <T>::push_front(const T &value)
{
    push_front(std::move(value));
}

template <typename T>
void deque <T>::push_front(T &&value)
{
    if (empty()) {
        push_back(value);
    } else {
        auto it = _previous(_begin);
        if (it == _end) {
            resize(_capacity * 2);
        }
        _begin = _previous(_begin);
    }
    _alloc.construct(_begin, std::move(value));
}

template <typename T>
void deque <T>::pop_front()
{
    if (!empty()) {
        _alloc.destroy(_begin);

        if (_begin == _end) {
            _end = nullptr;
            _begin = nullptr;
        } else {
            _begin = _next(_begin);
        }
    }
}

template <typename T>
void deque <T>::_deallocate()
{
    if (_buffer != nullptr) {
        // If the buffer still holds fully unconstructed
        // memory, _alloc.destroy() is an invalid op!
        if (_end != nullptr) {
            size_type offset = 0;
            while (offset < _capacity) {
                _alloc.destroy(_buffer + offset);
                offset += 1;
            }
            _begin = nullptr;
            _end = nullptr;
        }
        _alloc.deallocate(_buffer, _capacity);
        _buffer = nullptr;
        _capacity = 0;
    }
}

template <typename T>
void deque <T>::_assert_not_null(const T *p, const std::string &message)
{
    if (p == nullptr) {
        throw std::range_error(message);
    }
}

template <typename T>
T *deque <T>::_next(const T *it) const
{
    return _buffer + ((it - _buffer + 1) % _capacity);
}

template <typename T>
T *deque <T>::_previous(const T *it) const
{
    return _buffer + ((it - _buffer - 1 + _capacity) % _capacity);
}

template <typename T>
void deque <T>::resize(deque::size_type count)
{
    // allocate a second buffer
    T *new_buff = _alloc.allocate(count);

    // write old values to new buffer
    auto it = _begin;
    auto new_buff_cpy = new_buff;
    while (it != _end) {
        _alloc.construct(new_buff_cpy++, *it);
        it = _next(it);
    }

    // delete the old buffer
    auto s = size(); // store old size
    auto buff_it = _begin;
    while (buff_it != _end) {
        _alloc.destroy(buff_it);
        buff_it = _next(buff_it);
    }
    _alloc.deallocate(_buffer, _capacity);

    // reassign pointy bois
    _buffer = new_buff;
    _begin = _buffer;
    _end = _begin + s - 1;

    _capacity = count;
}

template <typename T>
void deque <T>::swap(deque &other) noexcept
{
    using std::swap;
    swap(_alloc, other._alloc);
    swap(_buffer, other._buffer);
    swap(_begin, other._begin);
    swap(_end, other._end);
    swap(_capacity, other._capacity);
}

template <typename T>
auto deque <T>::begin() noexcept -> deque <T>::iterator
{
    return deque<T>::iterator(*this, _begin);
}

template <typename T>
auto deque <T>::end() noexcept -> deque <T>::iterator
{
    return iterator(*this, _end);
}

template <typename T>
bool operator==(deque <T> const &lhs, deque <T> const &rhs)
{
    bool equal = false;
    if (lhs.size() == rhs.size()) {
        equal = true;
        auto lhs_it = lhs.begin();
        auto rhs_it = rhs.begin();
        while (lhs_it != lhs.end() && equal) {
            equal = *lhs_it++ == *rhs_it++;
        }
    }
    return equal;
}

template <typename T>
bool operator!=(deque <T> const &lhs, deque <T> const &rhs)
{
    return !(lhs == rhs);
}

template <typename T>
bool operator<(deque <T> const &lhs, deque <T> const &rhs)
{
    bool less = true;
    auto lhs_it = lhs.begin();
    auto rhs_it = rhs.begin();
    while (lhs_it != lhs.end() && rhs_it != rhs.end() && less) {
        less = *lhs_it++ < *rhs_it++;
    }
    return less;
}

template <typename T>
bool operator<=(deque <T> const &lhs, deque <T> const &rhs)
{
    return lhs < rhs || lhs == rhs;
}

template <typename T>
bool operator>(deque <T> const &lhs, deque <T> const &rhs)
{
    return !(lhs <= rhs);
}

template <typename T>
bool operator>=(deque <T> const &lhs, deque <T> const &rhs)
{
    return !(lhs < rhs);
}

template <typename T>
void swap(deque <T> &lhs, deque <T> &rhs)
{
    lhs.swap(rhs);
}

} // namespace swo