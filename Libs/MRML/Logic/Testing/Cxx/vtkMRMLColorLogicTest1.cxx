/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// MRMLLogic includes
#include "vtkMRMLColorLogic.h"

// MRML includes
#include <vtkMRMLColorNode.h>
#include <vtkMRMLColorTableNode.h>
#include <vtkMRMLdGEMRICProceduralColorNode.h>
#include <vtkMRMLPETProceduralColorNode.h>
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkColorTransferFunction.h>
#include <vtkLookupTable.h>
#include <vtkNew.h>
#include <vtkTimerLog.h>

// STD includes

#include "vtkMRMLCoreTestingMacros.h"

//----------------------------------------------------------------------------
namespace
{
bool TestPerformance();
bool TestNodeIDs();
bool TestDefaults();
bool TestCopy();
bool TestProceduralCopy();
}

//----------------------------------------------------------------------------
int vtkMRMLColorLogicTest1(int vtkNotUsed(argc), char * vtkNotUsed(argv)[] )
{
  bool res = true;
  res = TestPerformance() && res;
  res = TestNodeIDs() && res;
  res = TestDefaults() && res;
  res = TestCopy() && res;
  res = TestProceduralCopy() && res;
  return res ? EXIT_SUCCESS : EXIT_FAILURE;
}
namespace
{

//----------------------------------------------------------------------------
bool TestPerformance()
{
  // To load freesurfer files, SLICER_HOME is requested
  //vtksys::SystemTools::PutEnv("SLICER_HOME=..." );
  vtkNew<vtkMRMLScene> scene;
  vtkMRMLColorLogic* colorLogic = vtkMRMLColorLogic::New();

  vtkNew<vtkTimerLog> overallTimer;
  overallTimer->StartTimer();

  colorLogic->SetMRMLScene(scene.GetPointer());

  overallTimer->StopTimer();
  std::cout << "<DartMeasurement name=\"AddDefaultColorNodes\" "
            << "type=\"numeric/double\">"
            << overallTimer->GetElapsedTime() << "</DartMeasurement>" << std::endl;
  overallTimer->StartTimer();

  colorLogic->Delete();

  overallTimer->StopTimer();
  std::cout << "<DartMeasurement name=\"RemoveDefaultColorNodes\" "
            << "type=\"numeric/double\">"
            << overallTimer->GetElapsedTime()
            << "</DartMeasurement>" << std::endl;

  return true;
}

//----------------------------------------------------------------------------
bool TestNodeIDs()
{
  vtkNew<vtkMRMLScene> scene;
  vtkNew<vtkMRMLColorLogic> colorLogic;
  colorLogic->SetMRMLScene(scene.GetPointer());

  vtkMRMLNode* node= 0;
  scene->InitTraversal();
  while ( (node = scene->GetNextNodeByClass("vtkMRMLColorTableNode")) )
    {
    vtkMRMLColorTableNode* colorNode =
      vtkMRMLColorTableNode::SafeDownCast(node);
    const char* nodeID =
      vtkMRMLColorLogic::GetColorTableNodeID(colorNode->GetType());
    if (strcmp(colorNode->GetID(), nodeID) != 0)
      {
      std::cout << "Failed to generate color table node ID for "
                << colorNode->GetType() << std::endl;
      return false;
      }
    }
  scene->InitTraversal();
  while ( (node = scene->GetNextNodeByClass("vtkMRMLPETProceduralColorNode")) )
    {
    vtkMRMLPETProceduralColorNode* colorNode =
      vtkMRMLPETProceduralColorNode::SafeDownCast(node);
    const char* nodeID =
      vtkMRMLColorLogic::GetPETColorNodeID(colorNode->GetType());
    if (strcmp(colorNode->GetID(), nodeID) != 0)
      {
      std::cout << "Failed to generate color table node ID for "
                << colorNode->GetType() << std::endl;
      return false;
      }
    }
  scene->InitTraversal();
  while ( (node = scene->GetNextNodeByClass("vtkMRMLdGEMRICProceduralColorNode")) )
    {
    vtkMRMLdGEMRICProceduralColorNode* colorNode =
      vtkMRMLdGEMRICProceduralColorNode::SafeDownCast(node);
    const char* nodeID =
      vtkMRMLColorLogic::GetdGEMRICColorNodeID(colorNode->GetType());
    if (strcmp(colorNode->GetID(), nodeID) != 0)
      {
      std::cout << "Failed to generate color table node ID for "
                << colorNode->GetType() << std::endl;
      return false;
      }
    }
  // To test free surfers, SLICER_HOME env variable needs to be set.
  return true;
}

//----------------------------------------------------------------------------
bool TestDefaults()
{
  vtkNew<vtkMRMLScene> scene;
  vtkNew<vtkMRMLColorLogic> colorLogic;
  colorLogic->SetMRMLScene(scene.GetPointer());
  if (scene->GetNodeByID(colorLogic->GetDefaultVolumeColorNodeID()) == 0)
    {
    std::cout << "Can't find default volume color node with ID: "
              << colorLogic->GetDefaultVolumeColorNodeID() << std::endl;
    return false;
    }
  if (scene->GetNodeByID(colorLogic->GetDefaultLabelMapColorNodeID()) == 0)
    {
    std::cout << "Can't find default labelmap color node with ID: "
              << colorLogic->GetDefaultLabelMapColorNodeID() << std::endl;
    return false;
    }
  if (scene->GetNodeByID(colorLogic->GetDefaultEditorColorNodeID()) == 0)
    {
    std::cout << "Can't find default editor color node with ID: "
              << colorLogic->GetDefaultEditorColorNodeID() << std::endl;
    return false;
    }
  if (scene->GetNodeByID(colorLogic->GetDefaultModelColorNodeID()) == 0)
    {
    std::cout << "Can't find default model color node with ID: "
              << colorLogic->GetDefaultModelColorNodeID() << std::endl;
    return false;
    }
  if (scene->GetNodeByID(colorLogic->GetDefaultChartColorNodeID()) == 0)
    {
    std::cout << "Can't find default chart color node with ID: "
              << colorLogic->GetDefaultChartColorNodeID() << std::endl;
    return false;
    }
  //if (scene->GetNodeByID(colorLogic->GetDefaultFreeSurferLabelMapColorNodeID()) == 0)
  //  {
  //  std::cout << "Can't find default free surfer color node with ID: "
  //            << colorLogic->GetDefaultFreeSurferLabelMapColorNodeID() << std::endl;
  //  return false;
  //  }
  return true;
}

//----------------------------------------------------------------------------
bool TestCopy()
{
  vtkNew<vtkMRMLScene> scene;
  vtkNew<vtkMRMLColorLogic> colorLogic;
  colorLogic->SetMRMLScene(scene.GetPointer());

  vtkNew<vtkMRMLColorTableNode> originalNode;
  originalNode->SetTypeToFile();
  originalNode->NamesInitialisedOff();
  originalNode->SetNumberOfColors(6);
  originalNode->GetLookupTable()->SetTableRange(0, 5);
  originalNode->SetColor(0, "background", 0.0, 0.0, 0.0, 0.0);
  originalNode->SetColor(1, "one", 0.5, 1.0, 0.0, 0.1);
  originalNode->SetColor(2, "two", 0.5, 0.5, 0.0, 0.3);
  originalNode->SetColor(3, "three", 0.33, 0.0, 0.5, 0.5);
  originalNode->SetColor(4, "four", 0.75, 0.0, 1.0, 0.7);
  originalNode->SetColor(5, "five and done", 1.0, 1.0, 1.0, 1.0);
  originalNode->NamesInitialisedOn();

  vtkMRMLColorTableNode *copiedNode = colorLogic->CopyNode(originalNode.GetPointer(), "Copied Generic");
  if (!copiedNode)
    {
    std::cerr << "Failed to create a copy of the generic colors node" << std::endl;
    return false;
    }

  // check the copy
  double originalColor[4];
  double copyColor[4];
  int numToTest = originalNode->GetNumberOfColors();
  for (int i = 0; i < numToTest; ++i)
    {
    if (!originalNode->GetColor(i, originalColor))
      {
      std::cerr << "Failed to get color " << i << " from the origianl node." << std::endl;
      return false;
      }
    if (!copiedNode->GetColor(i, copyColor))
      {
      std::cerr << "Failed to get color " << i << " from the copied node." << std::endl;
      return false;
      }
    if (copyColor[0] != originalColor[0] ||
        copyColor[1] != originalColor[1] ||
        copyColor[2] != originalColor[2] ||
        copyColor[3] != originalColor[3])
      {
      std::cerr << "Copy failed to copy color " << i << ", expected "
                << originalColor[0] << "," << originalColor[1] << "," << originalColor[2] << "," << originalColor[3]
                << ", but got "
                << copyColor[0] << "," << copyColor[1] << "," << copyColor[2] << "," << copyColor[3]
                << std::endl;
      return false;
      }
    const char *originalColorName = originalNode->GetColorName(i);
    const char *copyColorName = copiedNode->GetColorName(i);
    if (originalColorName != NULL && copyColorName != NULL &&
        strcmp(originalColorName, copyColorName) != 0)
      {
      std::cerr << "Failed to copy color name for color number " << i
                << ", expected '" << originalColorName << "', but got '"
                << copyColorName << "'" << std::endl;
      return false;
      }
    }
  copiedNode->Delete();
  return true;
}

//----------------------------------------------------------------------------
bool TestProceduralCopy()
{
  vtkNew<vtkMRMLScene> scene;
  vtkNew<vtkMRMLColorLogic> colorLogic;
  colorLogic->SetMRMLScene(scene.GetPointer());

  vtkNew<vtkMRMLProceduralColorNode> originalNode;
  originalNode->SetTypeToFile();
  originalNode->NamesInitialisedOff();
  originalNode->SetAttribute("Category", "Continuous");

  vtkColorTransferFunction *func = originalNode->GetColorTransferFunction();
  func->SetColorSpaceToRGB();
  func->AddRGBPoint(-3.0, 1.0, 0.0, 0.0);
  func->AddRGBPoint(0.0, 0.0, 1.0, 0.0);
  func->AddRGBPoint(3.0, 0.0, 0.0, 1.0);
  //  func->AdjustRange(-1.0, 1.0);

  originalNode->SetNamesFromColors();

  originalNode->NamesInitialisedOn();

  vtkMRMLProceduralColorNode *copiedNode = colorLogic->CopyProceduralNode(originalNode.GetPointer(), "Copied Proc");
  if (!copiedNode)
    {
    std::cerr << "Failed to create a copy of a continuous colors node" << std::endl;
    return false;
    }

  // check the copy
  double originalColor[4];
  double copyColor[4];
  int numToTest = originalNode->GetNumberOfColors();
  for (int i = 0; i < numToTest; ++i)
    {
    if (!originalNode->GetColor(i, originalColor))
      {
      std::cerr << "Failed to get color " << i << " from the original node." << std::endl;
      return false;
      }
    if (!copiedNode->GetColor(i, copyColor))
      {
      std::cerr << "Failed to get color " << i << " from the copied node." << std::endl;
      return false;
      }
    if (copyColor[0] != originalColor[0] ||
        copyColor[1] != originalColor[1] ||
        copyColor[2] != originalColor[2] ||
        copyColor[3] != originalColor[3])
      {
      std::cerr << "Copy failed to copy color " << i << ", expected "
                << originalColor[0] << "," << originalColor[1] << "," << originalColor[2] << "," << originalColor[3]
                << ", but got "
                << copyColor[0] << "," << copyColor[1] << "," << copyColor[2] << "," << copyColor[3]
                << std::endl;
      return false;
      }
    const char *originalColorName = originalNode->GetColorName(i);
    const char *copyColorName = copiedNode->GetColorName(i);
    if (originalColorName != NULL && copyColorName != NULL &&
        strcmp(originalColorName, copyColorName) != 0)
      {
      std::cerr << "Failed to copy color name for color number " << i << ", expected '" << originalColorName << "', but got '" << copyColorName << "'" << std::endl;
      return false;
      }
    }
  copiedNode->Delete();
  return true;
}

}
