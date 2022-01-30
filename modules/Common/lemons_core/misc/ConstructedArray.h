
/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 *  ======================================================================================
 */

#pragma once

namespace lemons
{

/** @ingroup lemons_core
	A wrapper around juce::OwnedArray that automatically deletes or creates objects when you resize it. For example:
	@code
	struct MyStruct
	{
		MyStruct (SomeObject& arg);

		void doSomething();
	};

	SomeObject myObject;

	ConstructedArray<MyStruct> myStructs { 1, [&myObject]{ return new MyStruct(myObject) } };

	myStructs.resize (5);

	myStructs.for_each ([](MyStruct& s){ s.doSomething(); });

	myStructs.resize (1);
	@endcode
 */
template <typename ObjectType>
struct ConstructedArray final
{
	/** The function signature used to create new objects to add to the array. The function should return a pointer to the new object, and should not retain the pointer. */
	using ObjectCreationFunction = std::function<ObjectType*()>;

	/** Creates an array with a specified initial number of objects, and an optional lambda function that will be called to create new objects. */
	explicit ConstructedArray (
		int						 initialNumObjects	= 0,
		ObjectCreationFunction&& objectCreationFunc = []
		{ return new ObjectType(); })
		: creationFunc (std::move (objectCreationFunc))
	{
		resize (initialNumObjects);
	}

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

	/** Calls a function with each object in the array. */
	void for_each (std::function<void (ObjectType&)> func)
	{
		for (auto* obj : array)
			func (*obj);
	}

	/** Adds a single object to the end of the array, and returns a reference to it. */
	ObjectType& append()
	{
		return *array.add (creationFunc());
	}

	/** Returns a pointer to the underlying juce::OwnedArray. */
	juce::OwnedArray<ObjectType>* operator->() noexcept { return &array; }

	/** Returns a pointer to the underlying juce::OwnedArray. */
	const juce::OwnedArray<ObjectType>* operator->() const noexcept { return &array; }

	/** Internally calls OwnedArray::getUnchecked to return an item at a specified index in the array. */
	[[nodiscard]] ObjectType* operator[] (int index) const noexcept { return array.getUnchecked (index); }

	/** Calls OwnedArray::begin. */
	[[nodiscard]] ObjectType** begin() noexcept { return array.begin(); }

	/** Calls OwnedArray::begin. */
	[[nodiscard]] ObjectType* const * begin() const noexcept { return array.begin(); }

	/** Calls OwnedArray::end. */
	[[nodiscard]] ObjectType** end() noexcept { return array.end(); }

	/** Calls OwnedArray::end. */
	[[nodiscard]] ObjectType* const * end() const noexcept { return array.end(); }

	/** The managed array. */
	juce::OwnedArray<ObjectType> array;

private:

	const ObjectCreationFunction creationFunc;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConstructedArray)
};

}  // namespace lemons
