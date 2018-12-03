function(setup_soil executable)
  # for linux use pkg-config
  # find_package(SOIL REQUIRED)
  message(STATUS "SOIL libraries found: " ${SOIL_LIBRARIES})
  message(STATUS "SOIL include dir found: " ${SOIL_INCLUDE_DIRS})

  target_include_directories(${executable} PUBLIC
    /usr/include/SOIL
  )

  target_link_libraries(${executable} PUBLIC
    SOIL # from /usr/lib on arch
  )
endfunction(setup_soil)