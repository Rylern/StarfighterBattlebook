cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(StarfighterBattlebook   VERSION 1.0
                                LANGUAGES CXX)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS audio network graphics window system REQUIRED)
find_package(Threads)

add_subdirectory(src)
add_subdirectory(include)

add_subdirectory(ui)
target_link_libraries(StarfighterBattlebook PUBLIC Ui)

add_subdirectory(server)
target_link_libraries(StarfighterBattlebook PUBLIC Server)

add_subdirectory(utils)
target_link_libraries(StarfighterBattlebook PUBLIC Utils)

FILE(COPY res DESTINATION "${CMAKE_BINARY_DIR}")
install(DIRECTORY res DESTINATION StarfighterBattlebook)
install(DIRECTORY server/res DESTINATION StarfighterBattlebook)
if (WIN32)
    install(TARGETS StarfighterBattlebook
        RUNTIME_DEPENDENCIES
            PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-" "hvsifiletrust.dll" "wpaxholder.dll"
            POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
        DESTINATION StarfighterBattlebook
    )
endif()

FILE(COPY server/res DESTINATION "${CMAKE_BINARY_DIR}")
install(DIRECTORY server/res DESTINATION StarfighterBattlebookServer)
if (WIN32)
    install(TARGETS StarfighterBattlebookServer
        RUNTIME_DEPENDENCIES
            PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-" "hvsifiletrust.dll" "wpaxholder.dll"
            POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
        DESTINATION StarfighterBattlebookServer
    )
endif()