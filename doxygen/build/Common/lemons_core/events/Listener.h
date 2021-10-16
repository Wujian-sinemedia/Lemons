
/** @ingroup events
 *  @{
 */
#pragma once

namespace lemons::events
{
struct Listener : Broadcaster::Reciever
{
    Listener (Broadcaster& b, std::function< void() > func);

private:
    void callback() final;

    std::function< void() > callbackFunc;
};

}  // namespace lemons::events

/** @}*/
