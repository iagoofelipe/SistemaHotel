cmake_minimum_required(VERSION 3.19)
project(AppTest LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network)

qt_standard_project_setup()

qt_add_executable(AppTest
    main.cpp
    hotelapi.h hotelapi.cpp
)

target_link_libraries(AppTest
    PRIVATE
        Qt::Core
        Qt6::Network
)

include(GNUInstallDirs)

install(TARGETS AppTest
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET AppTest
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
