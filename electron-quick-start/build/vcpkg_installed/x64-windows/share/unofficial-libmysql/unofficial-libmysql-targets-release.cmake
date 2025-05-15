#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "unofficial::libmysql::libmysql" for configuration "Release"
set_property(TARGET unofficial::libmysql::libmysql APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(unofficial::libmysql::libmysql PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libmysql.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "zstd::libzstd_shared"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libmysql.dll"
  )

list(APPEND _cmake_import_check_targets unofficial::libmysql::libmysql )
list(APPEND _cmake_import_check_files_for_unofficial::libmysql::libmysql "${_IMPORT_PREFIX}/lib/libmysql.lib" "${_IMPORT_PREFIX}/bin/libmysql.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
