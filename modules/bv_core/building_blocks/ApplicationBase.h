
namespace bav
{

class ApplicationBase  :    private GUIInitializer,
                            private SystemInitializer
{
public:
    ApplicationBase()
    : SystemInitializer (getDefaultTranslationFile())
    { }
    
private:
};

};
