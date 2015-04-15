#ifndef JG_BUFFER_HPP
#define JG_BUFFER_HPP

#include <QtCore/QtCore>

#include "queue.hpp"
#include "spawner.hpp"

namespace jg {

template <typename T>
class Buffer : public QThread
{
public:
    static const size_t DEFAULT_CAPACITY = 128;
    
    Buffer(Spawner<T>* spawner, size_t capacity = DEFAULT_CAPACITY);
    explicit Buffer(size_t capacity = DEFAULT_CAPACITY);
    ~Buffer();

    size_t      capacity() const;
    void        setCapacity(size_t capacity);
    size_t      size() const;
    bool        empty() const;
    bool        full();
    T           next();
    void        pop();
    T const&    peek();
    T const&    peekLast();
    bool        buffering() const;
    bool        running() const;
    void        setSpawner(Spawner<T>* spawner);
    void        startBuffering();
    void        startBuffering(Spawner<T>* spawner);
    void        pause();
    void        stop();

private:
    Spawner<T>*     m_spawner;
    Queue<T>        m_queue;
    bool            m_buffering;
    bool            m_running;
    QMutex          m_queueMutex;
    QMutex          m_spawnerMutex;
    QMutex          m_bufferingMutex;
    QWaitCondition  m_bufferingWaitCondition;
    QWaitCondition  m_queueWaitCondition;

    void waitWhileEmpty();
    void waitWhileFullAndBuffering();
    void waitWhileNotBuffering();
    void run();
};

template <typename T> inline
Buffer<T>::Buffer(
    Spawner<T>* spawner,
    size_t      capacity
)
:   m_spawner(spawner),
    m_queue(capacity),
    m_buffering(false),
    m_running(false)
{/* Do nothing. */}

template <typename T> inline
Buffer<T>::Buffer(size_t capacity)
:   Buffer(NULL, capacity)
{/* Do nothing. */}

template <typename T> inline
Buffer<T>::~Buffer()
{
    stop();
}

template <typename T> inline size_t
Buffer<T>::capacity() const { return m_queue.capacity(); }

template <typename T>
inline void
Buffer<T>::setCapacity(size_t capacity)
{
    m_queueMutex.lock();
        m_queue.setCapacity(capacity);
    m_queueMutex.unlock();
}

template <typename T> inline size_t
Buffer<T>::size() const {return m_queue.size(); }

template <typename T> inline bool
Buffer<T>::empty() const { return m_queue.empty(); }

template <typename T>
inline bool
Buffer<T>::full()
{
    m_queueMutex.lock();
        bool b = m_queue.full();
    m_queueMutex.unlock();
    return b;
}

template <typename T>
inline T
Buffer<T>::next()
{
    T const n = peek();
    pop();
    return n;
}

template <typename T>
inline void
Buffer<T>::pop()
{
    waitWhileEmpty();
    m_queueMutex.lock();
        m_queue.pop();
    m_queueMutex.unlock();
    m_queueWaitCondition.wakeAll();
}

template <typename T>
inline T const&
Buffer<T>::peek()
{
    waitWhileEmpty();
    return m_queue.front();
}

template <typename T>
inline T const&
Buffer<T>::peekLast()
{
    waitWhileEmpty();
    m_queueMutex.lock();
        T const& n = m_queue.back();
    m_queueMutex.unlock();
    return n;
}

template <typename T> inline bool
Buffer<T>::buffering() const { return m_buffering; }

template <typename T> inline bool
Buffer<T>::running() const { return m_running; }

template <typename T>
inline void
Buffer<T>::setSpawner(Spawner<T>* spawner)
{
    m_spawnerMutex.lock();
        m_spawner = spawner;
    m_spawnerMutex.unlock();;
}

template <typename T>
inline void
Buffer<T>::startBuffering()
{
    if (m_spawner == NULL) throw std::runtime_error("Buffer::startBuffering(): \
Cannot start buffering withount a spawner.");
    if (!running())
    {
        m_running = true;
        start();
    }
    if (!buffering())
    {
        m_bufferingMutex.lock();
            m_buffering = true;
            m_bufferingWaitCondition.wakeAll();
        m_bufferingMutex.unlock();
    }
}

template <typename T>
inline void
Buffer<T>::startBuffering(Spawner<T>* spawner)
{
    setSpawner(spawner);
    startBuffering();
}

template <typename T>
inline void
Buffer<T>::pause()
{   
    m_bufferingMutex.lock();
        m_buffering = false;
    m_bufferingMutex.unlock();
    m_queueWaitCondition.wakeAll(); // Wake if waiting while full and buffering.
}

template <typename T>
inline void
Buffer<T>::stop()
{   
    m_running = false;
    m_bufferingWaitCondition.wakeAll();
    m_queueWaitCondition.wakeAll();
    wait();
    m_buffering = false;
    m_queue.clear();
}

template <typename T>
inline void
Buffer<T>::waitWhileEmpty()
{
    m_queueMutex.lock();
        while (empty() && buffering()) m_queueWaitCondition.wait(&m_queueMutex);
        if (!buffering())
                throw std::runtime_error("Buffer::waitWhileEmpty(): Not \
buffering. Waiting while the buffer is empty leads to a deadlock.");
    m_queueMutex.unlock();
}

template <typename T>
inline void
Buffer<T>::waitWhileFullAndBuffering()
{
    m_queueMutex.lock();
        if (m_queue.full() && buffering())
            m_queueWaitCondition.wait(&m_queueMutex);
    m_queueMutex.unlock();
}

template <typename T>
inline void
Buffer<T>::waitWhileNotBuffering()
{
    m_bufferingMutex.lock();
        if (!buffering()) m_bufferingWaitCondition.wait(&m_bufferingMutex);
    m_bufferingMutex.unlock();
}

template <typename T>
void
Buffer<T>::run()
{
    while (running())
    {
        waitWhileNotBuffering();
        waitWhileFullAndBuffering();    
        if(buffering() && !full())
        {
            m_spawnerMutex.lock();
                T const n = m_spawner->spawn();
            m_spawnerMutex.unlock();
            m_queueMutex.lock();
                m_queue.push(n);
            m_queueMutex.unlock();
            m_queueWaitCondition.wakeAll();
        }
    }
}

} // namespac jg

#endif // JG_BUFFER_HPP
