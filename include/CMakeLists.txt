target_include_directories(StarfighterBattlebook PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(StarfighterBattlebook PUBLIC ${PROJECT_BINARY_DIR}/include)

configure_file (
    "${PROJECT_SOURCE_DIR}/include/version.hpp.in"
    "${PROJECT_BINARY_DIR}/include/version.hpp"
)