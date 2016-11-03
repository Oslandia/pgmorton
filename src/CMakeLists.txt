#------------------------------------------------------------------------------
# c
#------------------------------------------------------------------------------
set (MORTON_SOURCES
  inout.c
  morton.c
  grid.c
)

set (MORTON_HEADERS
  pgsql.h
  morton.h
  grid.h
)

set(CMAKE_C_FLAGS "${PGSQL_CPPFLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${PGSQL_LDFLAGS}")

include_directories ("${PGSQL_INCLUDEDIR_SERVER}")

add_library (morton SHARED ${MORTON_SOURCES} ${MORTON_HEADERS})
add_library (morton-static STATIC ${MORTON_SOURCES} ${MORTON_HEADERS})

set_target_properties (morton PROPERTIES
  OUTPUT_NAME "morton-${PGMORTON_VERSION_MAJOR}"
  PREFIX ""
)

target_link_libraries(morton m)
target_link_libraries(morton-static m)

#------------------------------------------------------------------------------
# sql
#------------------------------------------------------------------------------
set ( MORTON_INSTALL_EXENSIONS
  "${PROJECT_BINARY_DIR}/pgsql/morton--${PGMORTON_VERSION}.sql"
  "${PROJECT_BINARY_DIR}/pgsql/morton.control"
)

configure_file(
  morton.sql.in
  "${PROJECT_BINARY_DIR}/pgsql/morton--${PGMORTON_VERSION}.sql"
)

configure_file(
  morton.control.in
  "${PROJECT_BINARY_DIR}/pgsql/morton.control"
)

#------------------------------------------------------------------------------
# install
#------------------------------------------------------------------------------
install (
  TARGETS morton
  DESTINATION ${PGSQL_PKGLIBDIR}
)

install (
  FILES ${MORTON_INSTALL_EXENSIONS}
  DESTINATION "${PGSQL_SHAREDIR}/extension"
)
