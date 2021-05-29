
#pragma once

namespace bav
{
struct SystemInitializer
{
    SystemInitializer()
    {
#if BV_USE_NE10
        ne10_init();
#endif

        initializeDefaultTranslations();
    }

    virtual ~SystemInitializer()
    {
    }
};


}  // namespace bav
