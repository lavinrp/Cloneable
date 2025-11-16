#pragma once

#include <memory>


namespace lagy {
	// Use a template as CRTP-lite such that we can have each Cloneable have a unique name.
	template <typename CrtpBase>
	class Cloneable
	{
	protected:
		// Define a TagDispatch tag to allow calling of DoClone
		// Note: Since this is defined in "protected", it is behaving halfway like a Passkey
		struct CloneTag {};
	public:
		virtual Cloneable* DoClone(CloneTag) const = 0;

		template <typename T>
		std::unique_ptr<T> Clone(this const T& self)
		{
			return std::unique_ptr<T>{self.DoClone(CloneTag{})};
		}

		// TODO: CloneAs?
		/*
		// TODO: Add concepts for validation that T has a DoClone? that returns the appropriate type "T*"
		template <typename T>
		std::unique_ptr<T> CloneAs()
		{
			return std::unique_ptr<T>{static_cast<const T*>(this).DoClone(CloneTag{})};
		}*/

	};
}


// TEST CODE
namespace lagyTest {
	class Base : public lagy::Cloneable<Base> {
	public:
		Base() = default;
		virtual ~Base() = default;
		Base(const Base&) = delete;
		Base(const Base&&) = delete;
	private:
	};

	class Base2 : public lagy::Cloneable<Base> {
	public:
		Base2() = default;
		virtual ~Base2() = default;
		Base2(const Base2&) = delete;
		Base2(const Base2&&) = delete;
	private:
	};

	class Derived : public Base {
	public:
		Derived() = default;
		~Derived() override = default;
		Derived* DoClone(CloneTag) const override {
			return new Derived(*this);
		}
	private:
		Derived(const Derived&)
		{
		}
	};

	//class DiamondDerived : public Base, public Base2{
	//public:
	//	DiamondDerived() = default;
	//	~DiamondDerived() override = default;
	//	DiamondDerived* DoClone(lagy::Cloneable<Base>::CloneTag) const override {
	//		return new DiamondDerived(*this);
	//	}
	//	DiamondDerived* DoClone(CloneTag) const override {
	//		return new DiamondDerived(*this);
	//	}
	//private:
	//	DiamondDerived(const DiamondDerived&)
	//	{
	//	}
	//};
}

int main()
{

	// Validate that Clone can return a unique_ptr<Derived>
	{
		std::unique_ptr<lagyTest::Derived> orig = std::make_unique<lagyTest::Derived>();
		std::unique_ptr<lagyTest::Derived> clone = orig->Clone();
	}
	
	/*
	// Validate that the diamond inheritance problem is solved
	{
		std::unique_ptr<lagyTest::DiamondDerived> orig = std::make_unique<lagyTest::DiamondDerived>();
		std::unique_ptr<lagyTest::DiamondDerived> clone = orig->Cloneable<lagyTest::Base>::Clone();
		// std::unique_ptr<lagyTest::DiamondDerived> clone = orig->Cloneable::Clone();
	}*/
}