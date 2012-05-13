#include "firstPlugin.hh"
#include <vector>
#include <string>
#include <algorithm>
#include <../ObjectTypes/PolyMesh/PolyMesh.hh>
#include <../ObjectTypes/TriangleMesh/TriangleMesh.hh>


#include "OpenFlipper/BasePlugin/PluginFunctions.hh"



FirstPlugin::FirstPlugin() :  labelStaticText_(0), labelDynamicText_(0),
                               buttonCountTargetObjects_(0), buttonCountTargetVertices_(0),
                               buttonClearSelection_(0), buttonTestSegmentController_(0),
			       buttonSegmentAdd_(0), buttonSegmentDelete_(0), buttonSegmentRemoveVertices_(0),
			       buttonSegmentAddVertices_(0)
{
}

FirstPlugin::~FirstPlugin(){
    if (meshSegmentCollectionBase_ != 0) delete meshSegmentCollectionBase_;
}

//Load the QT form elements
void FirstPlugin::initializePlugin(){

    //initialise member variables
    meshSegmentCollectionBase_ = 0;
	
  //Initialisation
  //*****************
 toolBox_ = new QWidget();

  labelStaticText_ = new QLabel("Current Count: ", toolBox_);
  labelDynamicText_ = new QLabel("0", toolBox_);

  //QPushButton
  buttonCountTargetObjects_ = new QPushButton("Count &Objects", toolBox_);
  buttonCountTargetVertices_ = new QPushButton("Count &Vertices", toolBox_);
  buttonClearSelection_ = new QPushButton(tr("Clear Selection"), toolBox_);
  buttonTestSegmentController_ = new QPushButton(tr("Run Test Routine"),toolBox_);
  buttonSegmentAdd_ = new QPushButton(tr("Add Segment"));
  buttonSegmentDelete_	= new QPushButton(tr("Delete Segment"));
  buttonSegmentAddVertices_ = new QPushButton(tr("Add Selected Vertices"));
  buttonSegmentRemoveVertices_ = new QPushButton(tr("Remove Selected Vertices"));

  //QComboBox
  comboSegments_ = new QComboBox(toolBox_);


  layout_ = new QGridLayout(toolBox_);

  //layout*
  //**********************
  layout_->addWidget(labelDynamicText_,0,0);
  layout_->addWidget(labelStaticText_,1,0);
  layout_->addWidget(buttonCountTargetVertices_,2,0);
  layout_->addWidget(buttonCountTargetObjects_,2,1);
  layout_->addWidget(buttonClearSelection_,3,0);
  layout_->addWidget(buttonTestSegmentController_,3,1);

  layout_->addWidget(buttonSegmentAdd_,5,0);
  layout_->addWidget(buttonSegmentDelete_,5,1);
  layout_->addWidget(buttonSegmentAddVertices_,6,0);
  layout_->addWidget(buttonSegmentRemoveVertices_,6,1);	

  layout_->addWidget(comboSegments_,7,0);
  
  //slot the buttons here
  connect(buttonCountTargetObjects_,SIGNAL(clicked()),this, SLOT(CountTargetObjects()));
  connect(buttonCountTargetVertices_,SIGNAL(clicked()),this,SLOT(CountTargetVertices()));
  connect(buttonClearSelection_,SIGNAL(clicked()),this,SLOT(ClearTargetsClicked()));
  connect(buttonTestSegmentController_,SIGNAL(clicked()),this,SLOT(onClick_buttonTestSegmentController()));
  //buttons for v2 segmenter
  connect(buttonSegmentAdd_,SIGNAL(clicked()),this,SLOT(onClick_buttonSegmentAdd()));
  connect(buttonSegmentDelete_,SIGNAL(clicked()),this,SLOT(onClick_buttonSegmentDelete()));
  connect(buttonSegmentAddVertices_,SIGNAL(clicked()),this,SLOT(onClick_buttonSegmentAddVertices()));
  connect(buttonSegmentRemoveVertices_,SIGNAL(clicked()),this,SLOT(onClick_buttonSegmentRemoveVertices()));

  emit addToolbox(tr("FirstPlugin"),toolBox_);
}


