
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

#ifndef MESHSEGMENTT_HH
#define MESHSEGMENTT_HH

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
    void set_SegmentDescriptor(SegmentDescriptor* _sd){segmentDescriptor_ = _sd;}
    void set_color(const Color& color);
    static std::string defaultVertexPropName(){ return std::string("vpropSegmentation"); }

    myMesh* mesh()  {return mesh_;}
    Color color(){return color_;};
    const VertexHandleSet& vertexHandleSet()  {return vertexHandleSet_;}
    SegmentDescriptor* segmentDescriptor(){return segmentDescriptor_;}
    //*****************Data Maintenance*******************************
    void regenerateDescriptor();
    /*! Destroys current segmentation and loads a NEW segmentation saved to a mesh
     * if no previous segmentation found function exits before destroying old data
     *  TODO::provide re-implementation which does no destroy previously loaded segmentations
     */

    
protected:

    //************** Data Members*************************
    SegmentHandle segmentHandle_;
    std::string name_;
    VertexHandleSet vertexHandleSet_;
    myMesh* mesh_;
    OpenMesh::VPropHandleT<SegmentHandle> vertexPropHandle_;
    SegmentDescriptorT<myMesh>* segmentDescriptor_;
    typename myMesh::Color color_;

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


//usability typedefs
typedef MeshSegmentT<PolyMesh> PolyMeshSegment;
typedef MeshSegmentT<TriMesh> TriMeshSegment;



#endif
