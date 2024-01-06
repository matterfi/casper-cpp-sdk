macro(caspersdk_link_external LIB)
  target_link_libraries(casper_sdk PUBLIC "${LIB}")
  target_link_libraries(casper_sdk-common PUBLIC "${LIB}")
endmacro()
