PROJECT(CortThick)

FIND_PACKAGE(ITK REQUIRED)
IF(ITK_FOUND)
  INCLUDE(${USE_ITK_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR "ITK not found. Please set ITK_DIR.")
ENDIF(ITK_FOUND)

FIND_PACKAGE(VTK REQUIRED)
IF (VTK_FOUND)
  INCLUDE (${VTK_USE_FILE})
ELSE(VTK_FOUND)
  MESSAGE(FATAL_ERROR, "VTK not found. Please set VTK_DIR.")
ENDIF (VTK_FOUND)

INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR}/Command
 ${CMAKE_CURRENT_SOURCE_DIR}/DistanceCalculate
 ${CMAKE_CURRENT_SOURCE_DIR}/DistanceCalculate/Array1D
 ${CMAKE_CURRENT_SOURCE_DIR}/DistanceCalculate/Array2D
 ${CMAKE_CURRENT_SOURCE_DIR}/DistanceCalculate/Boundary
 ${CMAKE_CURRENT_SOURCE_DIR}/DistanceCalculate/CalculateAngle
 ${CMAKE_CURRENT_SOURCE_DIR}/DistanceCalculate/HashMap
 ${CMAKE_CURRENT_SOURCE_DIR}/DistanceCalculate/Neighborhood
 ${CMAKE_CURRENT_SOURCE_DIR}/DistanceCalculate/WriteVector
 ${CMAKE_CURRENT_SOURCE_DIR}/Gui
 ${CMAKE_CURRENT_SOURCE_DIR}/Gui/FullScreen
 ${CMAKE_CURRENT_SOURCE_DIR}/Gui/ImageV
 ${CMAKE_CURRENT_SOURCE_DIR}/Gui/Pipeline
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
)


SET(CXX_DEFAULT_FILES 
	Gui/ImageSyl.cxx
	DistanceCalculate/Boundary/boundary.cxx
	DistanceCalculate/toolsdistance.cxx
	DistanceCalculate/CalculateAngle/CalculateAngle.cxx
	DistanceCalculate/HashMap/HashMapDistance.cxx
	DistanceCalculate/Array1D/Tableau1D.cxx
	DistanceCalculate/Neighborhood/Neighborhood.cxx
	DistanceCalculate/Array2D/Tableau2D.cxx
	DistanceCalculate/WriteVector/Vector3D.cxx
	Command/PipelineCommand.cxx
	Command/CommandLineArgumentParser.cxx	
	)

SET(CXX_FILES ${CXX_DEFAULT_FILES} CortThick.cxx)

ADD_EXECUTABLE(CortThick ${CXX_FILES})
TARGET_LINK_LIBRARIES(CortThick ${ITK_LIBRARIES} ${VTK_LIBRARIES} vtkRendering)

FIND_PACKAGE(FLTK QUIET)
IF(FLTK_FOUND)
  INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
  FIND_PACKAGE(GLUT REQUIRED)
  IF (GLUT_FOUND)
    INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})

	INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/FindInsightApplications.cmake)
	INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
	INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)
	
	INCLUDE_DIRECTORIES(InsightApplications/Auxiliary/FltkImageViewer)

	SET(CXX_FILES 
	CortThick_gui.cxx
	Gui/ImageV/ImageVGUIControls.cxx
	Gui/FullScreen/FullScreenGUIControls.cxx
	Gui/Pipeline/PipelineGUIControls.cxx
	${CXX_DEFAULT_FILES}
	)
	
	SET(GUI_FILES
	Gui/ImageV/ImageVGUI.fl
	Gui/FullScreen/FullScreenGUI.fl
	Gui/Pipeline/PipelineGUI.fl
	)
	
	LINK_DIRECTORIES(${FLTK_LIBRARY_DIRS})
	FLTK_WRAP_UI(CortThick_gui ${GUI_FILES})

	ADD_EXECUTABLE(CortThick_gui ${CXX_FILES} ${CortThick_gui_FLTK_UI_SRCS})

	TARGET_LINK_LIBRARIES(CortThick_gui
	ITKFltkImageViewer
	${OPENGL_glu_LIBRARY}
	${OPENGL_LIBRARY}
	${ITK_LIBRARIES}
	${FLTK_LIBRARIES}
	${OPENGL_LIBRARIES}
	${VTK_LIBRARIES}
	vtkRendering
	)
  ENDIF (GLUT_FOUND)
ELSE(FLTK_FOUND)
  MESSAGE(WARNING, " FLTK not found. CortThick_gui will not be build. If you also want to compile the GUI, please set FLTK_DIR.")
ENDIF(FLTK_FOUND)


FIND_PACKAGE(GenerateCLP QUIET)
IF(GenerateCLP_FOUND)

  INCLUDE(${GenerateCLP_USE_FILE})
  GENERATECLP(CortThickCLP.cxx CortThickCLP.xml )

	SET(CXX_FILES CortThickCLP.cxx ${CXX_DEFAULT_FILES})

	ADD_EXECUTABLE(CortThickCLP ${CXX_FILES})
	TARGET_LINK_LIBRARIES(CortThickCLP ${ITK_LIBRARIES} ${VTK_LIBRARIES} vtkRendering)

ELSE(GenerateCLP_FOUND)
  MESSAGE(WARNING, " GenerateCLP not found. CortThickCLP will not be build. If you also want to compile the CLP version (for a Slicer use), please set GenerateCLP_DIR.")
ENDIF(GenerateCLP_FOUND)
