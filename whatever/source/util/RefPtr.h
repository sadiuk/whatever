#pragma once
#include <atomic>
#include <concepts>

namespace wtv
{
	class IReferenceCounted
	{
		std::atomic_uint32_t m_refCount = 0;
	public:
		uint32_t AddRef() { return m_refCount++; }
		uint32_t Release() { return m_refCount--; }
		virtual ~IReferenceCounted() {}
	};

	template<typename T>
	concept ReferenceCounted = std::derived_from<T, IReferenceCounted>;
	template<typename DerivedType, typename ThisType>
	concept DerivedFromThisOrSame = std::derived_from<DerivedType, ThisType>;
	template<typename BaseType, typename ThisType>
	concept BaseOfThis = std::derived_from<ThisType, BaseType>;
	template<typename Type>
	class RefPtr
	{
		template<typename AnyOtherRefPtr> friend class RefPtr;
		using this_t = RefPtr<Type>;
				
	public:
		RefPtr() = default;
		RefPtr(std::nullptr_t nil) {}
		RefPtr(const this_t& other)
		{
			other.m_pointer->AddRef();
			m_pointer = other.m_pointer;
		}
		RefPtr(this_t&& other)
		{
			m_pointer = std::move(other.m_pointer);
			other.m_pointer = nullptr;
		}
		RefPtr(Type* pointer)
		{
			if (pointer) pointer->AddRef();
			m_pointer = pointer;
		}

		template<DerivedFromThisOrSame<Type> D>
		this_t& operator=(RefPtr<D>&& other)
		{
			if (this->m_pointer == other.m_pointer)
			{
				other.m_pointer = nullptr;
				return *this;
			}
			ReleasePointer();
			m_pointer = other.m_pointer;
			other.m_pointer = nullptr;
			return *this;
		}

		template<DerivedFromThisOrSame<Type> D>
		this_t& operator=(const RefPtr<D>& other)
		{
			if (this->m_pointer == other.m_pointer) return *this;
			other.m_pointer->AddRef();
			ReleasePointer();
			m_pointer = other.m_pointer;
			return *this;
		}
		this_t& operator=(this_t&& other) noexcept
		{
			if (this->m_pointer == other.m_pointer)
			{
				other.m_pointer = nullptr;
				return *this;
			}
			ReleasePointer();
			m_pointer = other.m_pointer;
			other.m_pointer = nullptr;
			return *this;
		}

		this_t& operator=(const this_t& other)
		{
			if (this->m_pointer == other.m_pointer) return *this;
			other.m_pointer->AddRef();
			ReleasePointer();
			m_pointer = other.m_pointer;
			return *this;
		}

		template<BaseOfThis<Type> D>
		operator RefPtr<D>()
		{
			return RefPtr<D>(m_pointer);
		}

		Type* operator->()
		{
			return m_pointer;
		}

		Type* get() { return m_pointer; }
		const Type* get() const { return m_pointer; }

		~RefPtr()
		{
			ReleasePointer();
		}
	private:
		void ReleasePointer()
		{
			if (!m_pointer) return;
			if (m_pointer->Release() == 1)
			{
				delete m_pointer;
			}
		}
	private:
		Type* m_pointer = nullptr;
	};

	template<ReferenceCounted Type>
	RefPtr<Type> MakeRef(auto&&... args)
	{
		Type* ptr = new Type(std::forward<decltype(args)>(args)...);
		return RefPtr<Type>(ptr);
	}

	template<typename T2, typename T1>
	RefPtr<T2> StaticRefCast(RefPtr<T1>& ptr)
	{
		T2* rawPtr = static_cast<T2*>(ptr.get());
		return RefPtr<T2>(rawPtr);
	}

	template<typename T2, typename T1>
	RefPtr<T2> StaticRefCast(RefPtr<T1>&& ptr)
	{
		T2* rawPtr = static_cast<T2*>(ptr.get());
		return RefPtr<T2>(rawPtr);
	}
}