#pragma once


#include <memory>
#include <type_traits>

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

		// Option 1: Works? IDK.
		template <typename T>
		std::unique_ptr<T> Clone(this const T& self)
		{
			return std::unique_ptr<T>{self.DoClone(CloneTag{})};
		}
		
		/*
		// Option 2: Works.
		template <typename T>
		using NonConst = std::remove_cvref_t<T>;

		template <typename Self>
		std::unique_ptr<NonConst<Self>> Clone(this Self&& self)
		{
			return std::unique_ptr<NonConst<Self>>{self.DoClone(CloneTag{})};
		}
		*/

		/*
		// Option 3: Works. Odd default parameter.
		template <typename Self, typename NonConstSelf = std::remove_cvref_t<Self>>
		std::unique_ptr<NonConstSelf> Clone(this Self&& self)
		{
			return std::unique_ptr<NonConstSelf>{self.DoClone(CloneTag{})};
		}
		*/

		/*
		// Option 4: Works. Crazy const cast.
		template <typename T>
		std::unique_ptr<T> Clone(this const T& self)
		{
			using NonConstSelf = std::remove_cvref_t<T>;
			NonConstSelf& nonConstSelf = const_cast<NonConstSelf&>(self);
			return std::unique_ptr<T>{nonConstSelf.DoClone(CloneTag{})};
			//return std::unique_ptr<T>{const_cast<std::remove_cvref_t<T>&>(self).DoClone(CloneTag{})};
		}
		*/
		

		
		// TODO: Add concepts for validation that T has a DoClone? that returns the appropriate type "T*"
		
		// Option 1: Works. Uses this pointer.
		template <typename Target>
			requires std::derived_from<Target, std::remove_reference_t<decltype(*this)>> || std::same_as<CrtpBase, Target>
		std::unique_ptr<Target> CloneAs()
		{
			return std::unique_ptr<Target>{static_cast<Target*>(this->DoClone(CloneTag{}))};
		}

		/*
		// Option 2: Uses deduce this.
		template <typename Target, typename Self>
		std::unique_ptr<Target> CloneAs(this Self&& self)
		{
			static_assert(std::is_base_of_v<Target, std::remove_cvref_t<Self>>, "Target must be a base of the object type.");
			return std::unique_ptr<Target>{static_cast<Target*>(self.DoClone(CloneTag{}))};
		}
		*/
	};
}