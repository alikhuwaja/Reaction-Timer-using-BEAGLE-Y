

include_directories(include)


set(APP_SRC "")
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/reaction_timer.c")
  list(APPEND APP_SRC src/reaction_timer.c)
endif()
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/reaction_timer/reaction_timer.c")
  list(APPEND APP_SRC src/reaction_timer/reaction_timer.c)
endif()


message(STATUS "App sources: ${APP_SRC}")


if (APP_SRC STREQUAL "")
  message(FATAL_ERROR "reaction_timer.c not found in app/src or app/src/reaction_timer/")
endif()

add_executable(reaction_timer ${APP_SRC})
target_link_libraries(reaction_timer PRIVATE hal)


add_custom_command(TARGET reaction_timer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "/home/ali/ensc351/public/myApps"
  COMMAND ${CMAKE_COMMAND} -E copy
          "$<TARGET_FILE:reaction_timer>"
          "/home/ali/ensc351/public/myApps/reaction_timer"
  COMMENT "Copying executable to /home/ali/ensc351/public/myApps/"
)


