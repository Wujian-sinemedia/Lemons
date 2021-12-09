#pragma once

namespace lemons
{

/** A wrapper around juce::OwnedArray that automatically deletes or creates objects when you resize it.
 */
template <typename ObjectType>
struct ConstructedArray final
{
	using ObjectCreationFunction = std::function<ObjectType*()>;

	/** Creates an array with a specified initial number of objects, and an optional lambda function to create new objects. */
	ConstructedArray (
	    int                      initialNumObjects  = 0,
	    ObjectCreationFunction&& objectCreationFunc = []()
	    { return new ObjectType(); })
	    : creationFunc (std::move (objectCreationFunc))
	{
		resize (initialNumObjects);
	}

	/** Destructor. */
	~ConstructedArray() = default;

	/** Resizes the array.
	    If the array contains more objects than newNumObjects, the extra objects will be deleted. If the array contains fewer objects, new objects will be added to the array, created using the lambda function passed to the constructor.
	 */
	void resize (int newNumObjects)
	{
		while (array.size() < newNumObjects)
			array.add (creationFunc());

		while (array.size() > newNumObjects)
			array.removeLast();
	}

	/** Returns a pointer to the underlying juce::OwnedArray. */
	juce::OwnedArray<ObjectType>* operator->() noexcept { return &array; }

	/** Returns a pointer to the underlying juce::OwnedArray. */
	const juce::OwnedArray<ObjectType>* operator->() const noexcept { return &array; }

	/** Internally calls OwnedArray::getUnchecked to return an item at a specified index in the array. */
	ObjectType* operator[] (int index) const noexcept { return array.getUnchecked (index); }

	/** Calls OwnedArray::begin. */
	ObjectType** begin() noexcept { return array.begin(); }

	/** Calls OwnedArray::begin. */
	ObjectType* const* begin() const noexcept { return array.begin(); }

	/** Calls OwnedArray::end. */
	ObjectType** end() noexcept { return array.end(); }

	/** Calls OwnedArray::end. */
	ObjectType* const* end() const noexcept { return array.end(); }

	/** The managed array. */
	juce::OwnedArray<ObjectType> array;

private:
	ObjectCreationFunction creationFunc;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConstructedArray)
};

}  // namespace lemons
