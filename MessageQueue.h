
namespace bav
{

    class MessageQueue
    {
    public:
        MessageQueue() { }
        
        ~MessageQueue() { }
        
        
        template<typename ValueType>
        class Message
        {
        public:
            Message() { }
            
            Message(juce::String type, ValueType newValue): messageType(type), val(newValue) { }
            
            ~Message() { }
            
            juce::String type() const { return messageType; }
            
            ValueType value() const { return val; }
            
        private:
            
            juce::String messageType;
            
            ValueType val;
            
        };  // class message
        
        
        template<typename ValueType>
        void pushMessage (Message<ValueType> message)
        {
            
        }
        
        template<typename ValueType>
        Message<ValueType> popMessage()
        {
            
        }
        
        int numStoredMessages() const
        {
            
        }

        
    private:
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MessageQueue)
    };
    
}  // namespace bav
