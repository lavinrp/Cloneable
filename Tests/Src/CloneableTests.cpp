
#include <lagy/Cloneable.hpp>

#include <catch2/catch_all.hpp>

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

		// TODO: Add explicit tests with and without this.
		using Cloneable<Base>::Clone;
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

TEST_CASE("SimpleClone")
{
	std::unique_ptr<lagyTest::Derived> orig = std::make_unique<lagyTest::Derived>();

	std::unique_ptr<lagyTest::Derived> clone = orig->Clone();
}


TEST_CASE("DiamondClone")
{
	std::unique_ptr<lagyTest::DiamondDerived> orig = std::make_unique<lagyTest::DiamondDerived>();

	std::unique_ptr<lagyTest::DiamondDerived> cloneDeduced = orig->Clone();
	//std::unique_ptr<lagyTest::DiamondDerived> cloneDeduced = orig->Cloneable::Clone();
	std::unique_ptr<lagyTest::DiamondDerived> clone = orig->Cloneable<lagyTest::Base>::Clone();
	std::unique_ptr<lagyTest::DiamondDerived> clone2 = orig->Cloneable<lagyTest::Base2>::Clone();
}



TEST_CASE("SimpleCloneAs")
{
	std::unique_ptr<lagyTest::Derived> orig = std::make_unique<lagyTest::Derived>();

	std::unique_ptr<lagyTest::Base> clonedAsBase = orig->CloneAs<lagyTest::Base>();

	// TODO: Handle Negative Tests
	// std::unique_ptr<lagyTest::Derived> clonedBaseAsDerived = clonedAsBase->CloneAs<lagyTest::Derived>();
}

TEST_CASE("DiamondCloneAs")
{
	std::unique_ptr<lagyTest::DiamondDerived> orig = std::make_unique<lagyTest::DiamondDerived>();

	std::unique_ptr<lagy::Cloneable<lagyTest::Base>> clonedAsCloneable = orig->Cloneable::CloneAs<lagy::Cloneable<lagyTest::Base>>();
	std::unique_ptr<lagyTest::Base> clonedAsBase = orig->Cloneable::CloneAs<lagyTest::Base>();
	std::unique_ptr<lagyTest::Base2> clonedAsBase2 = orig->Cloneable::CloneAs<lagyTest::Base2>();
	std::unique_ptr<lagyTest::DiamondDerived> clonedAsDerived = orig->Cloneable::CloneAs<lagyTest::DiamondDerived>();

	// TODO: Handle Negative Tests
	// std::unique_ptr<lagyTest::DiamondDerived> clonedBaseAsDerived = clonedAsBase->Cloneable::CloneAs<lagyTest::DiamondDerived>();
	// std::unique_ptr<lagyTest::Base> clonedBase2AsBase = clonedAsBase2->Cloneable::CloneAs<lagyTest::Base>();
}