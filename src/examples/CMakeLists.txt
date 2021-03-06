FILE(GLOB_RECURSE EXAMPLES *.cpp)
FOREACH(example ${EXAMPLES})
  STRING(REGEX REPLACE ".*/([^\\/]+).cpp" "\\1" EXE_EXAMPLE ${example})

  ADD_EXECUTABLE(${EXE_EXAMPLE} ${example})
  TARGET_LINK_LIBRARIES(${EXE_EXAMPLE} giga)
  TARGET_LINK_LIBRARIES(${EXE_EXAMPLE} ${OPENSSL_LIBRARIES})
  TARGET_LINK_LIBRARIES(${EXE_EXAMPLE} ${Boost_LIBRARIES})
  TARGET_LINK_LIBRARIES(${EXE_EXAMPLE} ${CASABLANCA_LIBRARY})
  TARGET_LINK_LIBRARIES(${EXE_EXAMPLE} ${CRYPTO++_LIBRARIES})
  TARGET_LINK_LIBRARIES(${EXE_EXAMPLE} ${CURL_LIBRARY})
  TARGET_LINK_LIBRARIES(${EXE_EXAMPLE} ${CURLCPP_LIBRARIES})
  
  IF(MSVC)
    FILE(GLOB DEP_DLLS "${DEPS_PATH}/bin/*.dll")
    FOREACH(_dllfile ${DEP_DLLS})
        ADD_CUSTOM_COMMAND(
            TARGET ${EXE_EXAMPLE}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_dllfile}" $<TARGET_FILE_DIR:${EXE_EXAMPLE}>
        )
    ENDFOREACH()
  ENDIF(MSVC)
ENDFOREACH(example)
