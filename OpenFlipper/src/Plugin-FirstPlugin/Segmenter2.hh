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

template <typename myMesh> class MeshSegmentT{
public:
	//typedefs for readability in template code
	typedef typename myMesh::VertexHandle VertexHandle;
	typedef std::vector<VertexHandle> VertexHandleVector;

	typedef unsigned int SegmentHandle;
	typedef  MeshSegmentT<myMesh> myType ;
	typedef MeshSegmentCollectionT<myMesh> SegmentCollection ;

	std::string name_;
	VertexHandleVector vertexHandles_;
	const SegmentHandle getHandle(){
		return segmentHandle_;
	}

protected:
		SegmentHandle segmentHandle_;
		friend class MeshSegmentCollectionT<myMesh>;
		friend struct OpenMesh::IO::binary<myType>;
		friend struct OpenMesh::IO::binary<SegmentCollection>;
};


template <typename myMesh> class MeshSegmentCollectionT{

public:

	typedef MeshSegmentCollectionT<myMesh> myType;
	//segment level types
	typedef typename MeshSegmentT<myMesh> Segment;
	typedef typename Segment::SegmentHandle SegmentHandle;
	typedef typename std::vector<Segment*> SegmentVector;

    //vertex level types
	typedef typename myMesh::VertexHandle VertexHandle;
	typedef typename Segment::VertexHandleVector VertexHandleVector;


	//remove the segments from the stack
	~MeshSegmentCollectionT(){
		SegmentVector::const_iterator s_it = segments_.begin();
		for(;s_it != segments_.end(); ++s_it){
			delete (*s_it);
		}
	}

	MeshSegmentCollectionT(){
		maxHandle_ = 0;
	}

	SegmentHandle AddSegment( Segment* _s){
		_s->segmentHandle_ = maxHandle_++;
		segments_.push_back(_s);
		return _s->segmentHandle_;
	}

	SegmentHandle AddSegment(){
		Segment* segment = new Segment();
		segment->segmentHandle_ = maxHandle_++;
		segments_.push_back(segment);
		return segment->segmentHandle_;
	}


	void RemoveSegment( Segment _s){
		unsigned int i = getSegmentIndex( _s.segmentHandle_);
		segments_.erase(segments_.begin() + i);
	}

	void RemoveSegment( unsigned int index){
		segments_.erase(segments_.begin() + index);
	}


	unsigned int getSegmentIndex(SegmentHandle _h){ //linear search for segment handle
		SegmentVector::const_iterator s_it = segments_.begin();
		int i = 0;
		for(; s_it != segments_.end(); ++s_it){
			if ((*s_it)->segmentHandle_ = _h) return i;
			i++;
		}
		return 0;
	}

	const Segment* getSegment(SegmentHandle _h){
		unsigned int i = getSegmentIndex(_h);
		return segments_[i];
	}
	
protected:
	//track the current larges handle per mesh
	//assume we'll never need more than unsigned int
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