/*! \cond NODOXYGEN */
#ifndef XMEDLLAPI_H
#define XMEDLLAPI_H

#ifndef XME_DLL_API

	#ifdef XME_EXPORT
		#ifdef _WIN32
			//#pragma message("*** XME_DLL_API EXport win")
			#define XME_DLL_API __declspec(dllexport)
		#else
			//#pragma message("*** XME_DLL_API EXport other")
			#define XME_DLL_API __attribute__((visibility("default")))
		#endif 	// _WIN32
	#else
		#ifdef _WIN32
			//#pragma message("*** XME_DLL_API IMport win")
			#define XME_DLL_API __declspec(dllimport)
		#else
			//#pragma message("*** XME_DLL_API IMport other")
			#define XME_DLL_API
		#endif	// _WIN32
	#endif
#else
	//#pragma message("*** XME_DLL_API already defined")
#endif

#endif
/*! \endcond */
