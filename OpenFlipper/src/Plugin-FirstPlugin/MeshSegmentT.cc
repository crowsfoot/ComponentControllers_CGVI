#include "MeshSegmentT.hh"

///constructor
template <typename myMesh> MeshSegmentT<myMesh>::MeshSegmentT(myMesh* _mesh, SegmentHandle _segmentHandle){

    mesh_ = _mesh;
    segmentHandle_ = _segmentHandle;
    //property initialisation
    if(!_mesh->get_property_handle(vertexPropHandle_, defaultVertexPropName())){ //check and/or instantiate vertex property for segment
        //declare properties
        _mesh->add_property(vertexPropHandle_, defaultVertexPropName());
        _mesh->property(vertexPropHandle_).set_persistent(true);
        if(!_mesh->has_vertex_colors()) _mesh->request_vertex_colors();

        //initialise properties
        Color colorBlue(0.0,0.0,1.0,1.0);
        myMesh::VertexIter v_it = _mesh->vertices_begin(), v_end = _mesh->vertices_end();
        for(;v_it != v_end;++v_it){ //initialise the vertex segments to 0
            _mesh->set_color(v_it.handle(), colorBlue);
            _mesh->property(vertexPropHandle_,v_it)=0;
        }

    }



    std::stringstream sstr; //create name
    sstr << "Segment" << segmentHandle_;
    name_ = sstr.str();
    //segment descriptor
    //segmentDescriptor_ = new SegmentDescriptor(); //implementation unstable
    for(int i = 0; i<4; i++){
        color_[i] = 0.0;
    }



}

template <typename myMesh> MeshSegmentT<myMesh>::MeshSegmentT(myMesh* _mesh, SegmentHandle _segmentHandle, std::string _name){
    MeshSegmentT(_mesh, _segmentHandle);
    name_ = _name;
    segmentDescriptor_ = new SegmentDescriptor();
}

template <typename myMesh> MeshSegmentT<myMesh>::~MeshSegmentT(){
    VertexHandleSet::const_iterator vv_it = vertexHandleSet_.begin(), vv_end = vertexHandleSet_.end();
    for(;vv_it != vv_end;++vv_it){ //initialise the vertex segments to 0
        mesh_->property(vertexPropHandle_,*vv_it)=0;
    }
    delete segmentDescriptor_;
}

template <typename myMesh> void MeshSegmentT<myMesh>::add(VertexHandle _v){
    mesh_->property(vertexPropHandle_, _v) = segmentHandle_;
    mesh_->set_color(_v, color_);
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





template <typename myMesh> void MeshSegmentT<myMesh>::set_color(const Color& _color){
    color_ = _color;
    VertexHandleSet::const_iterator vh_it = vertexHandleSet_.begin();
    VertexHandleSet::const_iterator vh_end = vertexHandleSet_.end();
    for(; vh_it != vh_end; ++vh_it) mesh_->set_color(*vh_it,color_);
}



template <typename myMesh>  void MeshSegmentT<myMesh>::regenerateDescriptor(){
    segmentDescriptor_->generate(mesh_, vertexHandleSet_);
}


//template predeclares
template class MeshSegmentT<TriMesh>;
template class MeshSegmentT<PolyMesh>;

