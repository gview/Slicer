project(vtkSlicer${MODULE_NAME}ModuleLogic)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "VTK_SLICER_${MODULE_NAME_UPPER}_MODULE_LOGIC_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  )

set(${KIT}_SRCS
  vtkImageConnectivity.cxx
  vtkImageErode.cxx
  vtkImageFillROI.cxx
  vtkImageLabelChange.cxx
  vtkImageSlicePaint.cxx
  vtkImageStash.cxx
  vtkPichonFastMarching.cxx
  vtkPichonFastMarchingPDF.cxx
  )

if(${VTK_VERSION_MAJOR} GREATER 5)
  # Minimum set of libraries already specified using components
else()
  set(VTK_LIBRARIES
    vtkCommon
    vtkFiltering
    vtkIO
    )
endif()

set(${KIT}_TARGET_LIBRARIES
  ${VTK_LIBRARIES}
  )

#-----------------------------------------------------------------------------
SlicerMacroBuildModuleLogic(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )
