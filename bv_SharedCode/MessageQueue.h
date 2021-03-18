
namespace bav
{

    /*
        A basic FIFO queue for messages that are keyed with an integer ID and can hold a float value.
     
        TO DO: make this class thread-safe!!!
    */
    
    class MessageQueue
    {
    public:
        MessageQueue() { }
        
        ~MessageQueue() { }
        
        
        /*
            The FIFO queue stores a collection of these message objects
        */
        class Message
        {
        public:
            Message(): typeID(-1), val(0.0f) { } // the default constructor creates an invalid message
            
            Message(int msgID, float newValue): typeID(msgID), val(newValue) { }
            
            ~Message() { }
            
            int type()     const noexcept { return typeID; }
            
            float value()  const noexcept { return val; }
            
            bool isValid() const noexcept { return typeID >= 0; }
            
        private:
            int typeID;  // the value -1 is used to represent an invalid message. Theoretically, other negative values could be used as valid keys
            float val;
        };
        
        
        // adds a message to the FIFO with a specified type and value
        void pushMessage (int typeID, float value)
        {
            pushMessage (Message(typeID, value));
        }
        
        
        // adds a pre-constructed message object to the FIFO
        void pushMessage (Message message)
        {
            messages.add (message);
        }
        
        
        // returns the next available message from the FIFO
        Message popMessage()
        {
            if (messages.isEmpty())
                return Message();  // returns an invalid message if the queue is empty
            
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
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MessageQueue)
    };
    
}  // namespace bav
