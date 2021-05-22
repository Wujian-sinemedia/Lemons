
namespace bav::intops
{
/*
        This namespace contains several floating inlined functions that provide a lot of similar functionality to the bav::vecops functions, but for integer data.
    */


/* fills a vector with the specified value. */
extern void
    fill (int* BV_R_ vector, const int value, const int count);


/* copies the contents of one vector to another. */
extern void
    copy (const int* const BV_R_ source, int* const BV_R_ dest, const int count);


/* adds a single operand to each value in the vector */
extern void
    addC (int* BV_R_ vector, const int value, const int count);


/* performs element-wise addition of two vectors and writes the output to vecA */
extern void
    addV (int* BV_R_ vecA, const int* BV_R_ vecB, const int count);


/* subtracts a single operand from every element in the vector */
extern void
    subtractC (int* BV_R_ vector, const int value, const int count);


/* performs element-wise subtraction of two vectors and writes the output to vecA */
extern void
    subtractV (int* BV_R_ vecA, const int* BV_R_ vecB, const int count);


/* multiplies every element in the vector by a single operand */
extern void
    multiplyC (int* BV_R_ vector, const int value, const int count);


/* performs element-wise multiplication of two vectors and writes the output to vecA */
extern void
    multiplyV (int* BV_R_ vecA, const int* BV_R_ vecB, const int count);


/* divides every element in the vector by a single operand */
extern void
    divideC (int* BV_R_ vector, const int value, const int count);

/* performs element-wise division of two vectors and writes the output to vecA */
extern void
    divideV (int* BV_R_ vecA, const int* BV_R_ vecB, const int count);


/* replaces every element in the passed vector with its square */
extern void square (int* BV_R_ data, const int dataSize);


/* replaces every element in the passed vector with its absolute value */
extern void absVal (int* BV_R_ data, const int dataSize);


// returns the minimum element in the dataset
extern int findMinElement (int* data, int dataSize);


// returns the maximum element in the dataset
extern int findMaxElement (int* data, int dataSize);


// returns the index in the dataset of the minimum datum
extern int findIndexOfMinElement (int* data, int dataSize);


// returns the index in the dataset of the maximum datum
extern int findIndexOfMaxElement (int* data, int dataSize);


// returns both the minimum element and its index in the dataset into the variables minimum and minIndex
extern void
    findMinAndMinIndex (int* data, const int dataSize, int& minimum, int& minIndex);


// returns both the maximum element and its index in the dataset into the variables maximum and maxIndex
extern void
    findMaxAndMaxIndex (int* data, const int dataSize, int& maximum, int& maxIndex);


/* locates the element with the highest absolute value and its index in the vector, and returns them into the variables greatestMagnitude and index */
extern void locateGreatestAbsMagnitude (const int* BV_R_ data,
                                        const int        dataSize,
                                        int&             greatestMagnitude,
                                        int&             index);


/* locates the element with the lowest absolute value and its index in the vector, and returns them into the variables leastMagnitude and index */
extern void locateLeastAbsMagnitude (const int* BV_R_ data,
                                     const int        dataSize,
                                     int&             leastMagnitude,
                                     int&             index);


// finds both the maximum and minimum elements in the dataset and returns them into the variables max and min.
extern void
    findExtrema (int* data, const int dataSize, int& min, int& max);

//  returns the distance between the maximum and minimum elements in the dataset
extern int findRangeOfExtrema (int* data, const int dataSize);


}  // namespace bav::intops