//link any signals to scriptable slots on external plugins
void FirstPlugin::pluginsInitialized(){
    log(QString("Linking script calls"));
    //connect external slots
    crossPluginConnect(QString("firstplugin"),SIGNAL(doClearTargets()),QString("primitivesgeneratorplugin"),SLOT(addSphere()));
    crossPluginConnect(QString("firstplugin"),SIGNAL(signalCreateCube()),QString("primitivesgeneratorplugin"),SLOT(addTriangulatedCube()));
    crossPluginConnect(QString("firstplugin"),SIGNAL(signalDeleteObject(ObjectHandle)) ,QString("core"),SLOT(deleteObject(ObjectHandle)));
    crossPluginConnect(QString("firstplugin"),SIGNAL(signalTransformObject(ObjectHandle, Matrix4x4)), QString("move"),SLOT(transform(ObjectHandle,Matrix4x4)));
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

///Call test routines on the segment selected in comboSegments_
///Identify the type and pass to the template equivalent
void FirstPlugin::onClick_buttonTestSegmentController(){
  log("Button Not Currently Implemented");
  

  //test one:
  //clear mesh selection
  //select vertices from current segment
  //test two:
  //generate and adisplay bounding box
  
  if(comboSegments_->count() == 0){
	  log(QString("No Segments Detected"));
	  return;
  }
  if(meshSegmentCollectionBase_->isPolyMesh())  TestSegmentControllerT<PolyMesh>();
  else if(meshSegmentCollectionBase_->isTriMesh()) TestSegmentControllerT<TriMesh>();
  else log(QString("segment handler is not defined using a compatable mesh type"));
}

template <typename myMesh> void FirstPlugin::TestSegmentControllerT(){
	//generate a SegmentDescriptor bounding box and log the resulting minmax coordinates
    typedef MeshSegmentT<myMesh> Segment;
    typedef MeshSegmentCollectionT<myMesh> SegmentCollection ;
	Segment::SegmentHandle segmentHandle = comboSegments_->itemData(comboSegments_->currentIndex()).toUInt();
	SegmentCollection* segmentCollection = (SegmentCollection*)meshSegmentCollectionBase_;
    Segment* segment = (*segmentCollection)[segmentHandle];
    
	SegmentDescriptorT<myMesh> descriptor(segment->mesh(), segment->vertexHandleSet());
	QString p1 = "%1, %2, %3";
       QString	p2 = p1;
	for(int i =0; i<3;++i){
		p1 = p1.arg(descriptor.maxPoint()[i]);
		p2 = p2.arg(descriptor.minPoint()[i]);
	}
	log(QString("MaxPoint: ").append(p1));
	log(QString("MinPoint: ").append(p2));
}

template void FirstPlugin::TestSegmentControllerT<PolyMesh>();
template void FirstPlugin::TestSegmentControllerT<TriMesh>();

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


void FirstPlugin::onClick_buttonSegmentAdd(){
	log(QString("buttonSegmentAdd clicked!"));
    //if necesary initialise the segment collection
    if(meshSegmentCollectionBase_ == 0){
        PluginFunctions::ObjectIterator o_it = PluginFunctions::ObjectIterator(PluginFunctions::TARGET_OBJECTS), o_end = PluginFunctions::objectsEnd();
         if(o_it != o_end){
           if(o_it->dataType(DATA_TRIANGLE_MESH)) {
               TriMeshSegmentCollection* tmSC = new TriMeshSegmentCollection();
               meshSegmentCollectionBase_ = tmSC;
           }
           else if(o_it->dataType(DATA_POLY_MESH)){
               PolyMeshSegmentCollection* pmSC = new PolyMeshSegmentCollection();
               meshSegmentCollectionBase_ = pmSC;
           }
           else {
               log(QString("Invalid Mesh Type: Current supports TriMesh and PolyMesh"));
           }
         }
    }

   PluginFunctions::ObjectIterator o_it = PluginFunctions::ObjectIterator(PluginFunctions::TARGET_OBJECTS), e = PluginFunctions::objectsEnd();
   if(o_it == e){
       log("No selected items");
       return;
   }

    //generate a new segment
    if(meshSegmentCollectionBase_->isTriMesh()){
	    TriMesh* tmesh = PluginFunctions::triMesh(*o_it);
        TriMeshSegmentCollection* tmsc = (TriMeshSegmentCollection*)meshSegmentCollectionBase_;
	    TriMeshSegment* tms = tmsc->add(tmesh);
    } else if(meshSegmentCollectionBase_->isPolyMesh()){
        PolyMesh* pmesh = PluginFunctions::polyMesh(*o_it);
        PolyMeshSegmentCollection* pmsc = (PolyMeshSegmentCollection*)meshSegmentCollectionBase_;
	    PolyMeshSegment* pms = pmsc->add(pmesh);
    }
    
    //refresh the comboBox
    refreshCombo();
    
    
}

template <typename myMesh> void FirstPlugin::addSegmentT(myMesh _mesh, std::string _name){
    //generate container of selected vertices
    std::vector<typename myMesh::VertexHandle> vertices;
    myMesh::VertexIter itV, itE;
    itV = _mesh->vertices_begin();
    itE = _mesh->vertices_end();
    for(; itV!=itE; ++itV)
        if(Tmesh->status(itV).selected()) vertices.push_back(itV.handle());
}


void FirstPlugin::onClick_buttonSegmentDelete(){
	int index = comboSegments_->currentIndex();
    //remove the current segment from the array
     

    if(meshSegmentCollectionBase_->isTriMesh()){
        TriMeshSegment::SegmentHandle handle = comboSegments_->itemData(index).toUInt();
	    TriMeshSegmentCollection* tsc = (TriMeshSegmentCollection*)meshSegmentCollectionBase_;
        tsc->remove(handle);
    } else if(meshSegmentCollectionBase_->isPolyMesh()){
        PolyMeshSegment::SegmentHandle handle = comboSegments_->itemData(index).toUInt();
	    PolyMeshSegmentCollection* psc = (PolyMeshSegmentCollection*)meshSegmentCollectionBase_;
        psc->remove(handle);
    }
    //refresh the comboBox
    refreshCombo();
}

void FirstPlugin::onClick_buttonSegmentAddVertices(){
  PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS);
    if (o_it->dataType(DATA_TRIANGLE_MESH) && meshSegmentCollectionBase_->isTriMesh()) {
        segmentAddVertices(PluginFunctions::triMesh(*o_it));
    } else if (o_it->dataType(DATA_POLY_MESH) && meshSegmentCollectionBase_->isPolyMesh()){
        segmentAddVertices(PluginFunctions::polyMesh(*o_it));
    }
    else{
        log(QString("Incompatable data type"));
    }
}

