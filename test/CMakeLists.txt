find_library(PURE_PURSUIT_LIB PurePursuitController PATHS ${CMAKE_INSTALL_PREFIX}/lib)
find_library(OMNIDIRECTIONAL_LIB OmniDirectionalController PATHS ${CMAKE_INSTALL_PREFIX}/lib)
find_library(DIFFERENTIAL_LIB DifferentialDriveController PATHS ${CMAKE_INSTALL_PREFIX}/lib)

find_package(DisplayOpenGL REQUIRED)

add_executable(test_simulator test_simulator.cpp)
target_include_directories(test_simulator PRIVATE ${CMAKE_INSTALL_PREFIX}/include)
target_link_libraries(test_simulator PRIVATE DisplayOpenGL::DisplayOpenGL3DLibrary unit::SimpleUnit ${PURE_PURSUIT_LIB} ${OMNIDIRECTIONAL_LIB} ${DIFFERENTIAL_LIB} Simulator)

