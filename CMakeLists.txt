PROJECT(AUTOSEG)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(VERSION 2.8)

IF(CMAKE_COMPILER_2005)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
ENDIF(CMAKE_COMPILER_2005)

IF (${CMAKE_SOURCE_DIR} STREQUAL ${AUTOSEG_SOURCE_DIR})
  SET (LIBRARY_OUTPUT_PATH ${AUTOSEG_BINARY_DIR}/lib CACHE PATH "Single output directory for building all libraries.")
  SET (EXECUTABLE_OUTPUT_PATH ${AUTOSEG_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")
ENDIF (${CMAKE_SOURCE_DIR} STREQUAL ${AUTOSEG_SOURCE_DIR})

OPTION(COMPILE_AUTOSEG "Compile AutoSeg." ON)
IF(COMPILE_AUTOSEG)
  SUBDIRS(AutoSeg)
ENDIF(COMPILE_AUTOSEG)

OPTION(COMPILE_IMAGEMATH "Compile ImageMath." ON)
IF(COMPILE_IMAGEMATH)
  SUBDIRS(ImageMath)
ENDIF(COMPILE_IMAGEMATH)

OPTION(COMPILE_IMAGESTAT "Compile ImageStat." ON)
IF(COMPILE_IMAGESTAT)
  SUBDIRS(ImageStat)
ENDIF(COMPILE_IMAGESTAT)

OPTION(COMPILE_CONVERTITKFORMATS "Compile convertITKformats." ON)
IF(COMPILE_CONVERTITKFORMATS)
  SUBDIRS(convertITKformats)
ENDIF(COMPILE_CONVERTITKFORMATS)

OPTION(COMPILE_CORTTHICK "Compile CortThick." ON)
IF(COMPILE_CORTTHICK)
  SUBDIRS(CortThick)
ENDIF(COMPILE_CORTTHICK)

OPTION(COMPILE_INTENSITYRESCALER "Compile IntensityRescaler." ON)
IF(COMPILE_INTENSITYRESCALER)
  SUBDIRS (Libraries/Display)
  SUBDIRS (Libraries/Algorithms)
  SUBDIRS(IntensityRescaler)
ENDIF(COMPILE_INTENSITYRESCALER)


OPTION(COMPILE_SEGPOSTPROCESS "Compile SegPostProcess." ON)
IF(COMPILE_SEGPOSTPROCESS)
  SUBDIRS(SegPostProcess)
ENDIF(COMPILE_SEGPOSTPROCESS)


#   #External Projects
#   include(ExternalProject)
#   if(CMAKE_EXTRA_GENERATOR)
#     set(gen "${CMAKE_EXTRA_GENERATOR} - ${CMAKE_GENERATOR}")
#   else()
#     set(gen "${CMAKE_GENERATOR}")
#   endif()
# 
#   OPTION(COMPILE_EXTERNAL_ABC "Compile External ABC." ON)
#   IF(COMPILE_EXTERNAL_ABC)
#     # Build ABC https://www.nitrc.org/svn/abc/tags/ABC-REL1.3/
#     set(proj ABC)
#     IF (Slicer3_FOUND)
#       ExternalProject_Add(${proj}
#         SVN_REPOSITORY "https://www.nitrc.org/svn/abc/tags/ABC-REL1.3/"
#         SVN_USERNAME slicerbot
#         SVN_PASSWORD slicer
#         SOURCE_DIR ${proj}
#         BINARY_DIR ${proj}-build
#         DEPENDS  ${ITK_DEPEND}
#         CMAKE_GENERATOR ${gen}
#         CMAKE_ARGS
#           ${LOCAL_CMAKE_BUILD_OPTIONS}
#           -DSlicer3_DIR:PATH=${Slicer3_DIR}
#           -DITK_DIR:PATH=${ITK_DIR}
#       INSTALL_COMMAND ""
#       )
#     ELSE (Slicer3_FOUND)
#       ExternalProject_Add(${proj}
#         SVN_REPOSITORY "https://www.nitrc.org/svn/abc/tags/ABC-REL1.3/"
#         SVN_USERNAME slicerbot
#         SVN_PASSWORD slicer
#         SOURCE_DIR ${proj}
#         BINARY_DIR ${proj}-build
#         DEPENDS  ${ITK_DEPEND}
#         CMAKE_GENERATOR ${gen}
#         CMAKE_ARGS
#           ${LOCAL_CMAKE_BUILD_OPTIONS}
#           -DGenerateCLP_DIR:PATH=${GenerateCLP_DIR}
#           -DITK_DIR:PATH=${ITK_DIR}
#       INSTALL_COMMAND ""
#       )
#     ENDIF (Slicer3_FOUND)
#   ENDIF(COMPILE_EXTERNAL_ABC)