template <typename myMesh> void FirstPlugin::segmentAddVertices(myMesh* _mesh){
typedef MeshSegmentCollectionT<myMesh> SegmentCollection;
    SegmentCollection::VertexHandleSet vertexHandles;
    SegmentCollection::SegmentHandle segmentHandle;
    segmentHandle = comboSegments_->itemData(comboSegments_->currentIndex()).toUInt();
    log(QString("Segment%1").arg(segmentHandle));
    
    MeshSegmentCollectionT<myMesh>* msc = (MeshSegmentCollectionT<myMesh>*)meshSegmentCollectionBase_;
    msc->harvestSelectedVertices(_mesh, vertexHandles);
    msc->assignVertices(vertexHandles,segmentHandle,_mesh);

    /*if (meshSegmentCollectionBase_->isTriMesh()){
	    TriMeshSegmentCollection* tmsc = (TriMeshSegmentCollection*)meshSegmentCollectionBase_;
	    tmsc->harvestSelectedVertices(_mesh, vertexHandles);
	    tmsc->assignVertices(vertexHandles,segmentHandle,_mesh);
    }
    else if(meshSegmentCollectionBase_->isPolyMesh()){
	    PolyMeshSegmentCollection* pmsc = (PolyMeshSegmentCollection*)meshSegmentCollectionBase_;
	    pmsc->harvestSelectedVertices(_mesh, vertexHandles);
	    pmsc->assignVertices(vertexHandles, segmentHandle,_mesh);
    }*/
}

template void FirstPlugin::segmentAddVertices<PolyMesh>(PolyMesh* _mesh);
template void FirstPlugin::segmentAddVertices<TriMesh>(TriMesh* _mesh);

void FirstPlugin::onClick_buttonSegmentRemoveVertices(){
	//TODO: make it work
	log(QString("Remove Vertices Not Yet Implemented"));
    //harvest selected vertices
    //remove them from the current segment
}

void FirstPlugin::refreshCombo(){

    if (meshSegmentCollectionBase_->isTriMesh()){
        refreshComboT<TriMeshSegmentCollection>((TriMeshSegmentCollection*)meshSegmentCollectionBase_);
    }else if (meshSegmentCollectionBase_->isPolyMesh()){
        refreshComboT<PolyMeshSegmentCollection>((PolyMeshSegmentCollection*)meshSegmentCollectionBase_);
    }
    else {
        log("Cannot refresh combo, invalid segmentatin type");
    }
   
}

