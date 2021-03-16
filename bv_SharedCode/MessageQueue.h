
namespace bav
{

    class MessageQueue
    {
    public:
        MessageQueue() { }
        
        ~MessageQueue() { }
        
        
        class Message
        {
        public:
            Message(int msgID, float newValue): typeID(msgID), val(newValue) { }
            
            ~Message() { }
            
            int type() const { return typeID; }
            
            float value() const { return val; }
            
        private:
            int typeID;
            float val;
        };  // class message
        
        
        
        void pushMessage (int typeID, float value)
        {
            pushMessage (Message(typeID, value));
        }
        
        
        void pushMessage (Message message)
        {
            messages.add (message);
        }
        
        
        Message popMessage()
        {
            return messages.removeAndReturn (0);
        }
        
        
        int numStoredMessages() const
        {
            return messages.size();
        }
        
        
        bool isEmpty() const
        {
            return messages.isEmpty();
        }
        
        
        void clear()
        {
            messages.clearQuick();
        }
        
        
        void reserveSize (int numMessages)
        {
            messages.ensureStorageAllocated (numMessages);
        }

        
        
    private:
        
        juce::Array<Message> messages;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MessageQueue);
    };
    
}  // namespace bav
