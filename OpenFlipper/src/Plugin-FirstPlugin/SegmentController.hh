/*
 * =====================================================================================
 *
 *  Filename:  SegmentController.hh
 *
 *	Description: An initial attempt at a segment controller class
 *					Contains test classes for OpenFlipper integrated segmentation and
 *					the algLib linear algebra library
 *					***Included for reference only***
 *					see the family of Segmenter2 files.
 *
 *
 *   Author:  David Lane
 *
 * =====================================================================================
 */

#include <vector>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include "alglib/linalg.h"


 class CBase{
public:
	virtual bool isTrimesh() = 0;
	virtual bool isPolyMesh() = 0;
};

template <class myMesh = TriMesh> class SController: public CBase{
  public:
  myMesh* _mesh;
  std::vector<typename myMesh::VertexHandle> _VertexHandles;
  bool isTrimesh(){return _mesh->is_polymesh();}
  bool isPolyMesh(){return _mesh->is_trimesh();}
  TriMeshObject* _BoundingCubeObject;
  TriMesh* _BoundingCube;
  int _BoundingCubeID;
 

  class TriMesh_AddFace{
  public:
	  TriMesh* mesh_;
  	 std::vector<typename TriMesh::VertexHandle>* handles_;
	  std::vector<TriMesh::VertexHandle> faceHandles;

	  TriMesh_AddFace(TriMesh* _mesh, std::vector<typename TriMesh::VertexHandle>* _handles){
		  faceHandles.reserve(4);
		  mesh_ = _mesh;
		  handles_ = _handles;
	  }
	  void addFace(int v1, int v2, int v3){
		  faceHandles.clear();
		  faceHandles.push_back((*handles_)[v1]);
		  faceHandles.push_back((*handles_)[v2]);
		  faceHandles.push_back((*handles_)[v3]);
		  mesh_->add_face(faceHandles);
	  }
  };


  //a test function for alglib integration.
	std::string testAlglib();
	void BuildBoundingBox( typename myMesh::Point &minPt, typename myMesh::Point &maxPt);
	
	void unSelectAll(){
		for(int i = 0; i < _VertexHandles.size();i++){		
			_mesh->status(_VertexHandles[i]).set_selected(false);
		}
	}
};



template class SController<TriMesh>;
template class SController<PolyMesh>;

template <typename myMesh> void SController<myMesh>::BuildBoundingBox( typename myMesh::Point &minPt, typename myMesh::Point &maxPt){

	_BoundingCube->clear();

	minPt = _mesh->point(_VertexHandles[0]);
	maxPt = minPt;
	
	myMesh::Point a;
	for(int i  = 1; i < _VertexHandles.size(); i++){
		a = _mesh->point(_VertexHandles[i]);
		minPt[0] = minPt[0] < a[0] ? minPt[0] : a[0];
		minPt[1] = minPt[1] < a[1] ? minPt[1] : a[1];
		minPt[2] = minPt[2] < a[2] ? minPt[2] : a[2];
		maxPt[0] = maxPt[0] > a[0] ? maxPt[0] : a[0];
		maxPt[1] = maxPt[1] > a[1] ? maxPt[1] : a[1];
		maxPt[2] = maxPt[2] > a[2] ? maxPt[2] : a[2];
	}

	std::vectorcubeHandles

	TriMesh_AddFace faceMaker(_BoundingCube, &_VertexHandles);
	faceMaker.addFace(0,4,2);
	faceMaker.addFace(0,1,4);

	faceMaker.addFace(3,7,5);
	faceMaker.addFace(6,7,3);

	faceMaker.addFace(4,1,7);
	faceMaker.addFace(1,5,7);
	
	faceMaker.addFace(0,6,2);
	faceMaker.addFace(0,3,6);

	faceMaker.addFace(2,4,7);
	faceMaker.addFace(2,7,6);

	faceMaker.addFace(0,3,5);
	faceMaker.addFace(0,5,1);
	
	_BoundingCube->update_normals();

	
}







template <typename myMesh> std::string SController<myMesh>::testAlglib(){
	alglib::real_2d_array a("[[1,2],[11,22]]");
	alglib::real_2d_array b;
	b.setlength(2,2);
	alglib::rmatrixtranspose(2,2,a,0,0,b,0,0);	  
	return b.tostring(1);
  }
