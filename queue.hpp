#ifndef JG_QUEUE_HPP
#define JG_QUEUE_HPP

#include <stdexcept>

namespace jg {

template <typename T>
class Queue
{
public:
    static size_t const DEFAULT_CAPACITY = 128;

    explicit Queue(size_t capacity = DEFAULT_CAPACITY);
    ~Queue();

    T const&    front() const;
    T const&    back() const;
    void        push(T const& item);
    void        pop();
    void        clear();
    size_t      size() const;
    size_t      capacity() const;
    void        setCapacity(size_t capacity);
    bool        empty() const;
    bool        full() const;

private:
    T**     m_storage;
    size_t  m_capacity;
    size_t  m_size;
    size_t  m_oldestPos;
    size_t  m_latestPos;
    size_t  m_newCapacity;
};

template <typename T> inline
Queue<T>::Queue(size_t capacity)
:   m_storage(NULL),
    m_size(0),
    m_oldestPos(0),
    m_latestPos(capacity - 1),
    m_newCapacity(0)
{
    setCapacity(capacity);
}

template <typename T> inline
Queue<T>::~Queue()
{
    if (m_storage != NULL)
    {
        clear();
        delete[] m_storage;
    }
}

template <typename T>
inline T const&
Queue<T>::front() const
{
    if (empty())
        throw std::range_error("Queue::front(): Looking up empty container.");
    else return *m_storage[m_oldestPos];
}

template <typename T>
inline T const&
Queue<T>::back() const
{
    if (empty())
        throw std::range_error("Queue::back(): Looking up empty container.");
    else return *m_storage[m_latestPos];
}

template <typename T>
inline void
Queue<T>::push(T const& item)
{
    if (full())
        throw std::range_error("Queue::push(): Pushing to a full container.");
    else
    {
        m_latestPos = (m_latestPos + 1) % m_capacity;
        m_storage[m_latestPos] = new T(item);
        ++m_size;
    }
}

template <typename T> inline
void
Queue<T>::pop()
{
    if (empty())
        throw std::range_error("Queue::pop(): Popping an empty container.");
    else
    {
        delete m_storage[m_oldestPos];
        m_oldestPos = (m_oldestPos + 1) % m_capacity;
        --m_size;
        if (m_size < m_newCapacity) setCapacity(m_newCapacity);
    }
}

template <typename T>
inline void
Queue<T>::clear()
{
    while (!empty()) pop();
}

template <typename T> inline size_t
Queue<T>::size() const { return m_size; }

template <typename T>
inline size_t
Queue<T>::capacity() const
{
    return (m_newCapacity == 0 ? m_capacity : m_newCapacity);
}

template <typename T>
inline void
Queue<T>::setCapacity(size_t capacity)
{
    if (capacity <= 0)
        throw std::invalid_argument("Queue::setCapacity(): Capacity must be \
greater than zero.");

    if (capacity < m_size)
    {
        m_newCapacity = capacity;
        return;
    }
    
    T** newStorage;
    try
    {
        newStorage = new T*[capacity];
    }
    catch(...)
    {
        throw std::runtime_error("Queue::Queue(): Unable to allocate \
storage.");
    }

    if (!empty())
    {
        if (m_latestPos < m_oldestPos)
        {
            std::copy(m_storage + m_oldestPos, m_storage + m_capacity,
                newStorage);
            std::copy(m_storage, m_storage + m_latestPos + 1,
                newStorage + m_capacity - m_oldestPos);
        }
        else
        {
            std::copy(m_storage + m_oldestPos, m_storage + m_latestPos + 1,
                newStorage);
        }
    }

    delete[] m_storage;
    m_storage       = newStorage;
    m_capacity      = capacity;
    m_newCapacity   = 0;
}

template <typename T> inline bool
Queue<T>::empty() const { return size() <= 0; }

template <typename T> inline bool
Queue<T>::full() const { return size() >=  capacity(); }

} // namespace jg

#endif // JG_QUEUE_HPP
