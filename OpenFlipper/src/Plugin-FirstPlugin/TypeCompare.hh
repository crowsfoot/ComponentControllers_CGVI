
#ifndef TYPECOMPAREE_HH
#define TYPECOMPAREE_HH
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

//short class enabling type comparisons
template <typename myClass> class TypeCompareT{
public:
    template <typename obj1> static bool isType(obj1 _class){
        return false;
    }
    template <> static bool isType<myClass>(myClass _class){
        return true;
    }
    template <> static bool isType<myClass*>(myClass* _class){
        return true;
    }
};

template  class TypeCompareT<PolyMesh>;
template  class TypeCompareT<TriMesh>;

#endif
