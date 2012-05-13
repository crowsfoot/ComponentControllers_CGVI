#ifndef SEGMENTDESCRIPTOR_HH
#define SEGMENTDESCRIPTOR_HH

#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
template <typename myMesh> class SegmentDescriptor{
public:
	//typedefs, inherited from myMesh
	typedef typename myMesh::Point Point;
    //data access
    const Point minPoint() const {return minPoint_;}
    const Point maxPoint() const {return maxPoint_;}
    const int objID() const {return objID_;}

    void set_minPoint(Point _p){minPoint_ = _p;};
    void set_maxPoint(Point _p){maxPoint_ = _p;};
    void set_objID(int _id){objID_ = _id;}

    //data functions
    //Matrix4x4 worldTransform();

protected:
	Point minPoint_;
    Point maxPoint_;
	
    int objID_;
};

template class SegmentDescriptor<TriMesh>;
template class SegmentDescriptor<PolyMesh>;

#endif
