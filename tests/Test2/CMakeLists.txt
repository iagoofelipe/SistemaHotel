cmake_minimum_required(VERSION 3.19)
project(Test2 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network)

qt_standard_project_setup()

qt_add_executable(Test2
    main.cpp
    hotelapi.h hotelapi.cpp
)

target_link_libraries(Test2
    PRIVATE
        Qt::Core
        Qt6::Network
)

include(GNUInstallDirs)

install(TARGETS Test2
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Test2
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
