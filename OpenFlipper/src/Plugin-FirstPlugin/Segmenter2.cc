#include "Segmenter2.hh"

template <typename myMesh> MeshSegmentT<myMesh>::MeshSegmentT(myMesh* _mesh, SegmentHandle _segmentHandle){

    //if the vertex segmentation property doesn't exist:
    mesh_ = _mesh;
    segmentHandle_ = _segmentHandle;
    if(!_mesh->get_property_handle(vertexPropHandle_, defaultVertexPropName())){
        _mesh->add_property(vertexPropHandle_, defaultVertexPropName());
        myMesh::VertexIter v_it = _mesh->vertices_begin(), v_end = _mesh->vertices_end();
        for(;v_it != v_end;++v_it){ //initialise the vertex segments to 0
            _mesh->property(vertexPropHandle_,v_it)=0;
        }
    }
    std::stringstream sstr;
    sstr << "Segment" << segmentHandle_;
    name_ = sstr.str();
}

template <typename myMesh> MeshSegmentT<myMesh>::MeshSegmentT(myMesh* _mesh, SegmentHandle _segmentHandle, std::string _name){
    MeshSegmentT(_mesh, _segmentHandle);
    name_ = _name;
}

template <typename myMesh> MeshSegmentT<myMesh>::~MeshSegmentT(){
    VertexHandleSet::const_iterator vv_it = vertexHandleSet_.begin(), vv_end = vertexHandleSet_.end();
    for(;vv_it != vv_end;++vv_it){ //initialise the vertex segments to 0
        mesh_->property(vertexPropHandle_,*vv_it)=0;
    }
}

template <typename myMesh> void MeshSegmentT<myMesh>::add(VertexHandle _v){
    mesh_->property(vertexPropHandle_, _v) = segmentHandle_;
    vertexHandleSet_.insert(_v);
}

template <typename myMesh> bool MeshSegmentT<myMesh>::remove(VertexHandle _v){
    VertexHandleSet::const_iterator v_it = vertexHandleSet_.find(_v);
    if(v_it != vertexHandleSet_.end()) {
        mesh_->property(vertexPropHandle_,_v) = 0;
        vertexHandleSet_.erase(v_it);
        return true;
    }
    return false;
}

template <typename myMesh> MeshSegmentCollectionT<myMesh>::~MeshSegmentCollectionT(){
    SegmentPointerContainer::const_iterator s_it = segmentPointerContainer_.begin();
    for(;s_it != segmentPointerContainer_.end(); ++s_it){
        delete (s_it->second);
    }
}


template <typename myMesh> MeshSegmentT<myMesh>* MeshSegmentCollectionT<myMesh>::add(myMesh* _mesh){


    Segment* segment = new Segment(_mesh, maxHandle_++);
    segment->mesh_ = _mesh;
    //segments_.push_back(segment);
    segmentPointerContainer_[segment->segmentHandle_] = segment;
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


template  <typename myMesh> MeshSegmentT<myMesh>* const MeshSegmentCollectionT<myMesh>::get(SegmentHandle _h){
    SegmentPointerContainer::iterator sp_it = segmentPointerContainer_.find(_h);
    if (sp_it == segmentPointerContainer_.end()) return 0;
    return sp_it->second;
}

//removed when converting segment vector to map
//template <typename myMesh> unsigned int MeshSegmentCollectionT<myMesh>::getSegmentIndex(SegmentHandle _h){ //linear search for segment handle
//    SegmentPointerContainer::iterator s_it;
//
//    segmentPointerContainer_
////    int i = 0;
////    for(; s_it != segmentPointerContainer_.end(); ++s_it){
////        if ((*s_it)->segmentHandle_ = _h) return i;
////        i++;
////    }
////    return 0;
//}

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

//template predeclares
template class MeshSegmentT<TriMesh>;
template class MeshSegmentT<PolyMesh>;
template class MeshSegmentCollectionT<TriMesh>;
template class MeshSegmentCollectionT<PolyMesh>;

