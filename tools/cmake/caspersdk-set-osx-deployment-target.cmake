macro(caspersdk_set_osx_deployment_target TARGET_VERSION)
  if(CMAKE_OSX_DEPLOYMENT_TARGET
     VERSION_LESS
     "${TARGET_VERSION}"
  )
    if(DEFINED CMAKE_OSX_DEPLOYMENT_TARGET)
      message(
        WARNING
          "Increasing previously defined CMAKE_OSX_DEPLOYMENT_TARGET to ${TARGET_VERSION}"
      )
    endif()

    set(CMAKE_OSX_DEPLOYMENT_TARGET "${TARGET_VERSION}")
  endif()
endmacro()
