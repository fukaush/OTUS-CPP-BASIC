cmake_minimum_required(VERSION 3.8)

project(guess-my-number)

add_executable(
	guess-my-number
	"${PROJECT_SOURCE_DIR}/src/guess-my-number.cpp"
	"${PROJECT_SOURCE_DIR}/src/parameters.cpp"
	"${PROJECT_SOURCE_DIR}/src/game/exception.cpp"
	"${PROJECT_SOURCE_DIR}/src/game/levels.cpp"
	"${PROJECT_SOURCE_DIR}/src/game/scores.cpp"
	"${PROJECT_SOURCE_DIR}/src/game/help.cpp"
)

set_target_properties(guess-my-number PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)