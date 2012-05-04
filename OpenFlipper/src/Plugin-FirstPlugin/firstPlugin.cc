#include "firstPlugin.hh"
#include <vector>
#include <string>
#include <algorithm>
#include <../ObjectTypes/PolyMesh/PolyMesh.hh>
#include <../ObjectTypes/TriangleMesh/TriangleMesh.hh>


#include "OpenFlipper/BasePlugin/PluginFunctions.hh"



FirstPlugin::FirstPlugin() :  labelStaticText_(0), labelDynamicText_(0),
                               buttonCountTargetObjects_(0), buttonCountTargetVertices_(0),
                               buttonClearSelection_(0), buttonTestSegmentController_(0)
{
}


void FirstPlugin::initializePlugin(){

  //create widgets
 toolBox_ = new QWidget();

  labelStaticText_ = new QLabel("Current Count: ", toolBox_);
  labelDynamicText_ = new QLabel("0", toolBox_);

  buttonCountTargetObjects_ = new QPushButton("Count &Objects", toolBox_);
  buttonCountTargetVertices_ = new QPushButton("Count &Vertices", toolBox_);
  buttonClearSelection_ = new QPushButton(tr("Clear Selection"), toolBox_);
  buttonTestSegmentController_ = new QPushButton(tr("Test Segment Controller"),toolBox_);

  layout_ = new QGridLayout(toolBox_);

  layout_->addWidget(labelDynamicText_,0,0);
  layout_->addWidget(labelStaticText_,1,0);
  layout_->addWidget(buttonCountTargetVertices_,2,0);
  layout_->addWidget(buttonCountTargetObjects_,2,1);
  layout_->addWidget(buttonClearSelection_,3,0);
  layout_->addWidget(buttonTestSegmentController_,3,1);

  
  //slot the buttons here
  connect(buttonCountTargetObjects_,SIGNAL(clicked()),this, SLOT(CountTargetObjects()));
  connect(buttonCountTargetVertices_,SIGNAL(clicked()),this,SLOT(CountTargetVertices()));
  connect(buttonClearSelection_,SIGNAL(clicked()),this,SLOT(ClearTargetsClicked()));
  connect(buttonTestSegmentController_,SIGNAL(clicked()),this,SLOT(click_TestSegmentController()));

  emit addToolbox(tr("FirstPlugin"),toolBox_);
}

void FirstPlugin::pluginsInitialized(){
    log(QString("Linking script calls"));
    crossPluginConnect(QString("firstplugin"),SIGNAL(doClearTargets()),QString("primitivesgeneratorplugin"),SLOT(addSphere()));
    
}

void FirstPlugin::CountTargetObjects(){
  PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS);

  int countMesh = 0, countTotal = 0;

  log(QString("Starting Count"));

  for(;o_it != PluginFunctions::objectsEnd(); ++o_it)
  {
    log(QString().setNum(++countTotal));
    if(o_it->dataType(DATA_TRIANGLE_MESH) || o_it->dataType(DATA_POLY_MESH)) countMesh++;
  }
  
  labelDynamicText_->setText(QString("%1 Meshes Out Of %2 Objects").arg(countMesh).arg(countTotal));
}

void FirstPlugin::CountTargetVertices(){
  PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS);

  long countVertices=0, countVerticesSelected=0, countObjectsSelected=0;

  log(QString("Vertex Count STarted"));
  
  for(;o_it != PluginFunctions::objectsEnd(); ++o_it)
  {
    countObjectsSelected++;
    if(o_it->dataType(DATA_TRIANGLE_MESH)) {
      TriMesh* Tmesh = PluginFunctions::triMesh(*o_it);
      functorNodeCount<TriMesh> counterT(Tmesh,countVerticesSelected, countVertices);
    std::for_each(Tmesh->vertices_begin(), Tmesh->vertices_end(), counterT);
    }
    else if(o_it->dataType(DATA_POLY_MESH)){
      PolyMesh*  Pmesh = PluginFunctions::polyMesh(*o_it);
      functorNodeCount<PolyMesh> counterP(Pmesh,countVerticesSelected, countVertices);
      std::for_each(Pmesh->vertices_begin(), Pmesh->vertices_end(), counterP);
    } 
    else continue;
  }

  QString lText;
  QTextStream ltStream(&lText);
  ltStream << countObjectsSelected << " Objects, " << countVertices << " Vertices, " 
     << countVerticesSelected << " Selected Vertices";

  labelDynamicText_->setText(lText);


}
  
