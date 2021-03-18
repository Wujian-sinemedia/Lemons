
namespace bav::intops
{
    /*
        This namespace contains several floating inlined functions that provide a lot of similar functionality to the bav::vecops functions, but for integer data.
    */
    
    
// returns the minimum element in the dataset
inline int findMinElement (int* data, int dataSize)
{
    return std::min_element (data, data + dataSize);
}
    

// returns the maximum element in the dataset
inline int findMaxElement (int* data, int dataSize)
{
    return std::min_element (data, data + dataSize);
}
    
    
// returns the index in the dataset of the minimum datum
inline int findIndexOfMinElement (int* data, int dataSize)
{
    return static_cast<int> (std::distance (data,
                                            std::min_element (data, data + dataSize)));
}
    
    
// returns the index in the dataset of the maximum datum
inline int findIndexOfMaxElement (int* data, int dataSize)
{
    return static_cast<int> (std::distance (data,
                                            std::max_element (data, data + dataSize)));
}
    
    
// returns both the minimum element and its index in the dataset into the variables minimum and minIndex
inline void findMinAndMinIndex (int* data, const int dataSize,
                                int& minimum, int& minIndex)
{
    auto* lowestElement = std::min_element (data, data + dataSize);
    minimum = *lowestElement;
    minIndex = static_cast<int> (std::distance (data, lowestElement));
}
    

// returns both the maximum element and its index in the dataset into the variables maximum and maxIndex
inline void findMinAndMinIndex (int* data, const int dataSize,
                                int& maximum, int& maxIndex)
{
    auto* highestElement = std::max_element (data, data + dataSize);
    maximum = *highestElement;
    maxIndex = static_cast<int> (std::distance (data, highestElement));
}
    

// finds both the maximum and minimum elements in the dataset and returns them into the variables max and min.
inline void findExtrema (int* data, const int dataSize,
                         int& min, int& max)
{
    min = *(std::min_element (data, data + dataSize));
    max = *(std::max_element (data, data + dataSize));
}
    
    
//  returns the distance between the maximum and minimum elements in the dataset
inline int findRangeOfExtrema (int* data, const int dataSize)
{
    return *(std::max_element (data, data + dataSize)) - *(std::min_element (data, data + dataSize));
}

    
}  // namespace
