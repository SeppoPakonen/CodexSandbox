#ifndef CORE_H
#define CORE_H

#ifdef WINSOCK_HACK_FIX
	#define _WS2DEF_
	#define _WINSOCK2API_
	#define _WS2IPDEF_
	#define _WS2TCPIP_H_

#else
	#ifdef flagWIN32
		#include <ws2tcpip.h>
		#include <Windows.h>
		#include <winsock2.h>
		#if defined flagAMP
			#include <amp.h>
			#include <amp_math.h>
			#include <amp_graphics.h>
			#include <dxgi.h>
			#include <d3dcommon.h>
			#include <d3d9types.h>
			#include <d3d11.h>
			#include <d3dcompiler.h>
			#include <directxmath.h>
		#endif
	#endif
#endif




#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <string.h>

#if defined flagPOSIX
	#include <unistd.h>
#endif

#if defined flagMSC
	#include <string.h> // memcmp
#endif

#include <functional>
#include <chrono>
#include <initializer_list>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <algorithm>
#include <tuple>
#include <shared_mutex>
#include <atomic>
#include <functional>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <codecvt>
#include <random>

#include <emmintrin.h>

#undef near
#undef far


#ifdef flagGLM
#include <plugin/glm/glm.hpp>
#include <plugin/glm/gtc/type_ptr.hpp>
#include <plugin/glm/gtx/transform.hpp>
#include <plugin/glm/gtx/intersect.hpp>
#include <plugin/f/gtx/matrix_decompose.hpp>
#include <plugin/glm/gtx/euler_angles.hpp>
#include <plugin/glm/gtc/matrix_transform.hpp>
#include <plugin/glm/gtc/type_ptr.hpp>
#include <plugin/glm/trigonometric.hpp>
#include <plugin/glm/gtx/quaternion.hpp>

using namespace glm;
#endif

using namespace std::chrono;



#ifdef __GNUG__
#include <cstdlib>
#include <memory>
#include <cxxabi.h>
#endif


#define DLOG(x) LOG(x)


#define NAMESPACE_SDK_BEGIN namespace  SDK {
#define NAMESPACE_SDK_END }



#include "Defs.h"
#include "Macros.h"
#include "Topt.h"
#include "Ops.h"
#include "Hash.h"
#include "Algo.h"
#include "String_.h"
#include "Tuple.h"
#include "Vector.h"
#include "Stream.h"
#include "Types.h"
#include "TypeTraits.h"
#include "Util.h"
#include "Math.h"
#include "Callback.h"
#include "Time_.h"
#include "Geom.h"
#include "Graph.h"
#include "Thread.h"
#include "Socket.h"
#include "Ptr.h"
#include "Value.h"
#include "Tokenizer.h"
#include "JSON.h"
#include "Management.h"
#include "Neural.h"
#include "Misc.h"
#include "CLI.h"
#include "DCT.h"
#include "Heap.h"
#include "GeneticOptimizer.h"
#include "Version.h"
#include "Coordinate.h"
#include "ECS.h"






#endif
