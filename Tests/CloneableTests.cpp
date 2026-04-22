
#include <lagy/Cloneable.hpp>

#include <memory>

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

	class Base2 : public lagy::Cloneable<Base2> {
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

	class DiamondDerived : public Base, public Base2 {
	public:
		DiamondDerived() = default;
		~DiamondDerived() override = default;

		DiamondDerived* DoClone(lagy::Cloneable<Base>::CloneTag) const override {
			return new DiamondDerived(*this);
		}
		DiamondDerived* DoClone(lagy::Cloneable<Base2>::CloneTag) const override {
			return new DiamondDerived(*this);
		}
	private:
		DiamondDerived(const DiamondDerived&)
		{
		}
	};
}


int main()
{

	// Validate that Clone can return a unique_ptr<Derived>
	{
		std::unique_ptr<lagyTest::Derived> orig = std::make_unique<lagyTest::Derived>();
		std::unique_ptr<lagyTest::Derived> clone = orig->Clone();

		// CloneAs
		std::unique_ptr<lagyTest::Base> clonedAsBase = orig->Cloneable::CloneAs<lagyTest::Base>();
	}
	
	
	// Validate that the diamond inheritance problem is solved
	{
		std::unique_ptr<lagyTest::DiamondDerived> orig = std::make_unique<lagyTest::DiamondDerived>();
		std::unique_ptr<lagyTest::DiamondDerived> clone = orig->Cloneable<lagyTest::Base>::Clone();
		std::unique_ptr<lagyTest::DiamondDerived> cloneDeduced = orig->Cloneable::Clone();

		// CloneAs
		std::unique_ptr<lagyTest::Base> clonedAsBase = orig->Cloneable::CloneAs<lagyTest::Base>();
	}
}