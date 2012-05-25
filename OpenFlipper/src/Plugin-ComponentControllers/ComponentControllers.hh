#ifndef COMPONENTCONTROLLERS_HH
#define COMPONENTCONTROLLERS_HH

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <../ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include <../ObjectTypes/PolyMesh/PolyMesh.hh>
#include <../OpenFlipper/common/Types.hh>
#include "OpenFlipper/BasePlugin/PluginFunctions.hh"


class ComponentControllerB {
public:
    typedef OpenMesh::Vec4f Color;
	typedef int SegmentHandle;
	typedef PolyMesh::VertexHandle PVH;
	typedef TriMesh::VertexHandle TVH;
	typedef std::map<SegmentHandle, int> ShMap;

	int meshID_;
	std::string vPropname_;
	/*
	 * 0: bad type
	 * 1: TriMesh
	 * 2: PolyMesh
	 */
	int meshType_;
	std::map<SegmentHandle, int> segmentIndex_;
	std::vector<Color> segmentColor_;
	std::vector<std::string> segmentName_;




	virtual SegmentHandle addSegment();
	virtual bool clearSegment (SegmentHandle _segmentHandle){return false;}
	virtual bool removeSegment(SegmentHandle _segmentHandle){return false;}
	virtual bool removeAllSegments(){return false};

	virtual void clearAll(){};
	virtual bool segmentExists(SegmentHandle _segmentHandle){return false;}

	virtual bool addVertex (SegmentHandle _segmentHandle, PVH _vertexHandle){return false;}
	virtual bool removeVertex (SegmentHandle _segmentHandle, PVH _vertexHandle){return false}

    //PVH is the same as TVH?
	//virtual bool addVertex (SegmentHandle _segmentHandle, TVH _vertexHandle){return false;}
	//virtual bool removeVertex (SegmentHandle _segmentHandle, TVH _vertexHandle){return false;}

//	virtual void getVertexHandle(SegmentHandle _segmentHandle, PVH& _vertexHandle){_vertexHandle = PVH();};
//	virtual void getVertexHandle(SegmentHandle _segmentHandle, TVH& _vertexHandle){_vertexHandle = TVH();};


	virtual bool addSelectedVertices(SegmentHandle _segmentHandle){return false};
	virtual bool removeSelectedSelectedVertices(){return false};

	private:
	ComponentControllerB(){};
	
};


