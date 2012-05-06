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

//Load the QT form elements
void FirstPlugin::initializePlugin(){
	
  //create widgets
 toolBox_ = new QWidget();

  labelStaticText_ = new QLabel("Current Count: ", toolBox_);
  labelDynamicText_ = new QLabel("0", toolBox_);

  buttonCountTargetObjects_ = new QPushButton("Count &Objects", toolBox_);
  buttonCountTargetVertices_ = new QPushButton("Count &Vertices", toolBox_);
  buttonClearSelection_ = new QPushButton(tr("Clear Selection"), toolBox_);
  buttonTestSegmentController_ = new QPushButton(tr("Test Segment Controller"),toolBox_);
  buttonSegmentAdd_ = new QPushButton(tr("Add Segment"));
  buttonSegmentDelete_	= new QPushButton(tr("Delete Segment"));
  buttonSegmentAddVertices_ = new QPushButton(tr("Add Selected Vertices"));
  buttonSegmentRemoveVertices_ = new QPushButton(tr("Remove Selected Vertices"));



  layout_ = new QGridLayout(toolBox_);

  layout_->addWidget(labelDynamicText_,0,0);
  layout_->addWidget(labelStaticText_,1,0);
  layout_->addWidget(buttonCountTargetVertices_,2,0);
  layout_->addWidget(buttonCountTargetObjects_,2,1);
  layout_->addWidget(buttonClearSelection_,3,0);
  layout_->addWidget(buttonTestSegmentController_,3,1);
  //
  //create buttons for segmentation
  layout_->addWidget(buttonSegmentAdd,5,0);
  layout_->addWidget(buttonSegmentDelete_,5,1);
  layout_->addWidget(buttonSegmentAddVertices_,6,0);
  layout_->addWidget(buttonSegmentRemoveVertices_,6,1);	
  
  //slot the buttons here
  connect(buttonCountTargetObjects_,SIGNAL(clicked()),this, SLOT(CountTargetObjects()));
  connect(buttonCountTargetVertices_,SIGNAL(clicked()),this,SLOT(CountTargetVertices()));
  connect(buttonClearSelection_,SIGNAL(clicked()),this,SLOT(ClearTargetsClicked()));
  connect(buttonTestSegmentController_,SIGNAL(clicked()),this,SLOT(onClick_buttonTestSegmentController()));

  //TODO:Slot segmentation buttons

  emit addToolbox(tr("FirstPlugin"),toolBox_);
}


//link any signals to scriptable slots on external plugins
void FirstPlugin::pluginsInitialized(){
    log(QString("Linking script calls"));
    crossPluginConnect(QString("firstplugin"),SIGNAL(doClearTargets()),QString("primitivesgeneratorplugin"),SLOT(addSphere()));
    
}

//updates the toolbox on the selected verties
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

//counts the number of selected vertices on all target objects and then updates the label on the toolbox
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
void FirstPlugin::onClick_buttonTestSegmentController(){
  log("Testing Controller class");
  PluginFunctions::ObjectIterator o_it = PluginFunctions::ObjectIterator(PluginFunctions::TARGET_OBJECTS), E = PluginFunctions::objectsEnd();
  if(o_it != E){

    if(o_it->dataType(DATA_TRIANGLE_MESH)) {
		//build a vector of selected vertices
	    std::vector<TriMesh::VertexHandle> vertices;
      TriMesh* Tmesh = PluginFunctions::triMesh(*o_it);
      TriMesh::VertexIter itV, itE;
      itV = Tmesh->vertices_begin();
      itE = Tmesh->vertices_end();
	  TriMeshSegmentCollection* segmentCollection;

      for(; itV!=itE; ++itV)
	      if(Tmesh->status(itV).selected()) vertices.push_back(itV.handle());
      Tmesh->handle();
      //INSERT SEGMENTER CLASS CODE HERE
	  //
	  ////check if the mesh has a SegmentCollection property
	  //OpenMesh::MPropHandleT<TriMeshSegmentCollection> SC_h;
   //   if(!Tmesh->get_property_handle(SC_h, TriMeshSegmentCollection::defaultName()){
		 // Tmesh->add_property(SC_h,TriMeshSegmentCollection.defaultName()); //if not create it
   //       Tmesh->property(SC_h).set_persistent(true);
	  //}
	  //
	  ////create the segment item
	  //TriMeshSegment* segment  = new TriMeshSegment;
	  //segment->vertexHandles_ = vertices;

	  ////add it to the collection
   //   TriMeshSegment::SegmentHandle s_h = Tmesh->property(SC_h).addSegment(segment);

   //     //unselect all vertices to show command complete
      for(int i = 0; i < segment.vertexHandles_.size(); i++){
          Tmesh->status(segment.vertexHandles_[i]).set_selected(false);
      }

    } else {
		std::vector<typename PolyMesh::VertexHandle> vertices;
      PolyMesh* Pmesh = PluginFunctions::polyMesh(*o_it);

      PolyMesh::VertexIter itV, itE;
      itV = Pmesh->vertices_begin();
      itE = Pmesh->vertices_end();
      for(; itV!=itE; ++itV)
	      if(Pmesh->status(itV).selected()) vertices.push_back(itV.handle());
		

	  //INSERT SEGMENTER CLASS CODE HERE
   	  emit updatedObject(o_it->id(), UPDATE_SELECTION_VERTICES);
	  emit updateView();
    }
  } else {
	  log(QString("No objects found"));
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


void onClick_buttonSegmentAdd(){

}

void onClick_buttonSegmentDelete(){

}

void onClick_buttonSegmentAddVertices(){

}

void onClick_buttonSegmentRemoveVertices(){

}
 Q_EXPORT_PLUGIN2(firstPlugin, FirstPlugin);
