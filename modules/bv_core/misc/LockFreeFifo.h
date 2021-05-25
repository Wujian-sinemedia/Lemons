/*
    A simple lock-free, single-producer single-consumer FIFO.
*/

#pragma once

namespace bav
{
/* DataType must be default-constructable. */
template < typename DataType, size_t size >
class FIFO
{
public:
    FIFO()
        : head (0), tail (0)
    {
    }

    ~FIFO() = default;

    bool push (DataType item)
    {
        const auto currentTail = tail.load();
        const auto nextTail    = nextIndex (currentTail);

        if (nextTail == head.load()) return false;

        data[currentTail] = std::move (item);
        tail.store (nextTail);
        return true;
    }

    DataType pop()
    {
        const auto currentHead = head.load();

        if (currentHead == tail.load()) return DataType();

        auto item = data[currentHead];
        head.store (nextIndex (currentHead));

        return item;
    }

    bool isEmpty() const { return tail == head; }

    void clearAll()
    {
        while (! isEmpty())
            pop();
    }


private:
    size_t nextIndex (const size_t index) const { return (index + 1) % (size + 1); }

    size_t previousIndex (const size_t index) const
    {
        return (index + size) % (size + 1);
    }

    std::atomic< size_t >            head;
    std::atomic< size_t >            tail;
    std::array< DataType, size + 1 > data;
};

}  // namespace bav
