macro(caspersdk_link_internal LIB)
  target_link_libraries(casper_sdk-common PRIVATE "${LIB}")
endmacro()
