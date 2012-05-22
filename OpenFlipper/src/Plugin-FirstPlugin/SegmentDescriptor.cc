#include "SegmentDescriptor.hh"

///empty constructor, initialise to the unit cube  between points (0,0) and (1,1)
template <typename myMesh> SegmentDescriptorT<myMesh>::SegmentDescriptorT(){
	minPoint_.vectorize(0.0);
	maxPoint_.vectorize(0.0);

     basisVectors_ = 0;
     basisVariance_ = 0;
     medianPoint_ = 0;
     worldTransform_ = 0;
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
template <typename myMesh> bool SegmentDescriptorT<myMesh>::generate(myMesh* _mesh, const VertexHandleSet& _vhs){
/*
	int pointCount = _vhs.size();
	alglib::real_2d_array pointsMatrix;
	pointsMatrix.setlength(3,_vhs.size());
	if(basisVectors_ != 0) delete basisVectors_;
        if(basisVariance_ != 0) delete basisVariance_;	
	if (_vhs.size() == 0) return false;

	basisVectors_ = new alglib::real_2d_array;
	basisVariance_ = new alglib::real_1d_array;
	medianPoint_ = new alglib::real_1d_array("[0.0,0.0,0.0]");

	VertexHandleSet::const_iterator vh_it = _vhs.begin(), vh_end = _vhs.end();
	int pointNumber = 0;
	for(; vh_it != vh_end; vh_it++){
		Point point = _mesh->point(*vh_it);
		for(int i = 0; i<3; ++i){
		       	pointsMatrix[i][pointNumber]= point[i];
			(*medianPoint_)[i] = point[i] + (*medianPoint_)[i];
		}
		pointNumber++;
	}
	vmul(arrPtr,stride,memberCount,multiplier);
	alglib::vmul(medianPoint_->getcontent(),1,3,1.0/(double)pointCount);
	
	//normalise the points
    //		subtract median point from all points
    	for(int i = 0; i < 3; i++){
		//vsub(arrPtr,stride,srcPtr,srcStride,variableCount)
		alglib::vsub(&points[0][i], pointsMatrix.getstride(),medianPoint_->getcontent(),1,3);
	}

    //find the PCA
	//info return values:
	//-4:hasn't converged
	//-1: bad parameters
	//1:task solved
	alglib::ae_int_t info;
	alglib::pcabuildbasis(pointsMatrix,_vhs.size(),3,*basisVariance_, *basisVectors_);
	if (info!=1){
		delete basisVectors_;
		delete basisVariance_;
		delete medianPoint_;
		basisVariance_ = 0;
		basisVectors_ = 0;
		medianPoint_ = 0;
		return false;
	}

	alglib::ae_int_t invOpInfo;
	alglib::matinvreport invRep;
	alglib::real_2d_array pcaInv(), pcaTranspose();
	pcaTranspose.setlength(3,3);
	pcaInv.setlength(3,3);
	alglib::rmatrixtranspose(3,3,*basisVectors,0,0,pcaTranspose);
	alglib::rmatrixinverse(pcaTranspose,2,invOpInfo,invRep); //true if matrix is not singular

	//get the transformation matrix: inverse of the transverse of the PCA
	worldTransform_ = new alglib::real_2d_array();
	worldTransform_->setlength(4,4);
	
	/*
	alglib::rmatrixinverse(
	alglib::cmatrixgemm(3,pointCount,
	*/

    

    //find new minmax for scaling


	return true;
/*	
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
	*/
}

///return a world transform which assumes a base frame of between (1,1,1) and (-1,-1,-1)
///(the frame used by OpenFlipper's InsertCube plugin function
template <typename myMesh> alglib::real_2d_array SegmentDescriptorT<myMesh>::worldTransformAL(){
    alglib::real_1d_array P1,P2,I1,I2, A;
    alglib::real_2d_array source,original,final;
    original = "[[-1,-1,-1,1],[-0.33,-0.33,-0.33,1],[0.33,0.33,0.33,1],[1,1,1,1]]";
    source.setlength(4,4);
    for(int i = 0; i < 4; i++){
        Point A = ((1-i*0.66) * minPoint_) + ((i*0.66)*maxPoint_);
        for(int j = 0; j < 4; j++){
            source[i][j]=A[j];
        }
        source[i][3]=1;
    }
    return source;
}

///set the matrix size to 4,4 and fill it with the identity
template <typename myMesh> void Identity4d(alglib::real_2d_array& _mat){
	double I[] = {1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0};
	_mat.setlength(4,4);
	_mat.setcontent(I);
    
}

template <typename myMesh> Matrix4x4 SegmentDescriptorT<myMesh>::worldTransformOF(){
    Matrix4x4 a;
    return a;
}
			
template class SegmentDescriptorT<TriMesh>;
template class SegmentDescriptorT<PolyMesh>;