# - Find libpq
# Find the native PostgreSQL includes and library
#
#  POSTGRESQL_INCLUDE_DIR - where to find libpq-fe.h, etc.
#  POSTGRESQL_LIBRARIES   - List of libraries when using PostgreSQL.
#  POSTGRESQL_FOUND       - True if PostgreSQL found.

IF (POSTGRESQL_INCLUDE_DIR)
  # Already in cache, be silent
  SET(POSTGRESQL_FIND_QUIETLY TRUE)
ENDIF (POSTGRESQL_INCLUDE_DIR)

FIND_PATH(POSTGRESQL_INCLUDE_DIR libpq-fe.h
  /usr/local/include/postgresql
  /usr/include/postgresql
)

SET(POSTGRESQL_NAMES pq)
FIND_LIBRARY(POSTGRESQL_LIBRARY
  NAMES ${POSTGRESQL_NAMES}
  PATHS /usr/lib /usr/local/lib /usr/lib/i386-linux-gnu /usr/lib32 
)

IF (POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARY)
  SET(POSTGRESQL_FOUND TRUE)
  SET( POSTGRESQL_LIBRARIES ${POSTGRESQL_LIBRARY} )
ELSE (POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARY)
  SET(POSTGRESQL_FOUND FALSE)
  SET( POSTGRESQL_LIBRARIES )
ENDIF (POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARY)

IF (POSTGRESQL_FOUND)
  IF (NOT POSTGRESQL_FIND_QUIETLY)
    MESSAGE(STATUS "Found PostgreSQL: ${POSTGRESQL_LIBRARY}")
  ENDIF (NOT POSTGRESQL_FIND_QUIETLY)
ELSE (POSTGRESQL_FOUND)
  IF (POSTGRESQL_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for PostgreSQL libraries named ${POSTGRESQL_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find PostgreSQL library")
  ENDIF (POSTGRESQL_FIND_REQUIRED)
ENDIF (POSTGRESQL_FOUND)

MARK_AS_ADVANCED(
  POSTGRESQL_LIBRARY
  POSTGRESQL_INCLUDE_DIR
  )