#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "unofficial::libmysql::libmysql" for configuration "Debug"
set_property(TARGET unofficial::libmysql::libmysql APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(unofficial::libmysql::libmysql PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/debug/lib/libmysql.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "zstd::libzstd_shared"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/bin/libmysql.dll"
  )

list(APPEND _cmake_import_check_targets unofficial::libmysql::libmysql )
list(APPEND _cmake_import_check_files_for_unofficial::libmysql::libmysql "${_IMPORT_PREFIX}/debug/lib/libmysql.lib" "${_IMPORT_PREFIX}/debug/bin/libmysql.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
