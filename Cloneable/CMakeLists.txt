
add_executable(Cloneable)

target_sources(
	Cloneable
	PUBLIC
	"Include/lagy/Cloneable.hpp"
	"Include/lagy/Cloneable.cpp"
)

target_include_directories(
	Cloneable
	PUBLIC
	"Include"
)