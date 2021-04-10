
namespace bav
{

    /*
        A basic FIFO queue for messages that are keyed with an integer ID and can hold a float value.
    */
    
    class MessageQueue
    {
    public:
        
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
            
            bool isValid() const noexcept { return typeID != -1; }
            
        private:
            int typeID;  // the value -1 is used to represent an invalid message. Theoretically, other negative values could be used as valid keys
            float val;
        };
        
        
        MessageQueue(size_t maxNumMessages): fifo(maxNumMessages) { }
        
        ~MessageQueue() { }
        
        
        // adds a message to the FIFO with a specified type and value
        void pushMessage (int typeID, float value)
        {
            pushMessage (Message(typeID, value));
        }
        
        
        // adds a pre-constructed message object to the FIFO
        void pushMessage (Message message)
        {
            fifo.push (message);
        }
        
        
        // returns the next available message from the FIFO
        Message popMessage()
        {
            return fifo.pop();
        }
        
        
        bool isEmpty() const
        {
            return fifo.isEmpty();
        }
        
        
        void clear()
        {
            fifo.clearAll();
        }
        
        
        /*
            This function puts all the currently ready messgaes in the FIFO into the passed in array of Message objects.
        */
        void getReadyMessages (juce::Array<Message>& outputMessages)
        {
            outputMessages.clearQuick();
            
            while (! fifo.isEmpty())
                outputMessages.add (fifo.pop());
        }
        
        
    private:
        
        FIFO<Message> fifo;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MessageQueue)
    };
    
}  // namespace bav
