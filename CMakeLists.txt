PROJECT(AUTOSEG)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.2)
CMAKE_POLICY(VERSION 2.8.2)

IF(CMAKE_COMPILER_2005)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
ENDIF(CMAKE_COMPILER_2005)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/Install CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
set(CMAKE_HELPER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
include(ExternalProject)
include(${CMAKE_HELPER_DIR}/CMakeBuildMacros.cmake)
include(${CMAKE_HELPER_DIR}/PreventInSourceBuilds.cmake)
include(${CMAKE_HELPER_DIR}/CMakeCommonExternalDefinitions.cmake)

SETIFEMPTY( INSTALL_RUNTIME_DESTINATION bin )
#-----------------------------------------------------------------------------
# Install macro
#-----------------------------------------------------------------------------

macro(COMPILE_EXTERNAL_TOOLS)
  set(options "")
  set(oneValueArgs
    TOOL_PROJECT_NAME
    )
  set(multiValueArgs
    TOOL_NAMES
    )
  CMAKE_PARSE_ARGUMENTS(LOCAL
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
    )
  foreach( var ${LOCAL_TOOL_NAMES} )
    if( DEFINED StringNames )
      set(StringNames "${StringNames}, ${var}" )
    else()
      set(StringNames "${var}")
    endif()
  endforeach()
  list( LENGTH LOCAL_TOOL_NAMES size )
  if( ${size} GREATER 1 )
    set( IsProjectWithMultipleExecutable TRUE )
  else()
    set( IsProjectWithMultipleExecutable FALSE )
  endif()
  option(COMPILE_EXTERNAL_${LOCAL_TOOL_PROJECT_NAME} "Compile External ${LOCAL_TOOL_PROJECT_NAME} (${StringNames})" ON )
  if( COMPILE_EXTERNAL_${LOCAL_TOOL_PROJECT_NAME} )
    foreach( var ${LOCAL_TOOL_NAMES} )
      if( IsProjectWithMultipleExecutable )
        OPTION(COMPILE_EXTERNAL_${var} "Compile ${var}." ON)
      endif()
      if( COMPILE_EXTERNAL_${var} OR ( NOT IsProjectWithMultipleExecutable AND COMPILE_EXTERNAL_${LOCAL_TOOL_PROJECT_NAME} ) )
        list( APPEND LIST_TOOLS ${var} )
        set( ${var}_INSTALL_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${LOCAL_TOOL_PROJECT_NAME}-install )
      endif()
    endforeach()
  else()
    foreach( var ${LOCAL_TOOL_NAMES} )
      list( FIND LIST_TOOLS ${var} pos )
      if( "${pos}" GREATER "1" )
        list( REMOVE_ITEM LIST_TOOLS ${var} )
      endif()
    endforeach()
    if( IsProjectWithMultipleExecutable )
      foreach( var ${LOCAL_TOOL_NAMES})
        unset( COMPILE_EXTERNAL_${var} CACHE )
      endforeach()
    endif()
  endif()
  unset( StringNames )
endmacro()

#-----------------------------------------------------------------------------
# Git protocole option
#-----------------------------------------------------------------------------
option(USE_GIT_PROTOCOL "If behind a firewall turn this off to use http
instead." ON)
set(git_protocol "git")
if(NOT USE_GIT_PROTOCOL)
  set(git_protocol "http")
endif()

#-----------------------------------------------------------------------------
# Set a default build type if none was specified
#-----------------------------------------------------------------------------

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

#-----------------------------------------------------------------------------
# Common CMake options
#-----------------------------------------------------------------------------

set(LOCAL_CMAKE_BUILD_OPTIONS
  -DMAKECOMMAND:STRING=${MAKECOMMAND}
  -DCMAKE_SKIP_RPATH:BOOL=ON
  -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
  -DCMAKE_CXX_FLAGS_RELEASE:STRING=${CMAKE_CXX_FLAGS_RELEASE}
  -DCMAKE_CXX_FLAGS_DEBUG:STRING=${CMAKE_CXX_FLAGS_DEBUG}
  -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
  -DCMAKE_C_FLAGS_RELEASE:STRING=${CMAKE_C_FLAGS_RELEASE}
  -DCMAKE_C_FLAGS_DEBUG:STRING=${CMAKE_C_FLAGS_DEBUG}
  -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
  -DCMAKE_C_COMPILER:STRING=${CMAKE_C_COMPILER}
  -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
  -DBUILD_EXAMPLES:BOOL=OFF
  -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
  -DBUILD_TESTING:BOOL=${BUILD_TESTING}
  -DCMAKE_GENERATOR:STRING=${CMAKE_GENERATOR}
  -DCMAKE_EXTRA_GENERATOR:STRING=${CMAKE_EXTRA_GENERATOR}
  -DDART_TESTING_TIMEOUT:STRING=${DART_TESTING_TIMEOUT}
  -DMEMORYCHECK_COMMAND_OPTIONS:STRING=${MEMORYCHECK_COMMAND_OPTIONS}
  -DMEMORYCHECK_COMMAND:PATH=${MEMORYCHECK_COMMAND}
  -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
  -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
  -DCMAKE_MODULE_LINKER_FLAGS:STRING=${CMAKE_MODULE_LINKER_FLAGS}
  -DSITE:STRING=${SITE}
  -DBUILDNAME:STRING=${BUILDNAME}
)


#External Projects
include(ExternalProject)
if(CMAKE_EXTRA_GENERATOR)
  set(gen "${CMAKE_EXTRA_GENERATOR} - ${CMAKE_GENERATOR}")
else()
  set(gen "${CMAKE_GENERATOR}")
endif()

PACKAGE_NEEDS_ITK("${LOCAL_CMAKE_BUILD_OPTIONS}" "${gen}")
PACKAGE_NEEDS_VTK_NOGUI("${LOCAL_CMAKE_BUILD_OPTIONS}" "${gen}")
PACKAGE_NEEDS_SlicerExecutionModel("${LOCAL_CMAKE_BUILD_OPTIONS}" "${gen}")
PACKAGE_NEEDS_BatchMake("${LOCAL_CMAKE_BUILD_OPTIONS}" "${gen}")
PACKAGE_NEEDS_FLTK("${LOCAL_CMAKE_BUILD_OPTIONS}" "${gen}")
set(proj niral_utilities)
COMPILE_EXTERNAL_TOOLS( TOOL_NAMES ImageMath ImageStat convertITKformats IntensityRescaler MultiAtlasSeg TOOL_PROJECT_NAME ${proj} )
IF(COMPILE_EXTERNAL_${proj})
  set( ${proj}_REPOSITORY ${git_protocol}://github.com/NIRALUser/niral_utilities.git )
  set( ${proj}_GIT_TAG e0572efb335d3720560dfd7d9fba362a0fbd7ccf )
  ExternalProject_Add(${proj}
    GIT_REPOSITORY ${${proj}_REPOSITORY}
    GIT_TAG ${${proj}_GIT_TAG}
    SOURCE_DIR ${proj}
    BINARY_DIR ${proj}-build
    DEPENDS ${ITK_DEPEND} ${VTK_DEPEND}
    CMAKE_GENERATOR ${gen}

    CMAKE_ARGS
      ${LOCAL_CMAKE_BUILD_OPTIONS}
      -DITK_DIR:PATH=${ITK_DIR}
      -DVTK_DIR:PATH=${VTK_DIR}
      -DCOMPILE_CONVERTITKFORMATS:BOOL=${COMPILE_EXTERNAL_convertITKformats}
      -DCOMPILE_IMAGESTAT:BOOL=${COMPILE_EXTERNAL_ImageStat}
      -DCOMPILE_IMAGEMATH:BOOL=${COMPILE_EXTERNAL_ImageMath}
      -DCOMPILE_IntensityRescaler:BOOL=${COMPILE_EXTERNAL_IntensityRescaler}
      -DCOMPILE_MULTIATLASSEG:BOOL=${COMPILE_EXTERNAL_MultiAtlasSeg}
      -DCOMPILE_Unsupported:BOOL=ON
      -DCOMPILE_TRANSFORMDEFORMATIONFIELD:BOOL=OFF
      -DCOMPILE_CROPTOOLS:BOOL=OFF
      -DCOMPILE_DWI_NIFTINRRDCONVERSION:BOOL=OFF
      -DCOMPILE_POLYDATATRANSFORM:BOOL=OFF
      -DCOMPILE_POLYDATAMERGE:BOOL=OFF
      -DCOMPILE_CURVECOMPARE:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/${proj}-install
  )
  FORCE_BUILD_CHECK(${proj})
ENDIF(COMPILE_EXTERNAL_${proj})

set(proj CortThick)
COMPILE_EXTERNAL_TOOLS( TOOL_NAMES CortThickCLP TOOL_PROJECT_NAME ${proj} )
IF(COMPILE_EXTERNAL_${proj})
  set( ${proj}_REPOSITORY ${git_protocol}://github.com/NIRALUser/ARCTIC.git )
  set( ${proj}_GIT_TAG 0cc38d92db43a3861e6e7491d516d5edb543bc4b )
  ExternalProject_Add(${proj}
    GIT_REPOSITORY ${${proj}_REPOSITORY}
    GIT_TAG ${${proj}_GIT_TAG}
    SOURCE_DIR ${proj}
    BINARY_DIR ${proj}-build
    DEPENDS ${ITK_DEPEND} ${SlicerExecutionModel_DEPEND} ${VTK_DEPEND} ${BatchMake_DEPEND}
    CMAKE_GENERATOR ${gen}
    CMAKE_ARGS
      ${LOCAL_CMAKE_BUILD_OPTIONS}
      -DITK_DIR:PATH=${ITK_DIR}
      -DSlicerExecutionModel_DIR:PATH=${SlicerExecutionModel_DIR}
      -DVTK_DIR:PATH=${VTK_DIR}
      -DBatchMake_DIR:PATH=${BatchMake_DIR}
      -DCOMPILE_ImageMath:BOOL=OFF
      -DCOMPILE_ImageStat:BOOL=OFF
      -DCOMPILE_ITKEMS:BOOL=OFF
      -DCOMPILE_SegPostProcess:BOOL=OFF
      -DCOMPILE_ARCTIC:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/${proj}-install
  )
  FORCE_BUILD_CHECK(${proj})
ENDIF(COMPILE_EXTERNAL_${proj})

set(proj SegPostProcessCLP)
COMPILE_EXTERNAL_TOOLS( TOOL_NAMES SegPostProcessCLP TOOL_PROJECT_NAME ${proj} )
IF(COMPILE_EXTERNAL_${proj})
  set( ${proj}_REPOSITORY ${git_protocol}://github.com/NIRALUser/SPHARM-PDM.git )
  set( ${proj}_GIT_TAG b39be76d6bb5862b2db5555a02e267acd36c945a )
  ExternalProject_Add(${proj}
    GIT_REPOSITORY ${${proj}_REPOSITORY}
    GIT_TAG ${${proj}_GIT_TAG}
    SOURCE_DIR ${proj}
    BINARY_DIR ${proj}-build
    DEPENDS ${ITK_DEPEND} ${SlicerExecutionModel_DEPEND} ${VTK_DEPEND} ${BatchMake_DEPEND}
    CMAKE_GENERATOR ${gen}
    CMAKE_ARGS
      ${LOCAL_CMAKE_BUILD_OPTIONS}
      -DSlicerExecutionModel_DIR:PATH=${SlicerExecutionModel_DIR}
      -DITK_DIR:PATH=${ITK_DIR}
      -DVTK_DIR:PATH=${VTK_DIR}
      -DBatchMake_DIR:PATH=${BatchMake_DIR}
      -DCOMPILE_ImageMath:BOOL=OFF
      -DCOMPILE_MetaMeshTools:BOOL=OFF
      -DCOMPILE_SegPostProcessCLP:BOOL=ON
      -DCOMPILE_GenParaMeshCLP:BOOL=OFF
      -DCOMPILE_RadiusToMesh:BOOL=OFF
      -DCOMPILE_ParaToSPHARMMeshCLP:BOOL=OFF
      -DCOMPILE_ShapeAnalysisModule:BOOL=OFF
      -DCOMPILE_SpharmTool:BOOL=OFF
      -DCOMPILE_shapeAnalysisMANCOVA_Wizard:BOOL=OFF
      -DCOMPILE_StatNonParamTestPDM:BOOL=OFF
      -DCOMPILE_shapeworks:BOOL=OFF
      -DCOMPILE_ParticleModule:BOOL=OFF
      -DBUILD_LIBRARIES:BOOL=OFF
      -DSPHARM-PDM_SUPERBUILD:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/${proj}-install
  )
  FORCE_BUILD_CHECK(${proj})
ENDIF(COMPILE_EXTERNAL_${proj})

set(proj ABC)
COMPILE_EXTERNAL_TOOLS( TOOL_NAMES ABC TOOL_PROJECT_NAME ${proj} )
IF(COMPILE_EXTERNAL_${proj})
  set( ${proj}_REPOSITORY ${git_protocol}://github.com/NIRALUser/ABC.git )
  set( ${proj}_GIT_TAG 32ed5f2d3d4c706efb81ff0745f9782a85fd8758 )
  ExternalProject_Add(${proj}
    GIT_REPOSITORY ${${proj}_REPOSITORY}
    GIT_TAG ${${proj}_GIT_TAG}
    SOURCE_DIR ${proj}
    BINARY_DIR ${proj}-build
    DEPENDS ${ITK_DEPEND}
    CMAKE_GENERATOR ${gen}
    CMAKE_ARGS
      ${LOCAL_CMAKE_BUILD_OPTIONS}
      -DGenerateCLP_DIR:PATH=${GenerateCLP_DIR}
      -DITK_DIR:PATH=${ITK_DIR}
      -DCOMPILE_SLICER4COMMANDLINE:BOOL=OFF
      -DCOMPILE_BRAINSEG:BOOL=OFF
      -DCOMPILE_STANDALONEGUI:BOOL=OFF
      -DCOMPILE_COMMANDLINE:BOOL=ON
      -DABC_CLI_NO_SUFFIX:BOOL=ON
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/${proj}-install
   )
  FORCE_BUILD_CHECK(${proj})
ENDIF(COMPILE_EXTERNAL_${proj})

set(proj neoseg)
COMPILE_EXTERNAL_TOOLS( TOOL_NAMES neoseg TOOL_PROJECT_NAME ${proj} )
IF(COMPILE_EXTERNAL_${proj})
  set( ${proj}_REPOSITORY ${git_protocol}://github.com/NIRALUser/neoseg.git )
  set( ${proj}_GIT_TAG 80a1714ee52e9a2b69e55619c371c22a36817f0e )
  ExternalProject_Add(${proj}
    GIT_REPOSITORY ${${proj}_REPOSITORY}
    GIT_TAG ${${proj}_GIT_TAG}
    SOURCE_DIR ${proj}
    BINARY_DIR ${proj}-build
    DEPENDS ${ITK_DEPEND}
    CMAKE_GENERATOR ${gen}
    CONFIGURE_COMMAND ${CMAKE_COMMAND}
      ${LOCAL_CMAKE_BUILD_OPTIONS}
      -DITK_DIR:PATH=${ITK_DIR}
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/${proj}-install
      ${CMAKE_CURRENT_BINARY_DIR}/${proj}/neoseg
   )
  FORCE_BUILD_CHECK(${proj})
ENDIF(COMPILE_EXTERNAL_${proj})

set(proj SlicerCLI)
COMPILE_EXTERNAL_TOOLS( TOOL_NAMES N4ITKBiasFieldCorrection ResampleScalarVectorDWIVolume TOOL_PROJECT_NAME ${proj} )
IF(COMPILE_EXTERNAL_${proj})
  find_package(Subversion REQUIRED)
  set( ${proj}_REPOSITORY ${git_protocol}://github.com/NIRALUser/SlicerCLI.git )
  set( ${proj}_GIT_TAG 8acfde9f975d7e199ee6f500e4c5b9afbf7ebeb6 )
  if( COMPILE_EXTERNAL_N4ITKBiasFieldCorrection )
    set(SlicerCLIModules N4ITKBiasFieldCorrection )
  endif()
  if( COMPILE_EXTERNAL_ResampleScalarVectorDWIVolume )
    list( APPEND SlicerCLIModules
          ResampleDTIVolume
          ResampleScalarVectorDWIVolume
        )
  endif()
  foreach( var ${SlicerCLIModules})
    if( DEFINED StringModules )#Even if we don't want to build on module, we still download it's code, because another module might depend on it
      set(StringModules "${StringModules} ${var}" )
    else()
      set(StringModules "${var}")
    endif()
  endforeach()
  ExternalProject_Add(${proj}
    GIT_REPOSITORY ${${proj}_REPOSITORY}
    GIT_TAG ${${proj}_GIT_TAG}
    SOURCE_DIR ${proj}
    BINARY_DIR ${proj}-build
    DEPENDS ${ITK_DEPEND} ${SlicerExecutionModel_DEPEND} ${VTK_DEPEND}
    CMAKE_GENERATOR ${gen}
      ${LOCAL_CMAKE_BUILD_OPTIONS}
      -DSlicerExecutionModel_DIR:PATH=${SlicerExecutionModel_DIR}
      -DITK_DIR:PATH=${ITK_DIR}
      -DVTK_DIR:PATH=${VTK_DIR}
      -DBUILD_CLI_ResampleScalarVectorDWIVolume:BOOL=${COMPILE_EXTERNAL_N4ITKBiasFieldCorrection}
      -DBUILD_CLI_ResampleDTIVolume:BOOL=${COMPILE_EXTERNAL_ResampleScalarVectorDWIVolume}
      -DBUILD_CLI_N4ITKBiasFieldCorrection:BOOL=${COMPILE_EXTERNAL_ResampleScalarVectorDWIVolume}
      -Dcli-modules_SUPERBUILD:BOOL=OFF
      -DSlicer_Revision:STRING=22599#Builds Slicer extension at SVN revision 22599 (=Slicer 4.3.1)
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/${proj}-install
    PATCH_COMMAND ${CMAKE_COMMAND} -DSlicer_Revision:STRING=22599 -DStringModules:STRING="${StringModules}" -DSubversion_SVN_EXECUTABLE:FILEPATH=${Subversion_SVN_EXECUTABLE} -DDOWNLOAD_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/${proj} -P ${CMAKE_CURRENT_BINARY_DIR}/${proj}/SuperBuild/SlicerExecutionModelDownload.cmake
   )
ENDIF(COMPILE_EXTERNAL_${proj})

set(proj ANTS)
COMPILE_EXTERNAL_TOOLS( TOOL_NAMES ANTS WarpImageMultiTransform TOOL_PROJECT_NAME ${proj} )
IF(COMPILE_EXTERNAL_ANTS)
  set( ${proj}_REPOSITORY ${git_protocol}://github.com/stnava/ANTs.git )
  set( ${proj}_GIT_TAG 40285bf1c8e0d8f4395b7220225e67ed599a31ac )
  set( ANTS_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${proj}-lib )
  set( ANTS_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${proj}-lib )
  ExternalProject_Add(${proj}
    GIT_REPOSITORY ${${proj}_REPOSITORY}
    GIT_TAG ${${proj}_GIT_TAG}
    SOURCE_DIR ${proj}
    BINARY_DIR ${proj}-build
    DEPENDS ${ITK_DEPEND} ${SlicerExecutionModel_DEPEND} ${VTK_DEPEND}
    CMAKE_GENERATOR ${gen}
      ${LOCAL_CMAKE_BUILD_OPTIONS}
      -DSlicerExecutionModel_DIR:PATH=${SlicerExecutionModel_DIR}
      -DITK_DIR:PATH=${ITK_DIR}
      -DVTK_DIR:PATH=${VTK_DIR}
      -DBUILD_EXAMPLES:BOOL=OFF
      -DBUILD_TESTING:BOOL=OFF
      -DANTS_SUPERBUILD:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/${proj}-install
   )
  set(ANTs_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${proj})
  # libantsUtilities does not get installed. Instead, we point to the build directory so that BRAINSTTools can find it. This would most likely not work on Windows as the library would likely be in a sub-folder named after the build type (eg Debug) but AutoSeg does not work on Windows, so this is not an issue.
  set(ANTs_LIBRARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/${proj}-build/lib )
  set(ANTs_DEPEND "${proj}")
  FORCE_BUILD_CHECK(${proj})
ENDIF(COMPILE_EXTERNAL_ANTS)

set(proj BRAINSTools)
COMPILE_EXTERNAL_TOOLS( TOOL_NAMES BRAINSDemonWarp BRAINSResample BRAINSFit TOOL_PROJECT_NAME ${proj} )
IF(COMPILE_EXTERNAL_${proj})
  set( ${proj}_REPOSITORY ${git_protocol}://github.com/NIRALUser/BRAINSTools.git)
  set( ${proj}_GIT_TAG 0be415485411a05a65c36678aa2571b6b1ae1585 )
  ExternalProject_Add(${proj}
    GIT_REPOSITORY ${${proj}_REPOSITORY}
    GIT_TAG ${${proj}_GIT_TAG}
    SOURCE_DIR ${proj}
    BINARY_DIR ${proj}-build
    DEPENDS ${ITK_DEPEND} ${SlicerExecutionModel_DEPEND} ${VTK_DEPEND} ${ANTs_DEPEND}
    CMAKE_GENERATOR ${gen}
      ${LOCAL_CMAKE_BUILD_OPTIONS}
      -DBUILD_SHARED_LIBS:BOOL=OFF
      -DINTEGRATE_WITH_SLICER:BOOL=OFF
      -DBRAINSTools_SUPERBUILD:BOOL=OFF
      -DSuperBuild_BRAINSTools_USE_GIT:BOOL=${USE_GIT_PROTOCOL}
      -DITK_VERSION_MAJOR:STRING=4
      -DITK_DIR:PATH=${ITK_DIR}
      -DVTK_DIR:PATH=${VTK_DIR}
      -DUSE_SYSTEM_ITK=ON
      -DUSE_SYSTEM_SlicerExecutionModel=ON
      -DUSE_SYSTEM_VTK=ON
      -DGenerateCLP_DIR:PATH=${GenerateCLP_DIR}
      -DModuleDescriptionParser_DIR:PATH=${ModuleDescriptionParser_DIR}
      -DSlicerExecutionModel_DIR:PATH=${SlicerExecutionModel_DIR}
      -DBUILD_TESTING:BOOL=OFF
      -DUSE_BRAINSDemonWarp:BOOL=${COMPILE_EXTERNAL_BRAINSDemonWarp}
      -DUSE_BRAINSFit:BOOL=${COMPILE_EXTERNAL_BRAINSFit}
      -DUSE_BRAINSResample:BOOL=${COMPILE_EXTERNAL_BRAINSResample}
      -DUSE_AutoWorkup:BOOL=OFF
      -DUSE_ANTS:BOOL=${COMPILE_EXTERNAL_ANTS}
      -DANTs_SOURCE_DIR=${ANTs_SOURCE_DIR}
      -DANTs_LIBRARY_DIR=${ANTs_LIBRARY_DIR}
      -DUSE_BRAINSDWICleanup:BOOL=OFF
      -DUSE_BRAINSContinuousClass:BOOL=OFF
      -DUSE_BRAINSFitEZ:BOOL=OFF
      -DUSE_BRAINSROIAuto:BOOL=OFF
      -DUSE_BRAINSSurfaceTools:BOOL=OFF
      -DUSE_BRAINSLabelStats:BOOL=OFF
      -DUSE_DebugImageViewer:BOOL=OFF
      -DUSE_BRAINSMultiSTAPLE:BOOL=OFF
      -DUSE_BRAINSStripRotation:BOOL=OFF
      -DUSE_BRAINSTalairach:BOOL=OFF
      -DUSE_BRAINSABC:BOOL=OFF
      -DUSE_BRAINSConstellationDetector:BOOL=OFF
      -DUSE_BRAINSCreateLabelMapFromProbabilityMaps:BOOL=OFF
      -DUSE_BRAINSCut:BOOL=OFF
      -DUSE_BRAINSImageConvert:BOOL=OFF
      -DUSE_BRAINSInitializedControlPoints:BOOL=OFF
      -DUSE_BRAINSLandmarkInitializer:BOOL=OFF
      -DUSE_BRAINSMultiModeSegment:BOOL=OFF
      -DUSE_BRAINSMush:BOOL=OFF
      -DUSE_BRAINSSnapShotWriter:BOOL=OFF
      -DUSE_BRAINSTransformConvert:BOOL=OFF
      -DUSE_ConvertBetweenFileFormats:BOOL=OFF
      -DUSE_DWIConvert:BOOL=OFF
      -DUSE_DebugImageViewer:BOOL=OFF
      -DUSE_ICCDEF:BOOL=OFF
      -DUSE_ImageCalculator:BOOL=OFF
      -DUSE_GTRACT:BOOL=OFF
      -DLOCAL_SEM_EXECUTABLE_ONLY:BOOL=ON # Variable used in SlicerExecutionModel/CMake/SEMMacroBuildCLI.cmake:l.120 : if true, will only create executable without shared lib lib(exec)Lib.so
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/${proj}-install
   )
  FORCE_BUILD_CHECK(${proj})
ENDIF(COMPILE_EXTERNAL_${proj})

set(proj AutoSeg)
list(APPEND LIST_TOOLS AutoSeg )
set( AutoSeg_INSTALL_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/AutoSeg-install )
ExternalProject_Add(${proj}
  DOWNLOAD_COMMAND ""
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${proj}
  BINARY_DIR ${proj}-build
  DEPENDS ${ITK_DEPEND} ${BatchMake_DEPEND} ${FLTK_DEPEND}
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    ${LOCAL_CMAKE_BUILD_OPTIONS}
    -DITK_DIR:PATH=${ITK_DIR}
    -DFLTK_DIR:PATH=${FLTK_DIR}
    -DFLTK_EXECUTABLE_DIRS:PATH=${FLTK_EXECUTABLE_DIRS}
    -DBatchMake_DIR:PATH=${BatchMake_DIR}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/${proj}-install
)
FORCE_BUILD_CHECK(${proj})

if(WIN32)
  set(fileextension .exe)
endif()
foreach( VAR ${LIST_TOOLS} )
  install(PROGRAMS ${${VAR}_INSTALL_DIRECTORY}/${INSTALL_RUNTIME_DESTINATION}/${VAR}${fileextension}
  DESTINATION ${INSTALL_RUNTIME_DESTINATION}
  )
endforeach()
