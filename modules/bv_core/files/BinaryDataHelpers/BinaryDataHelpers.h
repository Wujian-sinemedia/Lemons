
#pragma once

#ifndef BV_HAS_BINARY_DATA
#    define BV_HAS_BINARY_DATA 0
#endif


#if BV_HAS_BINARY_DATA

#    include "BinaryData.h"


namespace bav
{

struct RawData
{
    explicit RawData (const char* fileToFind);
    
    const char* data = nullptr;
    int         size = 0;
};

/** To easily access some binary data:
 @code
 RawData binary {"File.wav"};
 doSomethingWithData (binary.data, binary.size);
 @end-code
 */

}  // namespace bav


#endif /* if BV_HAS_BINARY_DATA */
