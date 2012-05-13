#include "SegmentDescriptor.hh"

///empty constructor, initialise to the unit cube  between points (0,0) and (1,1)
template <typename myMesh> SegmentDescriptorT<myMesh>::SegmentDescriptorT(){
	minPoint_.vectorize(0.0);
	maxPoint_.vectorize(0.0);
}

///simple constructor, set bounding box
template <typename myMesh> SegmentDescriptorT<myMesh>::SegmentDescriptorT(const Point& _minPoint, const Point& _maxPoint){
	minPoint_ = _minPoint;
	maxPoint_ = _maxPoint;
}

template <typename myMesh> SegmentDescriptorT<myMesh>::SegmentDescriptorT(myMesh* _mesh, const VertexHandleSet& _vhs){
	generate(_mesh,_vhs);
}


///generate the description
///current implementation is bounding box
template <typename myMesh> void SegmentDescriptorT<myMesh>::generate(myMesh* _mesh, const VertexHandleSet& _vhs){
	if(_vhs.size() == 0) {
		minPoint_.vectorize(0.0);
		maxPoint_.vectorize(0.0);
	}
	else{
		VertexHandleSet::const_iterator vh_it = _vhs.begin(), vh_end = _vhs.end();
		minPoint_ = _mesh->point(*vh_it);
		maxPoint_ = minPoint_;
		//generate a bounding box
		for(; vh_it != vh_end; ++vh_it){
			Point crntPoint = _mesh->point(*vh_it);
			maxPoint_ = maxPoint_.max(crntPoint);
			minPoint_ = minPoint_.min(crntPoint);
		}
	}
}

///return a world transform which assumes a base frame of between (1,1,1) and (-1,-1,-1)
///(the frame used by OpenFlipper's InsertCube plugin function
template <typename myMesh> alglib::real_2d_array SegmentDescriptorT<myMesh>::worldTransform(){
}

///set the matrix size to 4,4 and fill it with the identity
template <typename myMesh> void Identity4d(alglib::real_2d_array& _mat){
	double I[] = {1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0};
	_mat.setlength(4,4);
	_mat.setcontent(I);
}
			


	

template class SegmentDescriptorT<TriMesh>;
template class SegmentDescriptorT<PolyMesh>;

