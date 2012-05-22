#ifndef SEGMENTTYPES_HH
#define SEGMENTTYPES_HH

template <typename myMesh> class MeshSegmentT;
template <typename myMesh> class MeshSegmentCollectionT;
template <typename myMesh> class SegmentDescriptorT;

class SegmentTypesBase{
	public:
    typedef int SegmentHandle; //handle/identifier
    typedef int ObjectHandle;
};
template <typename myMesh> class SegmentTypesT : public SegmentTypesBase{
	public:
    typedef typename myMesh::VertexHandle VertexHandle; //vertex handle
    typedef std::set<VertexHandle> VertexHandleSet; //vertex handle container (see class definiton)
    typedef myMesh meshType; //underlying mesh class type
    typedef SegmentTypesT<myMesh> SegmentTypes; //self type
    typedef SegmentDescriptorT<myMesh> SegmentDescriptor;
    typedef typename myMesh::Color Color;
};

template class SegmentTypesT<PolyMesh>;
template class SegmentTypesT<TriMesh>;

typedef SegmentTypesT<TriMesh> TriMeshSegmentTypes;
typedef SegmentTypesT<PolyMesh> PolyMeshSegmentTypes;

#endif
