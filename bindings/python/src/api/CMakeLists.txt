include_directories(${PROJECT_SOURCE_DIR}/include
  SYSTEM ${HDF5_INCLUDE_DIRS}
)

link_directories("${PROJECT_BINARY_DIR}/src")
link_directories("${Boost_LIBRARY_DIRS}")
link_directories("${PYTHON_LIBRARY_PATH}")

madnex_python_module(api_local
  api_local
  File)

target_link_libraries(py_madnex_api_local Madnex)
madnex_python_script(madnex __init__.py)
