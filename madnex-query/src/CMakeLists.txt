add_executable(madnex-query madnex-query.cxx)
target_link_libraries(madnex-query Madnex)
install(TARGETS madnex-query DESTINATION bin COMPONENT core)
