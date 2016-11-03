#------------------------------------------------------------------------------
# test suite build
#------------------------------------------------------------------------------
set (MORTON_TEST_SOURCES
  cu_suite.c
  cu_morton.c
  cu_grid.c
)

set (MORTON_TEST_HEADERS
  cu_suite.h
)

include_directories ("${PGSQL_INCLUDEDIR_SERVER}")
include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${CUNIT_INCLUDE_DIR}")

add_executable(cu_tester ${MORTON_TEST_SOURCES} ${MORTON_TEST_HEADERS})
target_link_libraries (cu_tester morton-static ${CUNIT_LIBRARIES})

add_test(cu_tester cu_tester)
