include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}/../../..
   ${CMAKE_CURRENT_SOURCE_DIR}/../lib
   ${CHM_INCLUDE_DIR}
)


########### next target ###############

set(kio_msits_PART_SRCS msits.cpp kio_mits_debug.cpp)

kcoreaddons_add_plugin(kio_msits SOURCES ${kio_msits_PART_SRCS} INSTALL_NAMESPACE "kf5/kio")
target_link_libraries(kio_msits  KF5::KIOCore Qt5::Core ${CHM_LIBRARY} Qt5::Network)
