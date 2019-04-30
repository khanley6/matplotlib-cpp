file(GLOB files "*.cpp")
foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)

    add_executable(${file_basename} ${file})
    target_include_directories(${file_basename} PRIVATE
        ${PYTHON_INCLUDE_DIRS}
        ${xtensor_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}
    )
    target_link_libraries(${file_basename}
        ${PYTHON_LIBRARIES}
    )
endforeach()

