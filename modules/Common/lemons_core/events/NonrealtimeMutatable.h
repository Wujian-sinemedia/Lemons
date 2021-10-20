#pragma once

namespace lemons::serializing
{
template <class ObjectType>
class NonrealtimeMutatable
{
public:
	NonrealtimeMutatable() = default;

	explicit NonrealtimeMutatable (ObjectType& obj)
	    : storage (std::move (obj))
	{
	}

	template <typename... Args>
	explicit NonrealtimeMutatable (Args&&... args)
	    : storage (std::forward<Args> (args)...)
	{
	}

	~NonrealtimeMutatable()
	{
		jassert (pointer.load() != nullptr);  // Never delete this object while the realtime thread is holding the lock

		// Spin!
		while (pointer.load() == nullptr)
			;

		auto accquired = nonRealtimeLock.try_lock();

		((void) (accquired));
		jassert (accquired);  // <- you didn't call release on one of the non-realtime threads before deleting this object

		nonRealtimeLock.unlock();
	}

	/*------------------------------------------------------------------------------------*/

	struct RealtimeAccess
	{
	public:
		~RealtimeAccess() noexcept
		{
			parent.realtimeRelease();
		}

		const ObjectType& object;

		const ObjectType* operator->() { return &object; }
		const ObjectType& operator()() { return object; }

	private:
		friend class NonrealtimeMutatable;

		RealtimeAccess (NonrealtimeMutatable& p)
		    : object (p.realtimeAcquire())
		    , parent (p)
		{
		}

		NonrealtimeMutatable& parent;
	};

	RealtimeAccess access_realtime()
	{
		return { *this };
	}

	/*------------------------------------------------------------------------------------*/

	struct NonrealtimeAccess
	{
	public:
		~NonrealtimeAccess()
		{
			parent.nonrealtimeRelease();
		}

		ObjectType& object;

		ObjectType* operator->() { return &object; }
		ObjectType& operator()() { return object; }

	private:
		friend class NonrealtimeMutatable;

		NonrealtimeAccess (NonrealtimeMutatable& p)
		    : object (p.nonrealtimeAcquire())
		    , parent (p)
		{
		}

		NonrealtimeMutatable& parent;
	};

	NonrealtimeAccess access_nonrealtime()
	{
		return { *this };
	}

	/*------------------------------------------------------------------------------------*/

private:
	const ObjectType& realtimeAcquire() noexcept
	{
		jassert (pointer.load() != nullptr);  // You didn't balance your acquire and release calls!
		currentObj = pointer.exchange (nullptr);
		return *currentObj;
	}

	void realtimeRelease() noexcept
	{
		jassert (pointer.load() == nullptr);  // You didn't balance your acquire and release calls!
		pointer.store (currentObj);
	}

	ObjectType& nonrealtimeAcquire()
	{
		nonRealtimeLock.lock();
		copy.reset (new ObjectType (*storage));
		return *copy.get();
	}

	void nonrealtimeRelease()
	{
		ObjectType* ptr;

		// block until realtime thread is done using the object
		do
		{
			ptr = storage.get();
		} while (! pointer.compare_exchange_weak (ptr, copy.get()));

		storage = std::move (copy);
		nonRealtimeLock.unlock();
	}

	/*------------------------------------------------------------------------------------*/

	std::unique_ptr<ObjectType> storage;
	std::unique_ptr<ObjectType> copy;
	std::atomic<ObjectType*>    pointer { storage.get() };
	ObjectType*                 currentObj { nullptr };
	std::mutex                  nonRealtimeLock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NonrealtimeMutatable)
};

}  // namespace lemons::serializing
