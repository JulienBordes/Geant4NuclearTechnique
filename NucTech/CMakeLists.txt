#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.27)
project(nuctech)

#----------------------------------------------------------------------------
# Enable high-level warnings and treat warnings as errors (optional)
if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor 
                      -Wold-style-cast -Wcast-align -Wunused 
                      -Woverloaded-virtual -Wconversion -Wsign-conversion)
  # Optional: Uncomment the next line to treat all warnings as errors
  # add_compile_options(-Werror)
endif()

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
add_executable(nuctech nuctech.cc ${sources} ${headers})
target_link_libraries(nuctech ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy scripts and configure files
# List of macros related to simulation (stored in macros/simulation/)
set(nuctech_FILES
  macros/beam.mac
  macros/ion_beam.mac
  macros/radioactive_source.mac
  macros/quiet.mac
  macros/run.mac
  macros/vis.mac
  process_sim.C
)

foreach(_file ${nuctech_FILES})
    # Get the file name (without any directory components)
    get_filename_component(file_name ${_file} NAME)
    configure_file(
      ${PROJECT_SOURCE_DIR}/${_file}
      ${PROJECT_BINARY_DIR}/${file_name}
      COPYONLY
    )
endforeach()

# Set the build directory path
set(BUILD_DIR "${PROJECT_BINARY_DIR}")

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
install(TARGETS nuctech DESTINATION bin)