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

#ifndef SEGMENTER2_HH
#define SEGMENTER2_HH

#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "TypeCompare.hh"
#include "SegmentDescriptor.hh"
#include "SegmentTypes.hh"

template <typename myMesh> class MeshSegmentT;
template <typename myMesh> class MeshSegmentCollectionT;

///base class to support segment arrays
class MeshSegmentBase{
public:
    virtual bool isTriMesh(){return false;};
    virtual bool isPolyMesh(){return false;};
};

template <typename myMesh> class MeshSegmentT : public MeshSegmentBase, public SegmentTypesT<myMesh>{
public:

    //*******************type defines*******************
    typedef  MeshSegmentT<myMesh> myType ; //self type
    typedef MeshSegmentCollectionT<myMesh> SegmentCollection ; //Container/manager for this class
    typedef typename myMesh::VertexIter VertexIter;

    //*****************Constructors/Destructors******************************
    MeshSegmentT(myMesh* _mesh, SegmentHandle _segmentHandle);
    MeshSegmentT(myMesh* _mesh, SegmentHandle _segmentHandle, std::string _name);

    ~MeshSegmentT();///sets the segment property of each vertex to zero/unselected

    //******************Data Access functions*************************
    SegmentHandle handle(){ return segmentHandle_; }

    void add(VertexHandle _v);
    bool remove(VertexHandle _v);

    typename VertexHandleSet::const_iterator begin(){ return vertexHandleSet_.begin();}
    typename VertexHandleSet::const_iterator end(){ return vertexHandleSet_.end();}

    bool isPolyMesh(){return isXMesh<myMesh,PolyMesh>();}
    bool isTriMesh(){return isXMesh<myMesh,TriMesh>();}

    std::string name(){return  name_;}
    void set_name(std::string _name){name_=_name;}
    static std::string defaultVertexPropName(){ return std::string("vpropSegmentation"); };
    myMesh* mesh()  {return mesh_;}
    const VertexHandleSet& vertexHandleSet()  {return vertexHandleSet_;}
protected:

    //************** Data Members*************************
    SegmentHandle segmentHandle_;
    std::string name_;
    VertexHandleSet vertexHandleSet_;
    myMesh* mesh_;
    OpenMesh::VPropHandleT<SegmentHandle> vertexPropHandle_;
    SegmentDescriptorT<myMesh> segmentDescriptor_;

    //**************protected functions*******************

    //comparisons for class types
    //return true the types are supported, similar mesh types
    template <typename obj1, typename obj2> static bool isXMesh(){ return false; }
    template <> static bool isXMesh<TriMesh, TriMesh>(){ return true; }
    template <> static bool isXMesh<PolyMesh,PolyMesh>(){ return true; }


    //********************access declerations*********************
    friend class MeshSegmentCollectionT<myMesh>;
    friend struct OpenMesh::IO::binary<myType>;
    friend struct OpenMesh::IO::binary<SegmentCollection>;
};


///base class to support segment arrays
class MeshSegmentCollectionBase{
public:
    virtual bool isTriMesh(){return false;};
    virtual bool isPolyMesh(){return false;};
};

template <typename myMesh> class MeshSegmentCollectionT : public  MeshSegmentCollectionBase, public SegmentTypesT<myMesh>{

public:
    static std::string defaultName(){ return std::string("Segmentation"); }

    typedef MeshSegmentCollectionT<myMesh> myType; //self type
    //segment level types
    typedef typename MeshSegmentT<myMesh> Segment;
    typedef typename std::map<SegmentHandle, Segment*> SegmentPointerContainer; //TODO: change implementation to use a map


    typedef typename SegmentPointerContainer::const_iterator const_iterator;

    //*****************template base overrides******************************
    bool isTriMesh(){ 
        TriMesh* mesh=0;
        return TypeCompareT<myMesh>::isType(mesh);
    }
    bool isPolyMesh(){ 
        PolyMesh* mesh=0;
        return TypeCompareT<myMesh>::isType(mesh);
    }


    //*****************constructor/destructor*******************************
    ~MeshSegmentCollectionT();
    MeshSegmentCollectionT(){ maxHandle_ = 1; }//handle 0 means unassigned

    //*****************Data manipulators***********************************

    bool setVertexSelection(SegmentHandle _h, bool _state);
    void setVertexSelection(myMesh* _mesh, bool _state, VertexHandleSet* _vhc = 0);
    void harvestSelectedVertices( myMesh* _mesh, VertexHandleSet& _vhc);
    void assignVertices(const VertexHandleSet& _VH, SegmentHandle _h, myMesh* _mesh);


    Segment* add(myMesh* _mesh);
    bool remove( Segment* _s);
    bool  remove( SegmentHandle _h);
     Segment* const get  (SegmentHandle _h);
     Segment* const operator[] (SegmentHandle _h){return get(_h);};


    typename SegmentPointerContainer::const_iterator begin(){ return segmentPointerContainer_.begin();}
    typename SegmentPointerContainer::const_iterator end(){ return segmentPointerContainer_.end();}
protected:
    //track the current larges handle per mesh
    //assume we'll never need more than unsigned int
    //unsigned int getSegmentIndex(SegmentHandle _h); //linear search for segment handle

    std::string name_;
    SegmentHandle maxHandle_; 
    SegmentPointerContainer segmentPointerContainer_;
    friend struct OpenMesh::IO::binary<myType>;
    
};




//usability typedefs
typedef MeshSegmentT<PolyMesh> PolyMeshSegment;
typedef MeshSegmentT<TriMesh> TriMeshSegment;
typedef MeshSegmentCollectionT<PolyMesh> PolyMeshSegmentCollection;
typedef MeshSegmentCollectionT<TriMesh> TriMeshSegmentCollection;



#endif
