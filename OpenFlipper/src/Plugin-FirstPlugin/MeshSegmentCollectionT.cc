#include "MeshSegmentCollectionT.hh"



template <typename myMesh> MeshSegmentCollectionT<myMesh>::~MeshSegmentCollectionT(){
    SegmentPointerContainer::const_iterator s_it = segmentPointerContainer_.begin();
    for(;s_it != segmentPointerContainer_.end(); ++s_it){
        delete (s_it->second);
    }
}


template <typename myMesh> MeshSegmentT<myMesh>* MeshSegmentCollectionT<myMesh>::add(myMesh* _mesh, Color _colorRGBA = Color(1.0,1.0,1.0,1.0)){

	++maxHandle_; //find the next maximum handle
    Segment* segment = new Segment(_mesh, maxHandle_);
    segment->mesh_ = _mesh;
    segmentPointerContainer_[segment->segmentHandle_] = segment;
    segment->set_color(_colorRGBA); //fill the colour array  
    return segment;
}


template <typename myMesh> bool MeshSegmentCollectionT<myMesh>::remove(typename MeshSegmentCollectionT<myMesh>::Segment* _s){
    return remove(_s->handle());
}

template <typename myMesh> bool MeshSegmentCollectionT<myMesh>::remove(typename MeshSegmentCollectionT<myMesh>::SegmentHandle _h){
    SegmentPointerContainer::iterator sp_it;
    sp_it = segmentPointerContainer_.find(_h);
    if (sp_it == segmentPointerContainer_.end()) return false;
    delete sp_it->second;
    segmentPointerContainer_.erase(sp_it);
    return true;
}


///returns Segment* or zero if it does not exist.
template  <typename myMesh> MeshSegmentT<myMesh>* const MeshSegmentCollectionT<myMesh>::get(SegmentHandle _h){
    SegmentPointerContainer::iterator sp_it = segmentPointerContainer_.find(_h);
    if (sp_it == segmentPointerContainer_.end()) return 0;
    return sp_it->second;
}


///set the status()->selected property of all vertex handles passed to it
///if no argument is passed then sets the state of all vertices
template <typename myMesh> void MeshSegmentCollectionT<myMesh>::setVertexSelection(myMesh* _mesh, bool _state, VertexHandleSet* _vhc = 0){
    //int count;
    if(_vhc == 0){
        typename myMesh::VertexIter v_it, v_end;
        v_it = _mesh->vertices_begin();
        v_end = _mesh->vertices_end();
        for(; v_it != v_end;++v_it)
            _mesh->status(v_it.handle()).set_selected(_state);
    }
    else{
        VertexHandleSet::const_iterator vh_it, vh_end;
        vh_it = _vhc->begin();
        vh_it = _vhc->end();
        for(; vh_it != vh_end; ++vh_it)
            _mesh->status(*vh_it).set_selected(_state);
    }

}



///set the selection state for all vertices in the given segment
///if segment is not found then return false
template <typename myMesh> bool MeshSegmentCollectionT<myMesh>::setVertexSelection(SegmentHandle _h,bool _state){
	SegmentPointerContainer::const_iterator sp_it = segmentPointerContainer_.find(_h);
	if(sp_it == segmentPointerContainer_.end()) return false;
	Segment* segment = sp_it->second;
	setVertexSelection(segment->mesh_, _state, &segment->vertexHandleSet_);
}

///insert all selected  vertices from mesh _mesh into _vhc
template <typename myMesh> void MeshSegmentCollectionT<myMesh>::harvestSelectedVertices(myMesh* _mesh, VertexHandleSet& _vhc){
    //int count;
    typename myMesh::VertexIter v_it, v_end;

    v_it = _mesh->vertices_begin();
    v_end = _mesh->vertices_end();
    for(; v_it != v_end;++v_it){
        if(_mesh->status(v_it.handle()).selected()) _vhc.insert(v_it.handle());
    }
}

template <typename myMesh> void MeshSegmentCollectionT<myMesh>::assignVertices(const VertexHandleSet& _VH, SegmentHandle _h, myMesh* _mesh){
    typedef std::map<SegmentHandle, Segment*> SegmentMap;
    VertexHandleSet::const_iterator vh_it, vh_ite;

    vh_it = _VH.begin();
    vh_ite = _VH.end();
    int h;
    SegmentMap::iterator s_it;
    Segment *segment;
    OpenMesh::VPropHandleT<SegmentHandle> vp_h;
    _mesh->get_property_handle(vp_h, MeshSegmentT<TriMesh>::defaultVertexPropName());


    segment = get(_h);
    //for each vertex handle
    for(; vh_it != vh_ite; ++vh_it){
        //check the old property
        h = _mesh->property(vp_h,*vh_it);
        //check if we've already found this segment
        if (h != _h && h != 0){
            s_it = segmentPointerContainer_.find(h);
            if(s_it != segmentPointerContainer_.end()) (s_it->second->remove(*vh_it));
        }
        segment->add(*vh_it);
    }
}


template <typename myMesh> bool MeshSegmentCollectionT<myMesh>::test_SegmentInitialiseFromMesh(myMesh* _mesh){
    OpenMesh::VPropHandleT<typename SegmentHandle> vPropHandle;
	if(!_mesh->get_property_handle(vPropHandle, MeshSegmentT<myMesh>::defaultVertexPropName())) { return false;} //if the segment property does not exist then exit
	//delete the current Segmentation
	segmentPointerContainer_.clear();

	myMesh::VertexIter v_it = _mesh->vertices_begin();
	myMesh::VertexIter v_end = _mesh->vertices_end();

	//for each vertex
	for(;v_it != v_end; ++v_it){
	//	if we don't already have a segment with this handle then
		SegmentHandle segmentHandle = _mesh->property(vPropHandle, v_it.handle());
        Segment* segment;
        segment = get(segmentHandle);
		if ( segment == 0) {
	//		create a new segment
			Color color = _mesh->color(v_it.handle());
			 segment = add(_mesh, color);
	//		set handle, colour and mesh pointer
			segment->segmentHandle_ = segmentHandle;
		}
//		add vertex handle
		segment->add(v_it.handle());
	}
	//re-evaluate maximum segment handle and set member variable
	SegmentPointerContainer::const_iterator sp_it = segmentPointerContainer_.begin();
	SegmentPointerContainer::const_iterator sp_end = segmentPointerContainer_.end();
	SegmentHandle maxHandle = 0;
	for(;sp_it != sp_end; ++sp_it){
		maxHandle = maxHandle > (*sp_it).first ? maxHandle : (*sp_it).first;
	}
	maxHandle_ = maxHandle;
	

}

//template predeclares
template class MeshSegmentCollectionT<TriMesh>;
template class MeshSegmentCollectionT<PolyMesh>;

