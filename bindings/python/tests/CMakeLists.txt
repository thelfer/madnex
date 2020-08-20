macro(test_pymadnex test)
  add_test(NAME pymadnex${test}_py
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${test}.py)
  set_tests_properties(pymadnex${test}_py
    PROPERTIES ENVIRONMENT
    "PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python/tests;$ENV{PYTHONPATH}"
    ENVIRONMENT
    "MADNEX_REFERENCE_FILES_DIRECTORY=${PROJECT_SOURCE_DIR}/tests/references/"
    DEPENDS madnex_python_test_env)
endmacro(test_pymadnex)

add_custom_target(madnex_python_test_env)
add_custom_command(
  TARGET madnex_python_test_env
  COMMAND ${CMAKE_COMMAND} -E remove_directory madnex
  COMMAND ${CMAKE_COMMAND} -E make_directory madnex
  COMMAND ${CMAKE_COMMAND} -E make_directory madnex/api
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/bindings/python/src/__init__.py madnex/__init__.py
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/bindings/python/src/api/__init__.py madnex/api/__init__.py
  COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE:py_madnex_api_local> madnex/api/$<TARGET_FILE_NAME:py_madnex_api_local>
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/references/pivot-reference.edf test_api_macroResuWrite_00.edf
)

# test_pymadnex(test_api)
