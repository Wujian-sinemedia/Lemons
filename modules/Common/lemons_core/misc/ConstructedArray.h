#pragma once

namespace lemons
{

template <typename ObjectType>
struct ConstructedArray final
{
	using ObjectCreationFunction = std::function<ObjectType*()>;

	ConstructedArray (
	    int                      initialNumObjects  = 0,
	    ObjectCreationFunction&& objectCreationFunc = []()
	    { return new ObjectType(); })
	    : creationFunc (std::move (objectCreationFunc))
	{
		resize (initialNumObjects);
	}

	~ConstructedArray() = default;

	void resize (int newNumObjects)
	{
		while (array.size() < newNumObjects)
			array.add (creationFunc());

		while (array.size() > newNumObjects)
			array.removeLast();
	}

	juce::OwnedArray<ObjectType>* operator->() noexcept { return &array; }

	const juce::OwnedArray<ObjectType>* operator->() const noexcept { return &array; }

	ObjectType* operator[] (int index) const noexcept { return array.getUnchecked (index); }

	ObjectType** begin() noexcept { return array.begin(); }

	ObjectType* const* begin() const noexcept { return array.begin(); }

	ObjectType** end() noexcept { return array.end(); }

	ObjectType* const* end() const noexcept { return array.end(); }


	juce::OwnedArray<ObjectType> array;

	ObjectCreationFunction creationFunc;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConstructedArray)
};

}  // namespace lemons
