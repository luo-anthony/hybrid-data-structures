#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "HybridStructures::HybridStructures" for configuration "Debug"
set_property(TARGET HybridStructures::HybridStructures APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(HybridStructures::HybridStructures PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libHybridStructures.a"
  )

list(APPEND _cmake_import_check_targets HybridStructures::HybridStructures )
list(APPEND _cmake_import_check_files_for_HybridStructures::HybridStructures "${_IMPORT_PREFIX}/lib/libHybridStructures.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
