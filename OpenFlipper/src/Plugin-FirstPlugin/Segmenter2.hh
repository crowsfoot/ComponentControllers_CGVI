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

#include <vector>
#include <string>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>


template <typename myMesh> class MeshSegmentT;
template <typename myMesh> class MeshSegmentCollectionT;

///base class to support segment arrays
class MeshSegmentBase{
public:
    virtual static bool isTriMesh();
    virtual static bool isPolyMesh();
}

template <typename myMesh> class MeshSegmentT : MeshSegmentBase{
public:

    //*******************type defines*******************
    std::string defaultVertexPropHandle{ return std::string("vpropSegmentation"));
    typedef typename myMesh::VertexHandle VertexHandle; //vertex handle
    typedef std::set<VertexHandle> VertexHandleContainer; //vertex handle container (see class definiton)

    typedef unsigned int SegmentHandle; //handle/identifier
    typedef  MeshSegmentT<myMesh> myType ; //type of mesh
    typedef MeshSegmentCollectionT<myMesh> SegmentCollection ; //Container/manager for this class

    //*****************Check ClassType**********************
    
    //*****************Access Calls***********************
    MeshSegmentT(myMesh MESH, SegmentHandle _segmentHandle){
        //if the vertex segmentation property doesn't exist:
        mesh_ = MESH;
        segmentHandle_ = _segmentHandle;
        if(!MESH->get_property_handle(vertexPropHandle, defaultVertexPropHandle()){
            MESH->add_property(vertexPropHandle, defaultVertexPropHandle());
            MESH::VertexIter v_it = MESH.vertices_begin(), v_end = MESH.vertices_end();
            for(;v_it != v_end;++v_it){ //initialise the vertex segments to 0
                MESH->property(vertexPropHandle,v_it)=0;
            }
        }
    }

    ///sets the segment property of each vertex to zero/unselected
    ~MeshSegmentT(){
        VertexHandleContainer::const_iterator vv_it = vertexHandles.begin(), vv_end = vertexHandles.end();
            for(;vv_it != vv_end;++vv_it){ //initialise the vertex segments to 0
                MESH->property(vertexPropHandle,*vv_it)=0;
            }
    }



    const SegmentHandle getHandle(){
        return segmentHandle_;
    }

    void add_vertex(VertexHandle _v){
        myMesh->property(prop_handle, *v_it) = segmentHandle_;
        vertexHandles_.insert(_v);
    }
    bool remove_vertex(VertexHandle _v){
        VertexHandleContainer::const_iterator v_it = vertexHandles_.find(_v);
        if(v_it != vertexHandles_.end()) {
            myMesh->property(prop_handle,*v_it) = 0;
            vertexHandles_.erase(v_it);
            return true;
        }
        return false;
    }

    VertexHandleContainer::const_iterator begin(){ return vertexHandles_.begin();}
    VertexHandleContainer::const_iterator end(){ return vertexHandles_.end();}

    static bool isPolyMesh(){return isXMesh<myMesh,PolyMesh>();}
    static bool isTriMesh(){return isXMesh<myMesh,TriMesh>();}

protected:
    //************** Data Members*************************
    SegmentHandle segmentHandle_;
    std::string name_;
    VertexHandleContainer vertexHandles_;
    myMesh* mesh_;
    myMesh::VPropHandle<SegmentHandle> vertexPropHandle;

    //**************protected functions*******************

    //comparisons for class types
    //return true the types are supported, similar mesh types
    template <typename obj1, typename obj2> static bool isXMesh{
           return false;
    }
    template <> static bool isXMesh<TriMesh, TriMesh>{
        return true;
    }
    template <> static bool isXMesh<PolyMesh,Polymesh>{
        return true;
    }

    
    //********************access declerations*********************
    friend class MeshSegmentCollectionT<myMesh>;
    friend struct OpenMesh::IO::binary<myType>;
    friend struct OpenMesh::IO::binary<SegmentCollection>;
};


///base class to support segment arrays
class MeshSegmentCollectionBase{
public:
    virtual static bool isTriMesh();
    virtual static bool isPolyMesh();
}

template <typename myMesh> class MeshSegmentCollectionT : MeshSegmentCollectionBase {

public:
    static std::string defaultName(){ return std::string("Segmentation"); }

    typedef MeshSegmentCollectionT<myMesh> myType;
    //segment level types
    typedef typename MeshSegmentT<myMesh> Segment;
    typedef typename Segment::SegmentHandle SegmentHandle;
    typedef typename std::vector<Segment*> SegmentVector; //TODO: change implementation to use a map

    //vertex level types
    typedef typename myMesh::VertexHandle VertexHandle;
    typedef typename Segment::VertexHandleContainer VertexHandleContainer;


    //*****************template base overrides******************************
    static bool isTriMesh{ return Segment::isTriMesh();}
    static bool isPolyMesh{ return Segment::isPolyMesh();}

    //remove the segments from the stack
    ~MeshSegmentCollectionT(){
        SegmentVector::const_iterator s_it = segments_.begin();
        for(;s_it != segments_.end(); ++s_it){
            delete (*s_it);
        }
    }

    MeshSegmentCollectionT(){
        maxHandle_ = 1; //handle 0 means unassigned
    }

    //SegmentHandle AddSegment( Segment* _s){
    //    _s->segmentHandle_ = maxHandle_++;
    //    segments_.push_back(_s);
    //    return _s->segmentHandle_;
    //}

    Segment* AddSegment(){
        Segment* segment = new Segment();
        segment->segmentHandle_ = maxHandle_++;
        segments_.push_back(segment);
        return segment;
    }


    void RemoveSegment( Segment* _s){
        unsigned int i = getSegmentIndex( _s->getHandle());
        delete segments_[i];
        segments_.erase(segments_.begin() + i);
    }

    void RemoveSegment( unsigned int index){
        segments_.erase(segments_.begin() + index);
    }


    
    const Segment* getSegment(SegmentHandle _h){
        unsigned int i = getSegmentIndex(_h);
        return segments_[i];
    }

protected:
    //track the current larges handle per mesh
    //assume we'll never need more than unsigned int
    unsigned int getSegmentIndex(SegmentHandle _h){ //linear search for segment handle
        SegmentVector::const_iterator s_it = segments_.begin();
        int i = 0;
        for(; s_it != segments_.end(); ++s_it){
            if ((*s_it)->segmentHandle_ = _h) return i;
            i++;
        }
        return 0;
    }

    SegmentHandle maxHandle_; 
    SegmentVector segments_;
    friend struct OpenMesh::IO::binary<myType>;
};



//template predeclares
template class MeshSegmentT<TriMesh>;
template class MeshSegmentT<PolyMesh>;
template class MeshSegmentCollectionT<MeshSegmentT<TriMesh>>;
template class MeshSegmentCollectionT<MeshSegmentT<PolyMesh>>;

//usability typedefs
typedef MeshSegmentT<PolyMesh> PolyMeshSegment;
typedef MeshSegmentT<TriMesh> TriMeshSegment;
typedef MeshSegmentCollectionT<typename PolyMeshSegment> PolyMeshSegmentCollection;
typedef MeshSegmentCollectionT<typename TriMeshSegment> TriMeshSegmentCollection;

#endif
