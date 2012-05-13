/*
 * =====================================================================================
 *
 *  Filename:  Segmenter2IO.hh
 *
 *	Description: Implementation of OpenMesh property IO capability within Segmenter2
 *	
 *
 *   Author:  David Lane
 *
 * =====================================================================================
 */


//ok, this isn't actually useful or necesary
//store the segmentation with the vertices instead
//*!* that was silly


#ifndef SEGMENTCONTROLLERIO_HH
	#define SEGMENTCONTROLLERIO_HH

	//build the template for the polymesh class
	#define SEGMENTCONTROLLER_PCCODE
	#define MESHSEGMENTIO PolyMeshSegment
	#define MESHSEGMENT_CIO PolyMeshSegmentCollection
	#include "Segmenter2IO.hh"
	
	//build the template for the TriMesh class
	#undef MESHSEGMENTIO
	#undef MESHSEGMENT_CIO
	#define MESHSEGMENTIO TriMeshSegment
	#define MESHSEGMENT_CIO TriMeshSegmentCollection
	#include "Segmenter2IO.hh"
	
	#undef MESHSEGMENTIO
	#undef MESHSEGMENT_CIO
	#undef SEGMENTCONTROLLER_PCCODE
	#define SEGMENTCONTROLLERIO_HH
#endif

#ifdef SEGMENTCONTROLLER_PCCODE

#include "Segmenter2.hh"

namespace OpenMesh{
	namespace IO {
		 template <> struct binary<MESHSEGMENTIO>{
			//support persistance for class TrimeshSegment
			typedef MESHSEGMENTIO value_type;
			static const bool is_streamable = true;

			static size_t size_of(void){
				return UnknownSize;
			}

			static size_t size_of(const value_type& _v){
				size_t bytes = 0;
				unsigned int N = _v.VertexHandleSet_.size();//get the size of the segment count
				bytes += IO::size_of(N);
				bytes += IO::size_of(_v.name_); //get the size of name_
				bytes += IO::size_of(_v.segmentHandle_); //get the size of segmentHandle
				//get the size of handles_
				value_type::VertexHandleSet::const_iterator it = _v.VertexHandleSet_.begin(); 
				for(;it!=_v.VertexHandleSet_.end(); ++it)
				{
					bytes += IO::size_of(*it);	
				}
				return bytes;
			}

			static size_t store(std::ostream& _os, const value_type& _v, bool _swap=false){
				size_t bytes = 0;

				unsigned int N = _v.VertexHandleSet_.size(); //get the size of the segment count
				bytes += IO::store(_os,N,_swap);
				bytes += IO::store(_os,_v.name_,_swap); //get the size of name_
				bytes += IO::store(_os,_v.segmentHandle_,_swap);//get the size of handle_
				value_type::VertexHandleSet::const_iterator it = _v.VertexHandleSet_.begin(); //get the size of VertexHandleSet_
				for(;it!=_v.VertexHandleSet_.end(); ++it)
				{
					bytes += IO::store(_os,*it,_swap);	
				}
				return _os.good() ? bytes : 0;
			}

			static size_t restore( std::istream& _is, value_type& _v, bool _swap=false){
				size_t   bytes = 0;
				unsigned int N = 0;

				_v.VertexHandleSet_.clear();
				bytes += IO::restore(_is,N,_swap);//restore member count
				bytes += IO::restore(_is,_v.name_, _swap);//restore name_
				bytes += IO::restore(_is,_v.segmentHandle_,_swap);//restore handle

				//restore vertex handles
				value_type::VertexHandle H;
				for (size_t i = 0; i<N && _is.good(); ++i){
					bytes+= bytes += IO::restore(_is,H,_swap);
					_v.VertexHandleSet_.push_back(H);
				}
				return _is.good() ? bytes : 0;
			}

		};


		//segment collections
		template <> struct binary<MESHSEGMENT_CIO>{
			//support persistance for class TrimeshSegmentCollection
			typedef MESHSEGMENT_CIO value_type;
			static const bool is_streamable = true;

			static size_t size_of(void){
				return UnknownSize;
			}

			static size_t size_of(const value_type& _v){
				size_t bytes = 0;
				//get the size of the segment count
				unsigned int N = _v.segments_.size();
				bytes += IO::size_of(N);

				//get the size of handles_
				value_type::SegmentPointerContainer::const_iterator it = _v.segments_.begin();
				for(;it!=_v.segments_.end(); ++it)
				{
					bytes += IO::size_of(*it);	
				}
				return bytes;
			}

			static size_t store(std::ostream& _os, const value_type& _v, bool _swap=false){
				size_t bytes = 0;
				//get the size of the segment count
				unsigned int N = _v.segments_.size();
				bytes += IO::store(_os,N,_swap);
				//get the size of handles_
				value_type::SegmentPointerContainer::const_iterator it = _v.segments_.begin();

				for(;it!=_v.segments_.end(); ++it)
				{
					bytes += IO::store(_os,*it,_swap);	
				}
				return _os.good() ? bytes : 0;
			}

			static size_t restore( std::istream& _is, value_type& _v, bool _swap=false){
				size_t   bytes = 0;
				unsigned int N = 0;

				_v.segments_.clear();
				//restore member count
				bytes += IO::restore(_is,N,_swap);
				//restore vertex handles
				value_type::Segment* H = new value_type::Segment();
				for (size_t i = 0; i<N && _is.good(); ++i){
					bytes += IO::restore(_is,(*H),_swap);
					_v.segments_.push_back(H);
				}

				value_type::SegmentHandle maxHandle = 0;
				value_type::SegmentPointerContainer::const_iterator s_it = _v.segments_.begin();
				for(;s_it != _v.segments_.end(); ++s_it){
					maxHandle = (*s_it)->segmentHandle_ > maxHandle ? (*s_it)->segmentHandle_ : maxHandle;
				}
				_v.maxHandle_ = maxHandle+1;

				return _is.good() ? bytes : 0;
			}
		};	
		

	}
}


#endif
