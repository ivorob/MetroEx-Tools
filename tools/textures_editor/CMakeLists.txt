CMAKE_MINIMUM_REQUIRED (VERSION 3.4)
PROJECT (TexturesEditor)

SET (SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)
SET (MAIN_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
SET (SOURCES 
        ${SRC_DIR}/main.cpp
        ${MAIN_SRC_DIR}/log.cpp
        ${MAIN_SRC_DIR}/hashing.cpp
        ${MAIN_SRC_DIR}/metro/VFXReader.cpp
        ${MAIN_SRC_DIR}/metro/MetroCompression.cpp
        ${MAIN_SRC_DIR}/metro/MetroTexturesDatabase.cpp
        ${MAIN_SRC_DIR}/metro/MetroBinArchive.cpp
        ${MAIN_SRC_DIR}/metro/MetroFileSystem.cpp
        ${MAIN_SRC_DIR}/metro/reflection/MetroReflection.cpp
    )

SET (HEADERS
        ${MAIN_SRC_DIR}/log.h
        ${MAIN_SRC_DIR}/metro/VFXReader.h
        ${MAIN_SRC_DIR}/metro/MetroCompression.h
        ${MAIN_SRC_DIR}/metro/MetroTexturesDatabase.h
        ${MAIN_SRC_DIR}/metro/MetroBinArchive.h
        ${MAIN_SRC_DIR}/metro/MetroFileSystem.h
        ${MAIN_SRC_DIR}/metro/reflection/MetroReflection.h
    )

SET (CMAKE_CXX_STANDARD 17)

ADD_EXECUTABLE (TexturesEditor ${SOURCES} ${HEADERS})
TARGET_INCLUDE_DIRECTORIES (TexturesEditor PRIVATE ${CMAKE_SOURCE_DIR}/src)

TARGET_LINK_LIBRARIES (TexturesEditor glm)
TARGET_LINK_LIBRARIES (TexturesEditor xxhash)
TARGET_LINK_LIBRARIES (TexturesEditor lz4)
