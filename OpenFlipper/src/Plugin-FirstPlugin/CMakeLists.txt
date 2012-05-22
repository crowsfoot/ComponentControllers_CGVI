include (plugin)

openflipper_plugin (DIRS alglib
	ADDSRC MeshSegmentCollectionT.cc MeshSegmentT.cc SegmentDescriptor.cc alglib/linalg.cpp alglib/linalg.h alglib/alglibinternal.cpp alglib/alglibmisc.cpp alglib/ap.cpp alglib/linalg.cpp)
