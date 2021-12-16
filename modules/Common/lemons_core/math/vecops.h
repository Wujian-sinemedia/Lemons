/*
 ======================================================================================
 
 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 
 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 
 ======================================================================================
 */

#pragma once

namespace lemons::vecops
{
/* fills a vector with the specified value. */
template <typename Type>
void fill (Type* vector, Type value, int count);


/* copies the contents of one vector to another. */
template <typename Type>
void copy (const Type* const source, Type* const dest, int count);


/* copies each value of src into dst. The vectors may have different value types. If they are the same type, this is the same as using copy */
template <typename Type1, typename Type2>
void convert (Type1* const dst, const Type2* const src, int count);


/* adds a single operand to each value in the vector */
template <typename Type>
void addC (Type* vector, Type value, int count);


/* performs element-wise addition of two vectors and writes the output to vecA */
template <typename Type>
void addV (Type* vecA, const Type* vecB, int count);


/* subtracts a single operand from every element in the vector */
template <typename Type>
void subtractC (Type* vector, Type value, int count);


/* performs element-wise subtraction of two vectors and writes the output to vecA */
template <typename Type>
void subtractV (Type* vecA, const Type* vecB, int count);


/* multiplies every element in the vector by a single operand */
template <typename Type>
void multiplyC (Type* vector, Type value, int count);


/* performs element-wise multiplication of two vectors and writes the output to vecA */
template <typename Type>
void multiplyV (Type* vecA, const Type* vecB, int count);


/* divides every element in the vector by a single operand */
template <typename Type>
void divideC (Type* vector, Type value, int count);


/* performs element-wise division of two vectors and writes the output to vecA */
template <typename Type>
void divideV (Type* vecA, const Type* vecB, int count);


/* replaces every element in the passed vector with its square root */
template <typename Type>
void squareRoot (Type* data, int dataSize);


/* replaces every element in the passed vector with its square */
template <typename Type>
void square (Type* data, int dataSize);


/* replaces every element in the passed vector with its absolute value */
template <typename Type>
void absVal (Type* data, int dataSize);


/* returns the index in the vector of the minimum element */
template <typename Type>
[[nodiscard]] int findIndexOfMinElement (const Type* data, int dataSize);

template <typename Type>
[[nodiscard]] int findIndexOfMinElement (const juce::Array<Type>& array)
{
	return findIndexOfMinElement (array.getRawDataPointer(), array.size());
}


/* returns the index in the vector of the maximum element */
template <typename Type>
[[nodiscard]] int findIndexOfMaxElement (const Type* data, int dataSize);

template <typename Type>
[[nodiscard]] int findIndexOfMaxElement (const juce::Array<Type>& array)
{
	return findIndexOfMaxElement (array.getRawDataPointer(), array.size());
}


/* returns both the minimum element and its index in the vector into the variables minimum and minIndex */
template <typename Type>
void findMinAndMinIndex (const Type* data,
                         int         dataSize,
                         Type&       minimum,
                         int&        minIndex);

template <typename Type>
void findMinAndMinIndex (const juce::Array<Type>& array, Type& minimum, int& minIndex)
{
	findMinAndMinIndex (array.getRawDataPointer(), array.size(), minimum, minIndex);
}


/* returns both the maximum element and its index in the vector into the variables maximum and maxIndex */
template <typename Type>
void findMaxAndMaxIndex (const Type* data,
                         int         dataSize,
                         Type&       maximum,
                         int&        maxIndex);

template <typename Type>
void findMaxAndMaxIndex (const juce::Array<Type>& array, Type& maximum, int& maxIndex)
{
	findMaxAndMaxIndex (array.getRawDataPointer(), array.size(), maximum, maxIndex);
}


/* locates the element with the highest absolute value and its index in the vector, and returns them into the variables greatestMagnitude and index */
template <typename Type>
void locateGreatestAbsMagnitude (const Type* data,
                                 int         dataSize,
                                 Type&       greatestMagnitude,
                                 int&        index);

template <typename Type>
void locateGreatestAbsMagnitude (const juce::Array<Type>& array, Type& greatestMagnitude, int& index)
{
	locateGreatestAbsMagnitude (array.getRawDataPointer(), array.size(), greatestMagnitude, index);
}


/* locates the element with the lowest absolute value and its index in the vector, and returns them into the variables leastMagnitude and index */
template <typename Type>
void locateLeastAbsMagnitude (const Type* data,
                              int         dataSize,
                              Type&       leastMagnitude,
                              int&        index);

template <typename Type>
void locateLeastAbsMagnitude (const juce::Array<Type>& array, Type& leastMagnitude, int& index)
{
	locateLeastAbsMagnitude (array.getRawDataPointer(), array.size(), leastMagnitude, index);
}


/* finds both the maximum and minimum elements in the vector and returns them into the variables max and min */
template <typename Type>
void findExtrema (const Type* data, int dataSize, Type& min, Type& max);

template <typename Type>
void findExtrema (const juce::Array<Type>& array, Type& min, Type& max)
{
	findExtrema (array.getRawDataPointer(), array.size(), min, max);
}


/* returns the distance between the maximum and minimum element of the vector */
template <typename Type>
[[nodiscard]] Type findRangeOfExtrema (const Type* data,
                                       int         dataSize);

template <typename Type>
[[nodiscard]] Type findRangeOfExtrema (const juce::Array<Type>& array)
{
	return findRangeOfExtrema (array.getRawDataPointer(), array.size());
}


/* Normalises a set of samples to the absolute maximum contained within the buffer. */
template <typename Type>
void normalize (Type* vector, int size);

template <typename Type>
void normalize (juce::Array<Type>& array)
{
	normalize (array.getRawDataPointer(), array.size());
}


extern constexpr bool isUsingVDSP();

extern constexpr bool isUsingFallback();


}  // namespace lemons::vecops
