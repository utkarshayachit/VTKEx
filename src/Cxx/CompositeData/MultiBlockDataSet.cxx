// Demonstrates how to make and use VTK's MultiBlock type data

#include <vtkActor.h>
#include <vtkCompositeDataGeometryFilter.h>
#include <vtkExtractEdges.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkVector.h>

namespace {
vtkSmartPointer<vtkDataSet> CreateSphereDataSet(const vtkVector3d& center,
                                                double radius = 0.5)
{
  vtkNew<vtkSphereSource> leaf;
  leaf->SetCenter(center.GetData());
  leaf->SetRadius(radius);
  leaf->Update();
  return leaf->GetOutput();
}

} // namespace

int main (int, char *[])
{
  // PART 1 Make some Data
  // make a tree
  vtkNew<vtkMultiBlockDataSet> root;

  vtkNew<vtkMultiBlockDataSet> branch;
  root->SetBlock(0, branch);

  // make some leaves
  branch->SetBlock(0, ::CreateSphereDataSet({0, 0, 0}));
  branch->SetBlock(1, ::CreateSphereDataSet({1.75, 2.5, 0.0}, 1.5));
  root->SetBlock(1, ::CreateSphereDataSet({4, 0, 0}, 2.0));

  // uncomment to inspect
  //std::cerr << root->GetClassName() << std::endl;
  //root->PrintSelf(std::cerr, vtkIndent(0));

  // PART 2 Do something with the data
  // a non composite aware filter, the pipeline will iterate
  vtkNew<vtkExtractEdges> edges;
  edges->SetInputData(root);

  // uncomment to inspect
  //edges->Update();
  //cerr << edges->GetOutputDataObject(0)->GetClassName() << endl;
  //edges->GetOutputDataObject(0)->PrintSelf(std::cerr, vtkIndent(0));

  // PART 3 Show the data
  // also demonstrate a composite aware filter
  // this filter aggregates all blocks into one polydata
  // this is handy for display, although fairly limited
  // see vtkCompositePolyDataMapper2 for something better
  vtkNew<vtkCompositeDataGeometryFilter> polydata;
  polydata->SetInputConnection(edges->GetOutputPort());

  // uncomment to inspect
  //polydata->Update();
  //std::cerr << polydata->GetOutput()->GetClassName() << std::endl;
  //polydata->GetOutput()->PrintSelf(std::cerr, vtkIndent(0));

  // display the data
  vtkNew<vtkRenderer> aren;
  vtkNew<vtkRenderWindow> renWin;
  renWin->AddRenderer(aren);
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin);

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(0, polydata->GetOutputPort(0));
  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  aren->AddActor(actor);

  renWin->Render();
  iren->Start();

  return EXIT_SUCCESS;
}
