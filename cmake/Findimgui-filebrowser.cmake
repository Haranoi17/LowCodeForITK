find_path(filebrowser_INCLUDE_DIRS
  NAMES imfilebrowser.h
  PATHS ${CMAKE_SOURCE_DIR}/externals/imgui-filebrowser
)

if(filebrowser_INCLUDE_DIRS)
  set(filebrowser_FOUND TRUE)
endif()

if(filebrowser_FOUND)
  set(filebrowser_INCLUDE_DIRS "${filebrowser_INCLUDE_DIRS}" CACHE PATH "Path to filebrowser headers")

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(MyLibrary
    REQUIRED_VARS filebrowser_INCLUDE_DIRS 
  )
endif()