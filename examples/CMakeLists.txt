file(GLOB files "*.cpp")
if (NOT USE_NUMPY)
    message(STATUS "Can't build surface.cpp without numpy. Skipping...")
    get_filename_component(surface_path ${CMAKE_CURRENT_SOURCE_DIR}/surface.cpp ABSOLUTE)
    list(REMOVE_ITEM files "${surface_path}")
endif()

foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)

    add_executable(${file_basename} ${file})
    target_include_directories(${file_basename} PRIVATE
        ${PYTHON_INCLUDE_DIRS}
        ${xtensor_INCLUDE_DIRS}
        ${MPLCPP_INCLUDE_DIR}
    )
    if(NUMPY_FOUND)
        target_include_directories(${file_basename} PRIVATE
            ${NUMPY_INCLUDE_DIRS}
        )
    endif()

    target_link_libraries(${file_basename}
        ${PYTHON_LIBRARIES}
    )
endforeach()

