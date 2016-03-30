# There are common functions for configuring make script.

# get_git_branch returns name of git branch
function(get_git_branch branch)
  execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  # message(STATUS "Git branch: ${GIT_BRANCH}")
  set(${branch} GIT_BRANCH PARENT_SCOPE)
endfunction()

# There is a function for setting build type.
# If git branch is 'master', it will set TYPE variable to 'Release',
# otherwise TYPE will be set to 'Debug'.
function(get_build_type TYPE)
  get_git_branch(GIT_BRANCH)
  if(GIT_BRANCH STREQUAL "master")
    # message(STATUS "Release version")
    set(TMP_TYPE "Release")
  else()
    # message(STATUS "Developer version")
    set(TMP_TYPE "Debug")
  endif()
  set(${TYPE} "${TMP_TYPE}" PARENT_SCOPE)
endfunction()

# The main function. It adds compile flags to FLAGS variable.
function(add_compile_flags FLAGS)

  function(add_flag NEW_FLAG)
    set(TMP_FLAGS "${TMP_FLAGS} ${NEW_FLAG}" PARENT_SCOPE)
  endfunction()

  add_flag("-march=native")
  add_flag("-Ofast")
  add_flag("-std=c++14")
  add_flag("-Wall")
  add_flag("-Wextra")

  get_build_type(build_type)
  if(build_type STREQUAL "Release")
    add_flag("-Werror")
  else()
    add_flag("-fno-omit-frame-pointer")
    add_flag("-ggdb")
  endif()

  set(${FLAGS} "${CMAKE_CXX_FLAGS} ${TMP_FLAGS}" PARENT_SCOPE)
endfunction()
