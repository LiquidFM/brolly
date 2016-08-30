project (brolly)

# Project header
project_static_library_header_default ()

# 3rdparty
list (APPEND ${PROJECT_NAME}_LIBS platform)

# Sources
add_subdirectory (src)

# Target
add_library (brolly STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries (brolly ${${PROJECT_NAME}_LIBS})

# Documentation
add_documentation ("brolly" 0.0.1 "Crash handling library")

# Install rules
install_header_files (brolly "src/brolly_Assert.h:assert.h"
                             "src/brolly_CrashHandler.h:CrashHandler")
install_target (brolly)
