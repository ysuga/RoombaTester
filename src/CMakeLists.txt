set(comp_srcs RoombaTest.cpp )
set(standalone_srcs RoombaTestComp.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${PROJECT_BINARY_DIR}/idl)
include_directories(${OPENRTM_INCLUDE_DIRS})
include_directories(${OMNIORB_INCLUDE_DIRS})
include_directories(${OPENCV_INCLUDE_DIRS})
add_definitions(${OPENRTM_CFLAGS})
add_definitions(${OMNIORB_CFLAGS})
add_definitions(${OPENCV_CFLAGS})

if(NOT WIN32)
list(APPEND OPENCV_LIBRARIES opencv_core opencv_highgui)
endif(NOT WIN32)

link_directories(${OPENRTM_LIBRARY_DIRS})
link_directories(${OPENCV_LIBRARY_DIRS})
link_directories(${OMNIORB_LIBRARY_DIRS})


add_library(${PROJECT_NAME_LOWER} ${LIB_TYPE} ${comp_srcs} ${ALL_IDL_SRCS})
set_source_files_properties(${ALL_IDL_SRCS} PROPERTIES GENERATED 1)
add_dependencies(${PROJECT_NAME_LOWER} ALL_IDL_TGT)
target_link_libraries(${PROJECT_NAME_LOWER} ${OPENCV_LIBRARIES} ${OPENRTM_LIBRARIES})


add_executable(${PROJECT_NAME_LOWER}comp ${standalone_srcs})
target_link_libraries(${PROJECT_NAME_LOWER}comp ${PROJECT_NAME_LOWER}) 

install(TARGETS ${PROJECT_NAME_LOWER} ${PROJECT_NAME_LOWER}comp
    EXPORT ${PROJECT_NAME_LOWER}
    RUNTIME DESTINATION ${BIN_INSTALL_DIR} COMPONENT component
    LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT component
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT component)
install(EXPORT ${PROJECT_NAME_LOWER}
    DESTINATION ${LIB_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    FILE ${PROJECT_NAME_LOWER}Depends.cmake)

