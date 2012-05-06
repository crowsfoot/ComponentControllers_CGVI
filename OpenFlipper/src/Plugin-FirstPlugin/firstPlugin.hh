#ifndef FIRSTPLUGIN_HH_INCLUDED
#define FIRSTPLUGIN_HH_INCLUDED

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/PluginConnectionInterface.hh>
#include <OpenFlipper/common/Types.hh>
//#include <Plugin-FirstPlugin/SegmentController.hh>
#include "Segmenter_toplevel.hh"

class FirstPlugin : public QObject, BaseInterface, ToolboxInterface, LoggingInterface, PluginConnectionInterface, LoadSaveInterface
{
Q_OBJECT
Q_INTERFACES(BaseInterface)
Q_INTERFACES(LoggingInterface)
Q_INTERFACES(ToolboxInterface)
Q_INTERFACES(PluginConnectionInterface)
Q_INTERFACES(LoadSaveInterface)

signals:
//baseInterface
  void updateView();
  void updatedObject(int _identifier, const UpdateType& _type);

  //logginginterface
  void log(Logtype _type, QString _message);
  void log(QString _message);

  //toolboxinterface
  void addToolbox(QString _name,  QWidget* _widget);

  //LoadSaveInterface
  void addEmptyObject(DataType _type, int& _id);

  void doClearTargets();
  void crossPluginConnect(QString _pluginName1, const char* _signal, QString _pluginName2, const char* _slot);


public :
  
  FirstPlugin();
  ~FirstPlugin() {};
  
  QString name() { return QString("FirstPlugin"); };
  
  QString description() { return QString("Does actually nothing but works!"); };

private:
  QLabel* labelStaticText_;
  QLabel* labelDynamicText_;
  QPushButton* buttonCountTargetObjects_;
  QPushButton* buttonCountTargetVertices_;
  QPushButton* buttonClearSelection_;
  QWidget* toolBox_;
  QGridLayout* layout_;

  QPushButton* buttonTestSegmentController_;

  QComboBox* comboSegments_;
  QPushButton* buttonSegmentAdd_;
  QPushButton* buttonSegmentDelete_;
  QPushButton* buttonSegmentAddVertices_;
  QPushButton* buttonSegmentRemoveVertices_;


  //private functions
  int addTriMesh();
private slots:
  //baseInterface
  void initializePlugin();
  void pluginsInitialized();

  //button clicks
  void CountTargetObjects();
  void CountTargetVertices();
  void ClearTargetsClicked();

  void onClick_buttonTestSegmentController();
  void onClick_buttonSegmentAdd();
  void onClick_buttonSegmentDelete();
  void onClick_buttonSegmentAddVertices();
  void onClick_buttonSegmentRemoveVertices();

public slots:
	//openFlipper boilerplate
  QString version() {return QString("1.0"); };


};

//functor classes used in one of the test buttons built into the plugin

class fNC{
  protected:
  long* count_;
  long* countTotal_;
  public:
  fNC(long& count, long& countTotal){
    count_ = &count;
    countTotal_ = &countTotal;
  }
};  

template <typename myMesh> class functorNodeCount : fNC{
  private:
    myMesh* mesh_;
  public: 
      functorNodeCount(myMesh* mesh,long& count, long& countTotal ): fNC(count, countTotal) { mesh_ = mesh;};
      void operator()(const typename myMesh::VertexHandle& v_it){ 
        (*countTotal_)++;
        if(mesh_->status(v_it).selected()) (*count_)++;
      }
};


#endif