///emits a signal to test the CrossPluginConnect interface
void FirstPlugin::ClearTargetsClicked(){
  log("starting clear targets");
  emit doClearTargets();
}

/*! generates a SegmentController from the currently selected mesh vertices and performs test operations */
void FirstPlugin::click_TestSegmentController(){
  log("Testing Controller class");
  PluginFunctions::ObjectIterator o_it = PluginFunctions::ObjectIterator(PluginFunctions::TARGET_OBJECTS), E = PluginFunctions::objectsEnd();
  if(o_it != E){
    if(o_it->dataType(DATA_TRIANGLE_MESH)) {
	    std::vector<TriMesh::VertexHandle> vertices;
      TriMesh* Tmesh = PluginFunctions::triMesh(*o_it);
      TriMesh::VertexIter itV, itE;
      itV = Tmesh->vertices_begin();
      itE = Tmesh->vertices_end();
      for(; itV!=itE; ++itV)
	      if(Tmesh->status(itV).selected()) vertices.push_back(itV.handle());
      
   //   SController<TriMesh> TC;
   //   TC._VertexHandles = vertices;
	  //TC._mesh = Tmesh;
   //   TC.unSelectAll();
	  ////emit updatedObject(o_it->id(), UPDATE_SELECTION_VERTICES);
	  ////emit updateView();
	
	  //TriMesh::Point a(0,0,0),b(0,0,0);
	  ////call the openFlipper environment to add an empty mesh objecy
	  //int _BoundingBoxHandle  = addTriMesh();
	  ////get the empty mesh objecy
	  //TriMeshObject* TMO;
	  //PluginFunctions::getObject(_BoundingBoxHandle, TMO);
	  //TC._BoundingCube = TMO->mesh();
	  //TMO->setName( "SegmentController " + QString::number(_BoundingBoxHandle) );
	  //TC.BuildBoundingBox(a,b);
	  //emit updatedObject(_BoundingBoxHandle,UPDATE_ALL);
	  //log(QString("%1, %2, %3, %4,%5,%6").arg(a[0]).arg(a[1]).arg(a[2]).arg(b[0]).arg(b[1]).arg(b[2]));

    } else {
		std::vector<typename PolyMesh::VertexHandle> vertices;
      PolyMesh* Pmesh = PluginFunctions::polyMesh(*o_it);

      PolyMesh::VertexIter itV, itE;
      itV = Pmesh->vertices_begin();
      itE = Pmesh->vertices_end();
      for(; itV!=itE; ++itV)
	      if(Pmesh->status(itV).selected()) vertices.push_back(itV.handle());
      
   //   SController<PolyMesh> PC;
   //   PC._VertexHandles = vertices;
	  //PC._mesh = Pmesh;
	  //
   //   PC.unSelectAll();
	  emit updatedObject(o_it->id(), UPDATE_SELECTION_VERTICES);
	  emit updateView();
    }
  } else {
	  log(QString("No objects found"));
	  //SController<TriMesh> a;
	  //log(QString(a.testAlglib().c_str()));
  }
}

	///Code copied from PluginPrimitiveGenerator
int FirstPlugin::addTriMesh() {
  int objectId;
  objectId = -1;

  emit addEmptyObject(DATA_TRIANGLE_MESH, objectId);
	
  TriMeshObject* object;
  if ( !PluginFunctions::getObject(objectId,object) ) {
    emit log(LOGERR,"Unable to create new Object");
    return -1;
  }

  return objectId;
}


 Q_EXPORT_PLUGIN2(firstPlugin, FirstPlugin);
