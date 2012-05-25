#include "ComponentControllersPlugin.hh"


#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <../ObjectTypes/PolyMesh/PolyMesh.hh>
#include <../ObjectTypes/TriangleMesh/TriangleMesh.hh>


ComponentControllersPlugin::ComponentControllersPlugin() : labelObjects_(0), labelSegments_(0), labelSegmentOperations_(0), labelVertexOperations_(0), labelAdvanced_(0), comboObjects_(0), comboSegments_(0), buttonRefreshObjects_(0), buttonSegmentColor_(0), buttonSegmentHighlight_(0), buttonSegmentAdd_(0), buttonSegmentClear_(0), buttonSegmentRemove_(0), buttonVertexAdd_(0), buttonVertexClear_(0), buttonAdvancedGrow_(0), buttonAdvancedGrowAll_(0){
}


ComponentControllersPlugin::~ComponentControllersPlugin(){
}

//QT initilisers
void ComponentControllersPlugin::initializePlugin(){

    toolBox_ = new QWidget();

    layout_ = new QGridLayout(toolBox_);

    labelObjects_ = new QLabel("labelObjects_",toolBox_);
    labelSegments_ = new QLabel("labelSegments_",toolBox_);

    labelSegmentOperations_ = new QLabel("labelSegmentOperations_",toolBox_);
    labelVertexOperations_ = new QLabel("labelVertexOperations_",toolBox_);
    labelAdvanced_ = new QLabel("labelAdvanced_",toolBox_);

    comboObjects_ = new QComboBox(toolBox_);
    comboSegments_ = new QComboBox(toolBox_);

    buttonRefreshObjects_ = new QPushButton("buttonRefreshObjects_",toolBox_);

    buttonSegmentColor_ = new QPushButton("buttonSegmentColor_",toolBox_);
    buttonSegmentHighlight_ = new QPushButton("buttonSegmentHighlight_",toolBox_);
    buttonSegmentAdd_ = new QPushButton("buttonSegmentAdd_",toolBox_);
    buttonSegmentClear_ = new QPushButton("buttonSegmentClear_",toolBox_);
    buttonSegmentRemove_ = new QPushButton("buttonSegmentRemove_",toolBox_);

    buttonVertexAdd_ = new QPushButton("buttonVertexAdd_",toolBox_);
    buttonVertexClear_ = new QPushButton("buttonVertexClear_",toolBox_);


    buttonAdvancedGrow_ = new QPushButton("buttonAdvancedGrow_",toolBox_);
    buttonAdvancedGrowAll_ = new QPushButton("buttonAdvancedGrowAll_",toolBox_);

    

    layout_->addWidget(labelObjects_, 0,0);
    layout_->addWidget(comboObjects_, 0,1);
    layout_->addWidget(buttonRefreshObjects_, 0,2);

    layout_->addWidget(labelSegments_, 1,0);
    layout_->addWidget(comboSegments_, 1,1);

    layout_->addWidget(labelSegmentOperations_, 2,0);
    layout_->addWidget(buttonSegmentColor_, 3,0);
    layout_->addWidget(buttonSegmentHighlight_, 3,1);
    layout_->addWidget(buttonSegmentAdd_, 4,0);
    layout_->addWidget(buttonSegmentClear_, 4,1);
    layout_->addWidget(buttonSegmentRemove_, 4,2);

    layout_->addWidget(labelVertexOperations_, 5,0);
    layout_->addWidget(buttonVertexAdd_, 6,0);
    layout_->addWidget(buttonVertexClear_, 6,1);

    layout_->addWidget(labelAdvanced_, 7,0);
    layout_->addWidget(buttonAdvancedGrow_, 8,0);
    layout_->addWidget(buttonAdvancedGrowAll_, 8,1);




    connect(buttonRefreshObjects_,SIGNAL(clicked()),this,SLOT(onClick_buttonRefreshObjects()));

    connect(buttonSegmentColor_,SIGNAL(clicked()),this,SLOT(onClick_buttonSegmentColor()));
    connect(buttonSegmentHighlight_,SIGNAL(clicked()),this,SLOT(onClick_buttonSegmentHighlight()));
    connect(buttonSegmentAdd_,SIGNAL(clicked()),this,SLOT(onClick_buttonSegmentAdd()));
    connect(buttonSegmentClear_,SIGNAL(clicked()),this,SLOT(onClick_buttonSegmentClear()));
    connect(buttonSegmentRemove_,SIGNAL(clicked()),this,SLOT(onClick_buttonSegmentRemove()));

    connect(buttonVertexAdd_,SIGNAL(clicked()),this,SLOT(onClick_buttonVertexAdd()));
    connect(buttonVertexClear_,SIGNAL(clicked()),this,SLOT(onClick_buttonVertexClear()));


    connect(buttonAdvancedGrow_,SIGNAL(clicked()),this,SLOT(onClick_buttonAdvancedGrow()));
    connect(buttonAdvancedGrowAll_,SIGNAL(clicked()),this,SLOT(onClick_buttonAdvancedGrowAll()));


    emit addToolbox(tr("Component Controllers"),toolBox_);

}

void ComponentControllersPlugin::pluginsInitialized(){
}

void ComponentControllersPlugin::onClick_buttonSegmentColor(){
    log(QString("onClick_buttonSegmentColor"));
}

void ComponentControllersPlugin::onClick_buttonSegmentHighlight(){
    log(QString("onClick_buttonSegmentHighlight"));
}
void ComponentControllersPlugin::onClick_buttonSegmentAdd(){
    log(QString("onClick_buttonSegmentAdd"));
}
void ComponentControllersPlugin::onClick_buttonSegmentClear(){
    log(QString("onClick_buttonSegmentClear"));
}
void ComponentControllersPlugin::onClick_buttonSegmentRemove(){
    log(QString("onClick_buttonSegmentRemove"));
}

void ComponentControllersPlugin::onClick_buttonVertexAdd(){
    log(QString("onClick_buttonVertexAdd"));
}
void ComponentControllersPlugin::onClick_buttonVertexClear(){
    log(QString("onClick_buttonVertexClear"));
}

void ComponentControllersPlugin::onClick_buttonAdvancedGrow(){
    log(QString("onClick_buttonAdvancedGrow"));
}
void ComponentControllersPlugin::onClick_buttonAdvancedGrowAll(){
    log(QString("onClick_buttonAdvancedGrowAll"));
}


Q_EXPORT_PLUGIN2(componentControllersPlugin, ComponentControllersPlugin);

