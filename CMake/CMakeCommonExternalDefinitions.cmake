
#-----------------------------------------------------------------------------
## The FORCE_BUILD_CHECK macro adds a forecebuild step that will cause the
## external project build process to be checked for updates each time
## a dependent project is built.  It MUST be called AFTER the ExternalProject_Add
## step for the project that you want to force building on.
macro(FORCE_BUILD_CHECK  proj)
    ExternalProject_Add_Step(${proj} forcebuild
      COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BUILD_DIR}/${proj}-prefix/src/${proj}-stamp/${proj}-build
      DEPENDEES configure
      DEPENDERS build
      ALWAYS 1
    )
endmacro()
#-----------------------------------------------------------------------------
# Git protocole option
#-----------------------------------------------------------------------------
option(USE_GIT_PROTOCOL "If behind a firewall turn this off to use http instead." ON)
set(git_protocol "git")
if(NOT USE_GIT_PROTOCOL)
  set(git_protocol "http")
endif()
#-----------------------------------------------------------------------------
## empty until ITK is brought into here as an ExternalProject
macro(PACKAGE_NEEDS_ITK LOCAL_CMAKE_BUILD_OPTIONS gen)
  set(packageToCheck ITK)
  OPTION(OPT_USE_SYSTEM_${packageToCheck} "Use the system's ${packageToCheck} library." OFF)
  #  MARK_AS_ADVANCED(OPT_USE_SYSTEM_${packageToCheck})
  if(OPT_USE_SYSTEM_ITK)
    find_package(ITK REQUIRED)
    include(${ITK_USE_FILE})
    set(ITK_DEPEND "") ## Set the external depandancy for ITK
  else()
    set(proj Insight)
    set(${proj}_REPOSITORY ${git_protocol}://itk.org/ITK.git)
    set(${proj}_GIT_TAG 12f11d1f408680d24b6380856dd28dbe43582285 )
    set(ITK_VERSION_ID ITK-4.8)
    ExternalProject_Add(${proj}
      GIT_REPOSITORY ${${proj}_REPOSITORY}
      GIT_TAG ${${proj}_GIT_TAG}
      UPDATE_COMMAND ""
      SOURCE_DIR ${proj}
      BINARY_DIR ${proj}-build
      CMAKE_GENERATOR ${gen}
      CMAKE_ARG
        ${LOCAL_CMAKE_BUILD_OPTIONS}
        -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
        -DBUILD_TESTING:BOOL=OFF
        -DITK_USE_REVIEW:BOOL=ON
        -DITK_LEGACY_REMOVE:BOOL=OFF
        -DITKV3_COMPATIBILITY:BOOL=ON #Necessary for IntensityRescaler (niral_utilities)
        -DModule_ITKReview:BOOL=ON #Necessary for MultiAtlasSeg (niral_utilities)
#        -DITK_USE_REVIEW_STATISTICS:BOOL=ON
        INSTALL_COMMAND ""
        INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
    )
    FORCE_BUILD_CHECK(${proj})
    set(ITK_DIR ${CMAKE_CURRENT_BINARY_DIR}/Insight-build)
    set (ITK_DEPEND ${proj}) ## Set the internal dependancy for ITK
  endif()
endmacro()

#-----------------------------------------------------------------------------
# Get and build VTK
#
macro(PACKAGE_NEEDS_VTKWITHQT LOCAL_CMAKE_BUILD_OPTIONS gen)
  set(packageToCheck VTK)
  OPTION(OPT_USE_SYSTEM_${packageToCheck} "Use the system's ${packageToCheck} library." OFF)
  #  MARK_AS_ADVANCED(OPT_USE_SYSTEM_${packageToCheck})
  if(OPT_USE_SYSTEM_VTK)
    find_package(VTK 5.6 REQUIRED)
    include(${VTK_USE_FILE})
    set(VTK_DEPEND "") ## Set the external depandancy for VTK
  else()
    set(proj vtk-5-6)

    set(vtk_WRAP_TCL OFF)
    set(vtk_WRAP_PYTHON OFF)

    find_package(Qt4 REQUIRED)
    if(QT_USE_FILE)
      include(${QT_USE_FILE})
    endif(QT_USE_FILE)
    set(QT_ARGS
        -DDESIRED_QT_VERSION:STRING=4
        -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
      )

    set(vtk_QT_ARGS)
    set(vtk_QT_ARGS
        ${QT_ARGS}
        -DVTK_USE_GUISUPPORT:BOOL=ON
        -DVTK_USE_QVTK:BOOL=ON
        -DVTK_USE_QT:BOOL=ON
    )
    if(APPLE)
      # Qt 4.6 binary libs are built with empty OBJCXX_FLAGS for mac Cocoa
      set(vtk_QT_ARGS
        ${vtk_QT_ARGS}
        -DVTK_USE_CARBON:BOOL=OFF
        -DVTK_USE_COCOA:BOOL=ON
        -DVTK_REQUIRED_OBJCXX_FLAGS:STRING=
        )
    endif(APPLE)
    set(${proj}_GIT_TAG "v5.6.0")
    set(${proj}_REPOSITORY ${git_protocol}://vtk.org/VTK.git)
    ExternalProject_Add(${proj}
      GIT_REPOSITORY ${${proj}_REPOSITORY}
      GIT_TAG ${${proj}_GIT_TAG}
      SOURCE_DIR ${proj}
      BINARY_DIR ${proj}-build
      CMAKE_GENERATOR ${gen}
      CMAKE_ARGS
        ${LOCAL_CMAKE_BUILD_OPTIONS}
        -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
        -DVTK_USE_PARALLEL:BOOL=ON
        -DVTK_DEBUG_LEAKS:BOOL=OFF
        -DVTK_WRAP_TCL:BOOL=${vtk_WRAP_TCL}
        -DVTK_WRAP_PYTHON:BOOL=${vtk_WRAP_PYTHON}
        ${vtk_QT_ARGS}
      INSTALL_COMMAND ""
      #  INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
    )
    FORCE_BUILD_CHECK(${proj})

    set(VTK_DIR ${CMAKE_CURRENT_BINARY_DIR}/${proj}-build)
    set(VTK_DEPEND ${proj})
    MESSAGE(STATUS "Setting VTK_DIR to -DVTK_DIR:PATH=${VTK_DIR}")
    set(VTK_CMAKE
       -DVTK_DIR:PATH=${VTK_DIR}
        ${QT_ARGS}
    )
  endif()
endmacro()

macro(PACKAGE_NEEDS_VTK_NOGUI LOCAL_CMAKE_BUILD_OPTIONS gen)
  set(packageToCheck VTK)
  OPTION(OPT_USE_SYSTEM_${packageToCheck} "Use the system's ${packageToCheck} library." OFF)
  #  MARK_AS_ADVANCED(OPT_USE_SYSTEM_${packageToCheck})
  if(OPT_USE_SYSTEM_VTK)
    find_package(VTK 5.6 REQUIRED)
    include(${VTK_USE_FILE})
    set(VTK_DEPEND "") ## Set the external depandancy for ITK
  else()
    set(proj vtk-5-6)

    set(vtk_WRAP_TCL OFF)
    set(vtk_WRAP_PYTHON OFF)

    set(vtk_GUI_ARGS
        -DVTK_USE_GUISUPPORT:BOOL=OFF
        -DVTK_USE_QVTK:BOOL=OFF
        -DVTK_USE_QT:BOOL=OFF
        -DVTK_USE_X:BOOL=OFF
        -DVTK_USE_CARBON:BOOL=OFF
        -DVTK_USE_COCOA:BOOL=OFF
        -DVTK_USE_RENDERING:BOOL=OFF
    )
    if(APPLE)
      # Qt 4.6 binary libs are built with empty OBJCXX_FLAGS for mac Cocoa
      set(vtk_GUI_ARGS
        ${vtk_GUI_ARGS}
        -DVTK_REQUIRED_OBJCXX_FLAGS:STRING=
        )
    endif(APPLE)
    set(${proj}_GIT_TAG "v5.6.0")
    set(${proj}_REPOSITORY ${git_protocol}://vtk.org/VTK.git)
    ExternalProject_Add(${proj}
      GIT_REPOSITORY ${${proj}_REPOSITORY}
      GIT_TAG ${${proj}_GIT_TAG}
      SOURCE_DIR ${proj}
      BINARY_DIR ${proj}-build
      CMAKE_GENERATOR ${gen}
      CMAKE_ARGS
        ${LOCAL_CMAKE_BUILD_OPTIONS}
        -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
        -DVTK_USE_PARALLEL:BOOL=ON
        -DVTK_DEBUG_LEAKS:BOOL=OFF
        -DVTK_WRAP_TCL:BOOL=${vtk_WRAP_TCL}
        -DVTK_WRAP_PYTHON:BOOL=${vtk_WRAP_PYTHON}
        ${vtk_GUI_ARGS}
      INSTALL_COMMAND ""
      #  INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
    )
    FORCE_BUILD_CHECK(${proj})

    set(VTK_DIR ${CMAKE_CURRENT_BINARY_DIR}/${proj}-build)
    set(VTK_DEPEND ${proj})
    MESSAGE(STATUS "Setting VTK_DIR to -DVTK_DIR:PATH=${VTK_DIR}")
    set(VTK_CMAKE
       -DVTK_DIR:PATH=${VTK_DIR}
    )
  endif()
endmacro()

#-----------------------------------------------------------------------------
# Get and build SlicerExecutionModel
##  Build the SlicerExecutionModel Once, and let all derived project use the same version
macro(PACKAGE_NEEDS_SlicerExecutionModel LOCAL_CMAKE_BUILD_OPTIONS gen)
  set(packageToCheck SlicerExecutionModel)
  OPTION(OPT_USE_SYSTEM_${packageToCheck} "Use the system's ${packageToCheck} library." OFF)
  #  MARK_AS_ADVANCED(OPT_USE_SYSTEM_${packageToCheck})
  if(OPT_USE_SYSTEM_SlicerExecutionModel)
    find_package(GenerateCLP NO_MODULE REQUIRED)
    include(${GenerateCLP_USE_FILE})
    set(GenerateCLP_DEPEND "")
    set(SlicerExecutionModel_DEPEND "")
  else()
    #### ALWAYS BUILD WITH STATIC LIBS
    set(proj SlicerExecutionModel)
    set(${proj}_REPOSITORY "${git_protocol}://github.com/Slicer/SlicerExecutionModel.git")
    set(${proj}_GIT_TAG "e96f2965378cd9a9e5217cd43c8023350138b1cf")
    ExternalProject_Add(${proj}
      GIT_REPOSITORY ${${proj}_REPOSITORY}
      GIT_TAG ${${proj}_GIT_TAG}
      SOURCE_DIR ${proj}
      BINARY_DIR ${proj}-build
      DEPENDS ${ITK_DEPEND}
      CMAKE_GENERATOR ${gen}
      CMAKE_ARGS
        ${LOCAL_CMAKE_BUILD_OPTIONS}
        -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
        -DITK_DIR:PATH=${ITK_DIR}
        INSTALL_COMMAND ""
        INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
    )
    FORCE_BUILD_CHECK(${proj})
    set(GenerateCLP_DIR ${CMAKE_CURRENT_BINARY_DIR}/SlicerExecutionModel-build/GenerateCLP)
    set(SlicerExecutionModel_DIR ${CMAKE_CURRENT_BINARY_DIR}/SlicerExecutionModel-build)
    set(GenerateCLP_DEPEND "${proj}")
    set(SlicerExecutionModel_DEPEND "${proj}")
  endif()
endmacro()

#-----------------------------------------------------------------------------
# Get and build BRAINSCommonLib
macro(PACKAGE_NEEDS_BRAINSCommonLib LOCAL_CMAKE_BUILD_OPTIONS gen)
  set(packageToCheck BRAINSCommonLib)
  OPTION(OPT_USE_SYSTEM_${packageToCheck} "Use the system's ${packageToCheck} library." OFF)
  #  MARK_AS_ADVANCED(OPT_USE_SYSTEM_${packageToCheck})
  if(OPT_USE_SYSTEM_BRAINSCommonLib)
    find_package(BRAINSCommonLib NO_MODULE REQUIRED)
    include(${BRAINSCommonLib_USE_FILE})
    set(BRAINSCommonLib_DEPEND "")
  else()
    set(proj BRAINSCommonLib)
    ExternalProject_Add(${proj}
      SVN_REPOSITORY "https://www.nitrc.org/svn/brains/BRAINSCommonLib/trunk"
      SVN_USERNAME slicerbot
      SVN_PASSWORD slicer
      SOURCE_DIR ${proj}
      BINARY_DIR ${proj}-build
      DEPENDS ${ITK_DEPEND}
      CMAKE_GENERATOR ${gen}
      CMAKE_ARGS
        ${LOCAL_CMAKE_BUILD_OPTIONS}
        -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
        -DITK_DIR:PATH=${ITK_DIR}
        INSTALL_COMMAND ""
        INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
    )
    FORCE_BUILD_CHECK(${proj})
    set(BRAINSCommonLib_DIR ${CMAKE_CURRENT_BINARY_DIR}/BRAINSCommonLib-build)
    set(BRAINSCommonLib_DEPEND "${proj}")
  endif()
endmacro()


#Added by Clement

#-----------------------------------------------------------------------------
# Get and build BatchMake
##  Build the BatchMake Once, and let all derived project use the same version
macro(PACKAGE_NEEDS_BatchMake LOCAL_CMAKE_BUILD_OPTIONS gen)
  set(packageToCheck BatchMake)
  OPTION(OPT_USE_SYSTEM_${packageToCheck} "Use the system's ${packageToCheck} library." OFF)
  #  MARK_AS_ADVANCED(OPT_USE_SYSTEM_${packageToCheck})
  if(OPT_USE_SYSTEM_BatchMake)
    find_package(BatchMake NO_MODULE REQUIRED)
    include(${BatchMake_USE_FILE})
    set(BatchMake_DEPEND "")
  else()
    #### ALWAYS BUILD WITH STATIC LIBS
    set(proj BatchMake)
    set(${proj}_REPOSITORY ${git_protocol}://github.com/NIRALUser/BatchMake.git)
    set(${proj}_GIT_TAG "5e1c185b564ac2acef35ba68fbde370c19ba33de")
    ExternalProject_Add(${proj}
      GIT_REPOSITORY ${${proj}_REPOSITORY}
      GIT_TAG ${${proj}_GIT_TAG}
      UPDATE_COMMAND ""
      SOURCE_DIR ${proj}
      BINARY_DIR ${proj}-build
      DEPENDS ${ITK_DEPEND}
      CMAKE_GENERATOR ${gen}
      CMAKE_ARGS
        ${LOCAL_CMAKE_BUILD_OPTIONS}
        --no-warn-unused-cli
        -DUSE_FLTK:BOOL=OFF
        -DGRID_SUPPORT:BOOL=ON
        -DUSE_SPLASHSCREEN:BOOL=OFF
        -DITK_DIR:PATH=${ITK_DIR}
        INSTALL_COMMAND ""
        INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
    )
    FORCE_BUILD_CHECK(${proj})
    set(BatchMake_DIR ${CMAKE_CURRENT_BINARY_DIR}/BatchMake-build)
    set(BatchMake_DEPEND "${proj}")
  endif()
endmacro()

#-----------------------------------------------------------------------------
# Get and build vtkITK
##  Build the vtkITK Once, and let all derived project use the same version
macro(PACKAGE_NEEDS_vtkITK LOCAL_CMAKE_BUILD_OPTIONS gen)
  set(packageToCheck vtkITK)
  OPTION(OPT_USE_SYSTEM_${packageToCheck} "Use the system's ${packageToCheck} library." OFF)
  #  MARK_AS_ADVANCED(OPT_USE_SYSTEM_${packageToCheck})
  if(OPT_USE_SYSTEM_vtkITK)
    find_package(vtkITK NO_MODULE REQUIRED)
    include_directories(${vtkITK_SOURCE_DIR} ${vtkITK_BINARY_DIR})
    link_directories(${vtkITK_LIB_DIR})
    set(vtkITK_DEPEND "")
  else()
    #### ALWAYS BUILD WITH STATIC LIBS
    set(proj vtkITK)
    ExternalProject_Add(${proj}
      SVN_REPOSITORY "http://svn.slicer.org/Slicer3/trunk/Libs/vtkITK"
      SOURCE_DIR ${proj}
      BINARY_DIR ${proj}-build
      DEPENDS ${ITK_DEPEND} ${VTK_DEPEND}
      CMAKE_GENERATOR ${gen}
      CMAKE_ARGS
        ${LOCAL_CMAKE_BUILD_OPTIONS}
        -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
        -DITK_DIR:PATH=${ITK_DIR}
        -DVTK_DIR:PATH=${VTK_DIR}
        INSTALL_COMMAND ""
        INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
    )
    FORCE_BUILD_CHECK(${proj})
    set(vtkITK_DIR ${CMAKE_CURRENT_BINARY_DIR}/vtkITK-build)
    set(vtkITK_DEPEND "${proj}")
  endif()
endmacro()

#-----------------------------------------------------------------------------
# Get and build FLTK
##  Build the FLTK Once, and let all derived project use the same version
macro(PACKAGE_NEEDS_FLTK LOCAL_CMAKE_BUILD_OPTIONS gen)
  set(packageToCheck FLTK)
  OPTION(OPT_USE_SYSTEM_${packageToCheck} "Use the system's ${packageToCheck} library." OFF)
  #  MARK_AS_ADVANCED(OPT_USE_SYSTEM_${packageToCheck})
  set( FLTK_LOCAL_CMAKE_BUILD_OPTIONS ${LOCAL_CMAKE_BUILD_OPTIONS} )
  list( REMOVE_ITEM FLTK_LOCAL_CMAKE_BUILD_OPTIONS "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY:PATH=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )
  list( REMOVE_ITEM FLTK_LOCAL_CMAKE_BUILD_OPTIONS "-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY:PATH=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}" )
  list( REMOVE_ITEM FLTK_LOCAL_CMAKE_BUILD_OPTIONS "-DCMAKE_RUNTIME_OUTPUT_DIRECTORY:PATH=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" )
  list( REMOVE_ITEM FLTK_LOCAL_CMAKE_BUILD_OPTIONS "-DCMAKE_BUNDLE_OUTPUT_DIRECTORY:PATH=${CMAKE_BUNDLE_OUTPUT_DIRECTORY}" )
  list( REMOVE_ITEM FLTK_LOCAL_CMAKE_BUILD_OPTIONS "-DLIBRARY_OUTPUT_PATH:PATH=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )
  list( REMOVE_ITEM FLTK_LOCAL_CMAKE_BUILD_OPTIONS "-DEXECUTABLE_OUTPUT_PATH:PATH=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" )
  if(OPT_USE_SYSTEM_FLTK)
    find_package(FLTK REQUIRED)
    set(FLTK_DEPEND "")
  else()
    #### ALWAYS BUILD WITH STATIC LIBS
    set(proj FLTK)
    ExternalProject_Add(${proj}
      DOWNLOAD_NAME fltk-1.3.3.tar.gz
      DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}
      URL http://fltk.org/pub/fltk/1.3.3/fltk-1.3.3-source.tar.gz
      URL_MD5 9ccdb0d19dc104b87179bd9fd10822e3
      SOURCE_DIR ${proj}
      BINARY_DIR ${proj}-build
      CMAKE_GENERATOR ${gen}
      CMAKE_ARGS
        ${FLTK_LOCAL_CMAKE_BUILD_OPTIONS}
        -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
        -DBUILD_EXAMPLES:BOOL=OFF
        -DBUILD_TESTING:BOOL=OFF
        -DFLTK_USE_SYSTEM_JPEG:BOOL=OFF
        -DFLTK_USE_SYSTEM_PNG:BOOL=OFF
        -DFLTK_USE_SYSTEM_ZLIB:BOOL=OFF
        -DUSE_OPENGL:BOOL=ON
       INSTALL_COMMAND ""
    )
    FORCE_BUILD_CHECK(${proj})
    set(FLTK_DIR ${CMAKE_CURRENT_BINARY_DIR}/${proj}-build)
    set(FLTK_EXECUTABLE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/${proj}-build/bin)
    set(FLTK_DEPEND "${proj}")
  endif()
endmacro()
