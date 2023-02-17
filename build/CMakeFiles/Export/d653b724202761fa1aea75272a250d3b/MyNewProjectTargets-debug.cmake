#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "MyNewProject::MyNewProject" for configuration "Debug"
set_property(TARGET MyNewProject::MyNewProject APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(MyNewProject::MyNewProject PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libMyNewProject.a"
  )

list(APPEND _cmake_import_check_targets MyNewProject::MyNewProject )
list(APPEND _cmake_import_check_files_for_MyNewProject::MyNewProject "${_IMPORT_PREFIX}/lib/libMyNewProject.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
