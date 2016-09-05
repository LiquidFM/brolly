project (brolly)

# Project header
project_header_default ("POSITION_INDEPENDENT_CODE:YES")

# Sources
add_subdirectory (src)

# Target
add_library (brolly STATIC ${${PROJECT_NAME}_SOURCES})
add_dependencies (brolly platform)

# Documentation
add_documentation (brolly 0.0.1 "Crash handling library")

# Install rules
install_header_files (brolly "src/brolly_Assert.h:assert.h"
                             "src/brolly_CrashHandler.h:CrashHandler")
install_cmake_files ("cmake/FindBrolly.cmake")
install_target (brolly)
