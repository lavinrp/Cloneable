
add_library(Cloneable INTERFACE)

target_sources(
	Cloneable
	INTERFACE
	"Include/lagy/Cloneable.hpp"
)

target_include_directories(
	Cloneable
	INTERFACE
	"Include"
)