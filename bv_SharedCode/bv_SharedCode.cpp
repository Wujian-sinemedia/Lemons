
#include "bv_SharedCode.h"


#if BV_USE_VDSP
  #include "math/vecops/vecops_vdsp.cpp"
#elif BV_USE_IPP
  #include "math/vecops/ipp.cpp"
#else
  #include "math/vecops/fallback.cpp"
#endif
