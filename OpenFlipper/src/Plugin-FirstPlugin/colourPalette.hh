
#include <../ObjectTypes/PolyMesh/PolyMesh.hh>
#include <../ObjectTypes/TriangleMesh/TriangleMesh.hh>
struct MeshSegmentColors{
	typedef typename OpenMesh::Vec4f Color; //default colour format for TriMesh and PolyMesh
	static const Color colorRed(1,0,0,1);
	static const Color colorGreen(0,1,0,1);
	static const Color colorBlue(0,0,1,1);
	static const Color colorYellow(1,1,0,1);
	static const Color colorY


