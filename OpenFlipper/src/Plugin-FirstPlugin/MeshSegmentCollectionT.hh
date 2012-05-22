/*
* =====================================================================================
*
*  Filename:  Segmenter2.hh
*
*	Description: Contains:
*					MeshSegmentT<>: Template class describing an individual segment with name and ID handle.
*					MeshSegmentCollectionT<>: per model container for collections of MeshSegmentT<>. Assigns ID numbers
*
*
*   Author:  David Lane
*
* =====================================================================================
*/

#ifndef MESHSEGMENTCOLLECTIONT_HH
#define MESHSEGMENTCOLLECTIONT_HH

#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "MeshSegmentT.hh"
#include "TypeCompare.hh"
#include "SegmentDescriptor.hh"
#include "SegmentTypes.hh"

template <typename myMesh> class MeshSegmentCollectionT;


///base class to support segment arrays
class MeshSegmentCollectionBase{
public:
    virtual bool isTriMesh(){return false;};
    virtual bool isPolyMesh(){return false;};
};

template <typename myMesh> class MeshSegmentCollectionT : public  MeshSegmentCollectionBase, public SegmentTypesT<myMesh>{

public:

    typedef MeshSegmentCollectionT<myMesh> myType; //self type

    //segment types
    typedef typename MeshSegmentT<myMesh> Segment;
    typedef typename std::map<SegmentHandle, Segment*> SegmentPointerContainer; //TODO: change implementation to use a map
    typedef typename SegmentPointerContainer::const_iterator const_iterator;

    //*****************template base overrides******************************
    ///return true if class is derived from a TriMesh
    bool isTriMesh(){ TriMesh* mesh=0; return TypeCompareT<myMesh>::isType(mesh); }
    ///return true if class is derived from a PolyMesh
    bool isPolyMesh(){ PolyMesh* mesh=0; return TypeCompareT<myMesh>::isType(mesh); }


    //*****************constructor/destructor*******************************
    ~MeshSegmentCollectionT();
    MeshSegmentCollectionT(){ maxHandle_ = 0; }//handle 0 means unassigned

    //*****************Data manipulators***********************************

    bool setVertexSelection(SegmentHandle _h, bool _state);
    void setVertexSelection(myMesh* _mesh, bool _state, VertexHandleSet* _vhc = 0);
    void harvestSelectedVertices( myMesh* _mesh, VertexHandleSet& _vhc);
    void assignVertices(const VertexHandleSet& _VH, SegmentHandle _h, myMesh* _mesh);
    bool test_SegmentInitialiseFromMesh(myMesh* _mesh);


    Segment* add(myMesh* _mesh, Color _colorRGBA);
    bool remove( Segment* _s);
    bool  remove( SegmentHandle _h);

    ///returns Segment* or zero if it does not exist.
    Segment* const get  (SegmentHandle _h);
    ///returns Segment* or zero if it does not exist.
    Segment* const operator[] (SegmentHandle _h){return get(_h);};


    typename SegmentPointerContainer::const_iterator begin(){ return segmentPointerContainer_.begin();}
    typename SegmentPointerContainer::const_iterator end(){ return segmentPointerContainer_.end();}

    ///return the current default string prefix to segment names
    static std::string defaultName(){ return std::string("Segmentation"); }

protected:


    std::string name_;
    SegmentHandle maxHandle_; 
    SegmentPointerContainer segmentPointerContainer_;
    friend struct OpenMesh::IO::binary<myType>;


};




//usability typedefs
typedef MeshSegmentCollectionT<PolyMesh> PolyMeshSegment;
typedef MeshSegmentCollectionT<TriMesh> TriMeshSegmentCollection;



#endif
