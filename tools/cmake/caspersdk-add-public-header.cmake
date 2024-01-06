function(
  caspersdk_add_public_header
  install_location
  header_name
)
  if(EXISTS
     "${${PROJECT_NAME}_SOURCE_DIR}/include/Casper/${install_location}/${header_name}"
  )
    set(HEADER_FILE
        "${${PROJECT_NAME}_SOURCE_DIR}/include/Casper/${install_location}/${header_name}"
    )
  elseif(
    EXISTS
    "${${PROJECT_NAME}_BINARY_DIR}/include/Casper/${install_location}/${header_name}"
  )
    set(HEADER_FILE
        "${${PROJECT_NAME}_BINARY_DIR}/include/Casper/${install_location}/${header_name}"
    )
  else()
    message(
      FATAL_ERROR "Unable to find header ${install_location}/${header_name}"
    )
  endif()

  target_sources(casper_sdk-common PRIVATE "${HEADER_FILE}")

  if(CASPERSDK_INSTALL_HEADERS)
    target_sources(
      casper_sdk
      PUBLIC
        FILE_SET
        HEADERS
        FILES
        "${HEADER_FILE}"
    )
  endif()
endfunction()