template <typename MSC> void FirstPlugin::refreshComboT(MSC *_sc){
    comboSegments_->clear();
    MSC::const_iterator s_it, s_end;
    s_it = _sc->begin(), s_end = _sc->end();
    for(; s_it != s_end; ++s_it){
        QString s(s_it->second->name().c_str());
        comboSegments_->insertItem(0,s,s_it->first);
    }
}
template void FirstPlugin::refreshComboT<TriMeshSegmentCollection>(TriMeshSegmentCollection*);
template void FirstPlugin::refreshComboT<PolyMeshSegmentCollection>(PolyMeshSegmentCollection*);

void FirstPlugin::refreshDisplay(){
	QMap::const_iterator o_it = meshSegmentDispObjMap.begin(), o_end = meshSegmentDispObjMap_.end();
	//remove previous representations
	for(;o_it != o_ent;++o_it){
		//add any bad entries to the list to remove and log an error
		if(!PluginFunctions::objectExists(*o_it)){
			log(QString("Segment Handle %1 Appears in Map but display object does not exist").arg(*o_it));
		}
		else{
			//the object exists so delete it
			emit signalDeleteObject(*o_it);
		}
	}
	meshSegmentDispObjMap_->clear();
	
	SegmentPointerContainer::const_it s_it = segmentPointerContainer_->begin(), s_end = segmentPointerContainer_->end();
	for(;s_it <> s_end; ++s_it){
	}
}

template <typename myMesh> refreshDisplayT(){
	typedef MeshSegmentCollectionT<myMesh>* SegmentContainer;
	SegmentContainer* segmentContainer = (SegmentContainer*)meshSegmentCollectionBase_;
	segmentContainer::const_iterator sp_it = segmentContainer->begin(), sp_end = segmentContainer->end();
	for(; sp_it != sp_end; ++sp_it){
		SegmentHandle h  = emit signalCreateCube();
		meshSegmentDispObjMap_[(sp_it)->handle()] = h;
		BaseObject* obj = 0;
		PluginFunctions::getObject(h, obj);
		emit signalTransformObject(obj,(*sp_it)->segmentDescriptor_.worldTransform());
	}
}
 Q_EXPORT_PLUGIN2(firstPlugin, FirstPlugin);

 /***********************
*standard object iterator decleration:
* 
*	PluginFunctions::ObjectIterator o_it = PluginFunctions::ObjectIterator(PluginFunctions::TARGET_OBJECTS), o_end = PluginFunctions::objectsEnd();
*/
//
//
//
//**************************
//
//PluginFunctions::ObjectIterator o_it = PluginFunctions::ObjectIterator(PluginFunctions::TARGET_OBJECTS), o_end = PluginFunctions::objectsEnd();

  //PluginFunctions::ObjectIterator o_it = PluginFunctions::ObjectIterator(PluginFunctions::TARGET_OBJECTS), E = PluginFunctions::objectsEnd();
  //if(o_it != E){

  //  if(o_it->dataType(DATA_TRIANGLE_MESH)) {
		////build a vector of selected vertices
	 //   std::vector<TriMesh::VertexHandle> vertices;
  //    TriMesh* Tmesh = PluginFunctions::triMesh(*o_it);
  //    TriMesh::VertexIter itV, itE;
  //    itV = Tmesh->vertices_begin();
  //    itE = Tmesh->vertices_end();
	 // TriMeshSegmentCollection* segmentCollection;

  //    for(; itV!=itE; ++itV)
	 //     if(Tmesh->status(itV).selected()) vertices.push_back(itV.handle());
 
  //    for(int i = 0; i < segment.VertexHandleSet_.size(); i++){
  //        Tmesh->status(segment.VertexHandleSet_[i]).set_selected(false);
  //    }

  //  } else {
		//std::vector<typename PolyMesh::VertexHandle> vertices;
  //    PolyMesh* Pmesh = PluginFunctions::polyMesh(*o_it);

  //    PolyMesh::VertexIter itV, itE;
  //    itV = Pmesh->vertices_begin();
  //    itE = Pmesh->vertices_end();
  //    for(; itV!=itE; ++itV)
	 //     if(Pmesh->status(itV).selected()) vertices.push_back(itV.handle());
		//

	 // //INSERT SEGMENTER CLASS CODE HERE
  // 	  emit updatedObject(o_it->id(), UPDATE_SELECTION_VERTICES);
	 // emit updateView();
  //  }
  //} else {
	 // log(QString("No objects found"));
  //}
