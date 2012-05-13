#ifndef SEGMENTTYPES_HH
#define SEGMENTTYPES_HH


class SegmentTypesBase{
	public:
    typedef unsigned int SegmentHandle; //handle/identifier
};
template <typename myMesh> class SegmentTypesT : public SegmentTypesBase{
	public:
    typedef typename myMesh::VertexHandle VertexHandle; //vertex handle
    typedef std::set<VertexHandle> VertexHandleSet; //vertex handle container (see class definiton)
    typedef myMesh meshType; //underlying mesh class type
    typedef SegmentTypesT<myMesh> myType; //self type
};

template class SegmentTypesT<PolyMesh>;
template class SegmentTypesT<TriMesh>;

typedef SegmentTypesT<TriMesh> TriMeshSegmentTypes;
typedef SegmentTypesT<PolyMesh> PolyMeshSegmentTypes;

#endif
