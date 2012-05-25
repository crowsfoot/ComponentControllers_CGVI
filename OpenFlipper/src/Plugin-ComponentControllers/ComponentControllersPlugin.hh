#ifndef COMPONENTCONTROLLERSPLUGIN_HH_INCLUDED
#define COMPONENTCONTROLLERSPLUGIN_HH_INCLUDED

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/PluginConnectionInterface.hh>
#include <OpenFlipper/common/Types.hh>


//#include "ComponentControllers.hh"

class ComponentControllersPlugin : public QObject, BaseInterface, ToolboxInterface, LoggingInterface
{

Q_OBJECT
Q_INTERFACES(BaseInterface)
Q_INTERFACES(LoggingInterface)
Q_INTERFACES(ToolboxInterface)
//Q_INTERFACES(PluginConnectionInterface)
//Q_INTERFACES(LoadSaveInterface)

signals:
  //---interface signals---
  
//baseInterface
  void updateView();
  void updatedObject(int _identifier, const UpdateType& _type);

  //logginginterface
  void log(Logtype _type, QString _message);
  void log(QString _message);

  //toolboxinterface
  void addToolbox(QString _name,  QWidget* _widget);

  //LoadSaveInterface
  //void addEmptyObject(DataType _type, int& _id);

  //void doClearTargets();
  //void crossPluginConnect(QString _pluginName1, const char* _signal, QString _pluginName2, const char* _slot);

  //---in-code signals---
  //pluginPrimitiveGenerator
  
  //int signalCreateCube();
  //int signalDeleteObject(ObjectHandle);
  //int signalTransformObject(ObjectHandle _h, Matrix4x4 _matrix);



public:
  //---initialisers---
  
  ComponentControllersPlugin();
  ~ComponentControllersPlugin();

  //---public properties---
  
  QString name() { return QString("ComponentControllers"); };
  
  QString description() { return QString("CGVI_component controllers plugin module"); };

private:
  QLabel* labelObjects_;
  QLabel* labelSegments_;
  QLabel* labelSegmentOperations_;
  QLabel* labelVertexOperations_;
  QLabel* labelAdvanced_;

  QComboBox* comboObjects_;
  QComboBox* comboSegments_;

  QPushButton* buttonRefreshObjects_;

  QPushButton* buttonSegmentColor_;
  QPushButton* buttonSegmentHighlight_;
  QPushButton* buttonSegmentAdd_;
  QPushButton* buttonSegmentClear_;
  QPushButton* buttonSegmentRemove_;

  QPushButton* buttonVertexAdd_;
  QPushButton* buttonVertexClear_;


  QPushButton* buttonAdvancedGrow_;
  QPushButton* buttonAdvancedGrowAll_;

  //QList<ComponentControllerB*> listCompContB;

  QWidget* toolBox_;

  QGridLayout* layout_;

 private slots:
  //baseInterface
  void initializePlugin();
  void pluginsInitialized();

  //GUI slots
	void onClick_buttonSegmentColor();
	void onClick_buttonSegmentHighlight();
	void onClick_buttonSegmentAdd();
	void onClick_buttonSegmentClear();
	void onClick_buttonSegmentRemove();

	void onClick_buttonVertexAdd();
	void onClick_buttonVertexClear();

	void onClick_buttonAdvancedGrow();
	void onClick_buttonAdvancedGrowAll();

public slots:
	//openFlipper boilerplate
  QString version() {return QString("0.1"); };
};
#endif