
#include <../ObjectTypes/PolyMesh/PolyMesh.hh>
#include <../ObjectTypes/TriangleMesh/TriangleMesh.hh>
struct ColorPalette{
	typedef typename OpenMesh::Vec4f Color;
	static const Color red(1,0,0,1);
	static const Color green(0,1,0,1);
	static const Color blue(0,0,1,1);
	static const Color yello
