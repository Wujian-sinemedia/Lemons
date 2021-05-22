
namespace bav
{

class ApplicationBase  :    private GUIInitializer,
                            private SystemInitializer
{
public:
    ApplicationBase() { }
    
    virtual ~ApplicationBase() override { }
    
private:
};

};
