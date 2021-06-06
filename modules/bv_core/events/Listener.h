#pragma once

namespace bav::events
{
struct Listener : Broadcaster::Reciever
{
    Listener (Broadcaster& b, std::function< void() > func)
        : Reciever (b),
          callbackFunc (std::move (func))
    {
    }

    virtual ~Listener() = default;

private:
    void callback() final
    {
        callbackFunc();
    }

    std::function< void() > callbackFunc;
};

}  // namespace bav::events
