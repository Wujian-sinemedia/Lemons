#pragma once

namespace bav::events
{

class Broadcaster
{
public:
    
    struct Reciever
    {
        Reciever (Broadcaster& b): broadcaster(b)
        {
            broadcaster.addListener (this);
        }
        
        virtual ~Reciever()
        {
            broadcaster.removeListener (this);
        }
        
        virtual void callback() = 0;
        
    private:
        Broadcaster& broadcaster;
    };
    
    void trigger()
    {
        listeners.call ([](Reciever& l){ l.callback(); });
    }
    
private:
    friend Reciever;
    
    void addListener (Reciever* l) { listeners.add (l); }
    void removeListener (Reciever* l) { listeners.remove (l); }
    
    juce::ListenerList<Reciever> listeners;
};

}  // namespace
