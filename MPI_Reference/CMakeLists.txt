cmake_minimum_required(VERSION 2.6)

include_directories(${MPI_CXX_INCLUDE_PATH})

add_executable(SPTE_MPI MainDriver.cpp)

if(MPI_COMPILE_FLAGS)
	set_target_properties(SPTE_MPI PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
endif()
if(MPI_LINK_FLAGS)
	set_target_properties(SPTE_MPI PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
endif()

target_link_libraries(SPTE_MPI ${MPI_CXX_LIBRARIES} SPTE_lib)

