#ifndef SEGMENTDESCRIPTOR_HH
#define SEGMENTDESCRIPTOR_HH

#include <set>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include "SegmentTypes.hh"
#include "alglib/linalg.cpp"

template <typename myMesh> class SegmentDescriptorT: public SegmentTypesT<myMesh>{
    typedef typename myMesh::Point Point;
public:
	//constructors
	SegmentDescriptorT();
	SegmentDescriptorT(const Point& _minPoint, const Point& _maxPoint);
	SegmentDescriptorT(myMesh* _mesh, const VertexHandleSet& _vhs);

    
	//typedefs, inherited from myMesh
	typedef typename myMesh::Point Point;

    //data access
    //note:: Point is default defined as Vec3d in PolyMeshTypes.hh
    const Point minPoint() const {return minPoint_;}
    const Point maxPoint() const {return maxPoint_;}
    const int objID() const {return objID_;}

    void set_minPoint(Point _p){minPoint_ = _p;};
    void set_maxPoint(Point _p){maxPoint_ = _p;};
    void set_objID(int _id){objID_ = _id;}

    //data functions
    alglib::real_2d_array worldTransform();
    void generate(myMesh* _mesh, const VertexHandleSet& _vhs);

protected:
	Point minPoint_;
    Point maxPoint_;
	
    int objID_;
};




#endif