template <typename myMesh> class ComponentController : public ComponentControllerB{
	public:
	typedef typename myMesh::VertexHandle VertexHandle;


	std::vector<std::set<VertexHandle>> segmentVertexHandleSet_;


	ComponentController(int _objectID, bool _fromMesh = false){
		BaseObject baseObject; 
		PluginFunctions::getObject(_objectID,baseObject);
		if(baseObject->dataType(DATA_TRIANGLE_MESH)) meshType_ = 1;
		else if(baseObject->dataType(DATA_POLY_MESH)) meshType_ = 2;
		else meshType_ = 0;
		objectID_  = _objectID;
		myMesh* mesh = getMesh();
		typename myMesh::vertex_iter v_it = mesh->vertices_begin();
		typename myMesh::vertex_iter v_end = mesh->vertices_end();
		if(!mesh->has_vertex_colors()) mesh->request_vertex_colors();

		
		vpropName_ = std::string("vpropSegmentation");

		OpenMesh::VPropHandle<SegmentHandle> vPropHandle;
		//if the property doesn't exist then create it and initialise
		if (!mesh->has_vertex_colors){
			mesh->request_vertex_colors();
			typename myMesh::vertexIter v_it = mesh->vertices_begin();
			typename myMesh::vertexIter v_end = mesh->vertices_end();
			Color colorWhite(0,0,0,1);
			for(; v_it != v_end; ++v_it){
				mesh->property(vPropHandle, v_it) = 0;
				mesh->set_vertex_color(vPropHandle, colorWhite);
			}
		}

		bool noProperty = !propertyExists(mesh, vpropName_);
		if(!_fromMesh || noProperty) {
			if(noProperty) mesh->add_property(vPropHandle, vpropName_);
			typename myMesh::vertexIter v_it = mesh->vertices_begin();
			typename myMesh::vertexIter v_end = mesh->vertices_end();
			Color colorWhite(0,0,0,1);
			for(; v_it != v_end; ++v_it){
				mesh->property(vPropHandle, v_it) = 0;
				mesh->set_vertex_color(vPropHandle, colorWhite);
			}
		}else{
			//else initialise from the mesh property
			mesh_->get_property_handle(vPropHandle, vpropName);
			if(_fromMesh){
				typename myMesh::vertexIter v_it = mesh->vertices_begin();
				typename myMesh::vertexIter v_end = mesh->vertices_end();
				for(; v_it != v_end; ++v_it){
					//if the segment doesn't exist then create it
					SegmentHandle segmentHandle = mesh->property(vPropHandle);
					if (segmentHandle == 0) continue;
					if (!segmentExists(segmentHandle)) {
						addSegment(segmentHandle);
						//initialise the color from the handle
						segmentColor_[segmentIndex[segmentHandle]] = mesh_->color(v_it.handle());
					}
					else{
						//else add the vertex to the segment
						addVertex(segmentHandle,v_it.handle());
					}
					
				}
			}
		}
	}


	 SegmentHandle addSegment(){
		SegmentHandle maxHandle = 0;
		for(; sh_it != sh_end; ++sh_it) maxHandle = maxHandle > sh_it.first ? maxHandle : sh_it.first;
		addSegment(++maxHandle);
		return segmentHandle;
	}

	bool addSegment(SegmentHandle _segmentHandle){
		if(segmentExists(_segmentHandle)) return false;
		ShMap::const_iterator sh_it = segmentIndex_.begin();
		ShMap::const_iterator sh_end = segmentIndex_.end();
		int newIndex = segmentColor_.size();
		segmentIndex_[_segmentHandle] = newIndex;
		segmentColor_.push_back(Color(0,0,0));
		stringstream str;
		str << "segment" << _segmentHandle;
		segmentName_.push_back(str.st());
	}

	bool removeSegment(SegmentHandle _segmentHandle){
		ShMap::iterator sh_it = segmentIndex_.find(_segmentHandle);
		ShMap::iterator sh_end = segmentIndex_.end();
		clearSegment(_segmentHandle);

		int index = (*sh_it).first;
		segmentIndex_.erase(sh_it);
		segmentColor_.erase(segmentColor_.begin() + index);
		segmentName_.erase(segmentName_.begin() + index);
		sh_it = segmentIndex_.begin();
		//decrement all indices after the index in order to maintain parity
		for(; sh_it != sh_end; ++sh_it) if((*sh_it).second > index) (*sh_it).second--;
	}

	bool removeAllSegments(){
		ShMap::const_iterator sh_it = segmentIndex_.begin();
		ShMap::const_iterator sh_end = segmentIndex_.end();
		//clear all internal properties
		for(; sh_it!= sh_end; ++sh_it) clearSegment(sh_it.first);
		
		//clear all containers
		segmentIndex_.clear();
		segmentColor_.clear();
		segmentName_.clear();
		segmentVertexHandleSet_.clear();
	}


	bool addVertex (SegmentHandle _segmentHandle, VertexHandle _vertexHandle){
		int index = segmentIndex_[segmentHandle_];
		myMesh* mesh = getMesh();

		VPropHandle<SegmentHandle> vPropHandle;
		mesh->get_property_handle(vPropHandle, vpropName)
		//check if we have to remove it
		SegmentHandle segmentHandle = mesh->property(vPropHandle);
		if(segmentHandle == _segmentHandle) return true;
		if( segmentHandle != 0 ) removeVertex( segmentHandle, _vertexHandle);

		//insert the new vertex handle
		segmentVertexHandleSet_[index].insert(_vertexHandle);
		//set the color of the vertex handle on the mesh
		mesh->set_color(_vertexHandle, segmentColor_[index]);
		//set the stored property
		mesh->property(vPropHandle) = _segmentHandle;
	}

	bool removeVertex (SegmentHandle _segmentHandle, VertexHandle _vertexHandle){
		int index = segmentIndex_[_segmentHandle];
		myMesh* mesh = getMesh();
		//set the color to default
		mesh->set_color(_vertexHandle, Color(0,0,0,1));
		//remove the vertex from the array
		segmentVertexHandleSet_[index].erase(segmentVertexHandleSet_[index].find(_vertexHandle));
		//remove the mesh proeprty
		VPropHandle<SegmentHandle> vPropHandle;
		mesh->get_property_handle(vPropHandle, vpropName)
		mesh->property(vPropHandle, _vertexHandle) = 0;
	}


	bool clearSegment (SegmentHandle _segmentHandle){
		int index = segmentIndex_[_segmentHandle];

		std::set<VertexHandle>::iterator vhs_it = segmentVertexHandleSet_[_segmentHandle].begin();
		std::set<VertexHandle>::iterator vhs_end = segmentVertexHandleSet[_segmentHandle].end();

		VPropHandle<SegmentHandle> vPropHandle;
		mesh->get_property_handle(vPropHandle, vpropName);

		//for each vertex
		for(; vhs_it != vhs_end; ++vhs_it){
			//clear the vertex color
			mesh->set_color(_vertexHandle, Color(0,0,0,1));
			//vlear the vertex property
			mesh->property(vPropHandle, vhs_it.first) = 0;
		}
		//clear the handle set
		segmentVertexHandleSet_[index].clear();
	}

	void clearAll(){
		ShMap::const_iterator sh_it = segmentIndex_.begin();
		SgMap::const_iterator sh_end = segmentIndex_.end();
		for(;sh_it!=sh_end;++sh_it) clearSegment(sh_it.first);
	}


	myMesh* getMesh(){
		myMesh* mesh;
		BaseObject* baseObject;
		PluginFunctions::getObject(meshID_, baseObject);
		mesh = dynamic_cast<myMesh*>(baseObject);
		return mesh;
	}

	bool propertyExists(myMesh* _mesh, std::string _propertyName){
		typename myMesh::prop_iterator p_it = mesh->vprops_begin();
		typename myMesh::prop_iterator p_end = mesh->vprops_end();
		for(; p_it != p_end; ++p_it){
			if((*p_it)->name() == _propertyName) return true;
		}
		return false;
	}

	bool addSelectedVertices(SegmentHandle _segmentHandle){return false}{
		myMesh* mesh = getMesh();
		std::vector<VertexHandle> vertexHandles;
		getSelectedVertices(vertexHandles);
		
		std::vector<VertexHandle>::const_iterator vh_it = vertexHandles.begin();
		std::vector<VertexHandle>::const_iterator vh_end = vertexHandles.end();

		for(;vh_it != vh_end;++vh_it) addVertex(_segmentHandle, *vh_it);
	}

	bool removeSelectedVertices(){return false}{
		myMesh* mesh = getMesh();
		std::vector<VertexHandle> vertexHandles;
		getSelectedVertices(vertexHandles);
		
		std::vector<VertexHandle>::const_iterator vh_it = vertexHandles.begin();
		std::vector<VertexHandle>::const_iterator vh_end = vertexHandles.end();

		for(;vh_it != vh_end;++vh_it) removeVertex(*vh_it);
	}

	bool getSelectedVertices(std::vector<VertexHandle>& _vertexHandles){
		myMesh* mesh = getMesh();
		typename myMesh::vertexIter v_it = mesh->vertices_begin();
		typename myMesh::vertexIter v_end = mesh->vertices_end();
		_vertexHandles.clear();
		for(;v_it!=v_end; ++v_it) if(mesh->status(v_it.handle()).selected()) _vertexHandles.push_back(v_it.handle());
	}
		


	bool segmentExists(SegmentHandle _segmentHandle) {return !(segmentIndex_.find(_segmentHandle) == segmentIndex_.end());}

};

template class ComponentController<TriMesh>;
template class ComponentController<PolyMesh>;

#endif
