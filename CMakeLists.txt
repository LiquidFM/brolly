project (brolly)

# Project header
project_cxx_library_header_default ()

# 3rdparty
list (APPEND ${PROJECT_NAME}_LIBS )

# Sources
add_subdirectory (src)

# Target
add_library (brolly STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries (brolly ${${PROJECT_NAME}_LIBS})

# Version
build_version_file (brolly 0.0.1)

# Documentation
add_documentation ("brolly" 0.0.1 "Crash handling library")
