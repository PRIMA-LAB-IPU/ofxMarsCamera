/**
* @file   ImageConvert.h
* @brief  ÕºœÒ∏Ò Ω◊™ªª.
* @author 
* @par    Copyright (c):
*         All Rights Reserved
* @version 1.0.0.1
*/

#ifndef __IMAGE_CONVERT_H_
#define __IMAGE_CONVERT_H_

#ifdef  __cplusplus
extern "C"{
#endif  // end #ifdef  __cplusplus

#ifdef WIN64
#undef WIN32
#endif

	/** ±‡“ÅE°œÅE*/
#ifdef WIN32  // win64Œªœ¬£¨ª·±®∫Õdef÷ÿ∏¥∂®“ÂµƒæØ∏ÅEIn Win 64bit, it will report warning for duplicate definition
#  ifdef _USRDLL // ∂ØÃ¨ø‚µº≥ÅEExport DLL
#    ifdef IMAGECONVERT_EXPORTS
#		define IMGCNV_API __declspec(dllexport)
#	 else
#		define IMGCNV_API __declspec(dllimport)
#	 endif
#  else
#    define IMGCNV_API
#  endif // end of ifdef _USRDLL
#else
#	define IMGCNV_API
#endif //end #ifdef WIN32

#if (defined (WIN32) || defined(WIN64))
#   define CALLMETHOD __stdcall
#else
#	define CALLMETHOD
#endif // end #if (defined (WIN32) || defined(WIN64))

	typedef void* IMGCNV_HANDLE;        ///< ◊™¬ÅEPI π”√µƒæ‰±ÅEHandle for using conversion API

	/**
	* @enum IMGCNV_EErr
	* @brief Ω”ø⁄∑µªÿ÷µ Return value
	* @attention Œﬁ No
	*/
	typedef enum tagIMGCNV_EErr
	{
		IMGCNV_SUCCESS,
		IMGCNV_ILLEGAL_PARAM,			 ///< ∑«∑®≤Œ ˝ Illegal parameter
		IMGCNV_ERR_ORDER,                ///< µ˜”√Ω”ø⁄À≥–Ú¥˙ÍÅESequence error for calling interfaces
		IMGCNV_NO_MEMORY,				 ///< ƒ⁄¥Ê≤ª◊ÅENo memory
		IMGCNV_NOT_SUPPORT,              ///< ≤ª÷ß≥÷   Not support
	}IMGCNV_EErr;

	typedef struct tagIMGCNV_SOpenParam
	{
		int width;							///< ÕºœÒøÅEImage width
		int height;							///< ÕºœÒ∏ﬂ Image Height
		int paddingX;						///< ÕºœÒø˙Ë˚œÅEPadding X
		int paddingY;						///< ÕºœÒ∏ﬂÃ˚œÅEPadding Y
		int dataSize;						///< ÕºœÒ¥Û–°   Image size
		unsigned int pixelForamt;			///< ‘¥ÕºœÒµƒÕºœÒ∏Ò Ω Image format of source image
	}IMGCNV_SOpenParam;

	typedef enum tagIMGCNV_EBayerDemosaic
	{
		IMGCNV_DEMOSAIC_NEAREST_NEIGHBOR,		 ///< ◊˚ŸÅE⁄ Nearest neighbor
		IMGCNV_DEMOSAIC_BILINEAR,				 ///< À´œﬂ–‘ Bilinear
		IMGCNV_DEMOSAIC_EDGE_SENSING,			 ///< ±ﬂ‘µºÅEÅEEdge sensing
		IMGCNV_DEMOSAIC_NOT_SUPPORT = 255,       ///< ≤ª÷ß≥÷ Not support
	}IMGCNV_EBayerDemosaic;

	/**
    *  ~chinese  
	*  @brief  ◊™ªªŒ™BGR24µƒ◊™ªª∫Ø ˝
	*  @param[in] pSrcData		£∫‘¥ ˝æ›
	*  @param[in] pOpenParam	£∫IMGCNV_SOpenParamΩ·ππÃÅE∏Ò Ω◊™ªªÀ˘–Ë“™µƒ≤Œ ˝
	*  @param[out]pDstData		£∫◊™ªª∫Ûµƒ ˝æ›	
	*  @param[out]pDstDataSize	£∫◊™ªª∫Û ˝æ›µƒ≥§∂»	
	*  @Return:   IMGCNV_ERR_E  : ÕºœÒ∏Ò Ω◊™ªªΩ”ø⁄∑µªÿ÷µ
	*  - IMGCNV_SUCCESS ±˙Êæ÷¥––≥…π¶
	*  - ∆‰À˚÷µº˚IMGCNV_ERR_E√∂æŸ
	*  Ãÿ±Àµ√ÅE
	*  œÒÀÿ∏Ò ΩŒ™YUV411Packedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª4’˚≥˝
	*  œÒÀÿ∏Ò ΩŒ™YUV422Packedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª2’˚≥˝
	*  œÒÀÿ∏Ò ΩŒ™YUYVPackedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª2’˚≥˝
	*  ◊™ªª∫ÛµƒÕºœÅE ˝æ›¥Ê¥¢ «¥”◊˚Âœ√Êµ⁄“ª––ø™ ºµƒ£¨’‚∏ˆ «œ‡ª˙ ˝æ›µƒƒ¨»œ¥Ê¥¢∑ΩœÅE
	*  ~english  
	*  @brief  convert to BGR24
	*  @param[in] pSrcData		£∫source data
	*  @param[in] pOpenParam	£∫conversion required paraneters
	*  @param[out]pDstData		£∫converted data
	*  @param[out]pDstDataSize	£∫length of converted data 
	*  @Return:   IMGCNV_ERR_E	:return value
	*  - IMGCNV_SUCCESS return ok
	*  - Other values refers to enumeration of IMGCNV_ERR_E 
	*  Special note
	*  pixelFormat:YUV411Packed,the image width is divisible by 4
	*  pixelFormat:YUV422Packed,the image width is divisible by 2
	*  pixelFormat:YUYVPacked£¨the image width is divisible by 2
	*  converted image£∫The first row of the image is located at the start of the image buffer.This is the default for image taken by a camera.
	*/
	IMGCNV_API IMGCNV_EErr CALLMETHOD IMGCNV_ConvertToBGR24(unsigned char* pSrcData, IMGCNV_SOpenParam* pOpenParam, unsigned char* pDstData, int* pDstDataSize);

	/**
    *  ~chinese  
	*  @brief  ◊™ªªŒ™RGB24µƒ◊™ªª∫Ø ˝
	*  @param[in] pSrcData		£∫‘¥ ˝æ›
	*  @param[in] pOpenParam	£∫IMGCNV_SOpenParamΩ·ππÃÅE∏Ò Ω◊™ªªÀ˘–Ë“™µƒ≤Œ ˝
	*  @param[out]pDstData		£∫◊™ªª∫Ûµƒ ˝æ›	
	*  @param[out]pDstDataSize	£∫◊™ªª∫Û ˝æ›µƒ≥§∂»	
	*  @Return:   IMGCNV_ERR_E  : ÕºœÒ∏Ò Ω◊™ªªΩ”ø⁄∑µªÿ÷µ
	*  - IMGCNV_SUCCESS ±˙Êæ÷¥––≥…π¶
	*  - ∆‰À˚÷µº˚IMGCNV_ERR_E√∂æŸ
	*  Ãÿ±Àµ√ÅE
	*  œÒÀÿ∏Ò ΩŒ™YUV411Packedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª4’˚≥˝
	*  œÒÀÿ∏Ò ΩŒ™YUV422Packedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª2’˚≥˝
	*  œÒÀÿ∏Ò ΩŒ™YUYVPackedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª2’˚≥˝
	*  ◊™ªª∫ÛµƒÕºœÅE ˝æ›¥Ê¥¢ «¥”◊˚Âœ√Êµ⁄“ª––ø™ ºµƒ£¨’‚∏ˆ «œ‡ª˙ ˝æ›µƒƒ¨»œ¥Ê¥¢∑ΩœÅE
	*  ~english  
	*  @brief  convert to RGB24
	*  @param[in] pSrcData		£∫source data
	*  @param[in] pOpenParam	£∫conversion required paraneters
	*  @param[out]pDstData		£∫converted data
	*  @param[out]pDstDataSize	£∫length of converted data 
	*  @Return:   IMGCNV_ERR_E	:return value
	*  - IMGCNV_SUCCESS return ok
	*  - Other values refers to enumeration of IMGCNV_ERR_E 
	*  Special note
	*  pixelFormat:YUV411Packed,the image width is divisible by 4
	*  pixelFormat:YUV422Packed,the image width is divisible by 2
	*  pixelFormat:YUYVPacked£¨the image width is divisible by 2
	*  converted image£∫The first row of the image is located at the start of the image buffer.This is the default for image taken by a camera.
	*/
	IMGCNV_API IMGCNV_EErr CALLMETHOD IMGCNV_ConvertToRGB24(unsigned char* pSrcData, IMGCNV_SOpenParam* pOpenParam, unsigned char* pDstData, int* pDstDataSize);

	/**
	*  ~chinese
	*  @brief  ◊™ªªŒ™Mono8µƒ◊™ªª∫Ø ˝
	*  @param[in] pSrcData		£∫‘¥ ˝æ›
	*  @param[in] pOpenParam	£∫IMGCNV_SOpenParamΩ·ππÃÅE∏Ò Ω◊™ªªÀ˘–Ë“™µƒ≤Œ ˝
	*  @param[out]pDstData		£∫◊™ªª∫Ûµƒ ˝æ›
	*  @param[out]pDstDataSize	£∫◊™ªª∫Û ˝æ›µƒ≥§∂»
	*  @Return:   IMGCNV_ERR_E  : ÕºœÒ∏Ò Ω◊™ªªΩ”ø⁄∑µªÿ÷µ
	*  - IMGCNV_SUCCESS ±˙Êæ÷¥––≥…π¶
	*  - ∆‰À˚÷µº˚IMGCNV_ERR_E√∂æŸ
	*  Ãÿ±Àµ√ÅE
	*  œÒÀÿ∏Ò ΩŒ™YUV411Packedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª4’˚≥˝
	*  œÒÀÿ∏Ò ΩŒ™YUV422Packedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª2’˚≥˝
	*  œÒÀÿ∏Ò ΩŒ™YUYVPackedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª2’˚≥˝
	*  ◊™ªª∫ÛµƒÕºœÅE ˝æ›¥Ê¥¢ «¥”◊˚Âœ√Êµ⁄“ª––ø™ ºµƒ£¨’‚∏ˆ «œ‡ª˙ ˝æ›µƒƒ¨»œ¥Ê¥¢∑ΩœÅE
	*  ~english
	*  @brief  convert to Mono8
	*  @param[in] pSrcData		£∫source data
	*  @param[in] pOpenParam	£∫conversion required paraneters
	*  @param[out]pDstData		£∫converted data
	*  @param[out]pDstDataSize	£∫length of converted data
	*  @Return:   IMGCNV_ERR_E	:return value
	*  - IMGCNV_SUCCESS return ok
	*  - Other values refers to enumeration of IMGCNV_ERR_E
	*  Special note
	*  pixelFormat:YUV411Packed,the image width is divisible by 4
	*  pixelFormat:YUV422Packed,the image width is divisible by 2
	*  pixelFormat:YUYVPacked£¨the image width is divisible by 2
	*  converted image£∫The first row of the image is located at the start of the image buffer.This is the default for image taken by a camera.
	*/
	IMGCNV_API IMGCNV_EErr CALLMETHOD IMGCNV_ConvertToMono8(unsigned char* pSrcData, IMGCNV_SOpenParam* pOpenParam, unsigned char* pDstData, int* pDstDataSize);

	/**
	*  ~chinese
	*  @brief  ◊™ªªŒ™BGR24µƒ◊™ªª∫Ø ˝
	*  @param[in] pSrcData		£∫‘¥ ˝æ›
	*  @param[in] pOpenParam	£∫IMGCNV_SOpenParamΩ·ππÃÅE∏Ò Ω◊™ªªÀ˘–Ë“™µƒ≤Œ ˝
	*  @param[out]pDstData		£∫◊™ªª∫Ûµƒ ˝æ›
	*  @param[out]pDstDataSize	£∫◊™ªª∫Û ˝æ›µƒ≥§∂»
	*  @param[in]eBayerDemosaic £∫—°‘Ò◊™ªªBayer∏Ò ΩÀ˘”√µƒÀ„∑®
	*  @Return:   IMGCNV_ERR_E  : ÕºœÒ∏Ò Ω◊™ªªΩ”ø⁄∑µªÿ÷µ
	*  - IMGCNV_SUCCESS ±˙Êæ÷¥––≥…π¶
	*  - ∆‰À˚÷µº˚IMGCNV_ERR_E√∂æŸ
	*  Ãÿ±Àµ√ÅE
	*  œÒÀÿ∏Ò ΩŒ™YUV411Packedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª4’˚≥˝
	*  œÒÀÿ∏Ò ΩŒ™YUV422Packedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª2’˚≥˝
	*  œÒÀÿ∏Ò ΩŒ™YUYVPackedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª2’˚≥˝
	*  ◊™ªª∫ÛµƒÕºœÅE ˝æ›¥Ê¥¢ «¥”◊˚Âœ√Êµ⁄“ª––ø™ ºµƒ£¨’‚∏ˆ «œ‡ª˙ ˝æ›µƒƒ¨»œ¥Ê¥¢∑ΩœÅE
	*  ~english
	*  @brief  convert to BGR24
	*  @param[in] pSrcData		£∫source data
	*  @param[in] pOpenParam	£∫conversion required paraneters
	*  @param[out]pDstData		£∫converted data
	*  @param[out]pDstDataSize	£∫length of converted data
	*  @param[in]eBayerDemosaic £∫alorithm used for Bayer demosaic
	*  @Return:   IMGCNV_ERR_E	:return value
	*  - IMGCNV_SUCCESS return ok
	*  - Other values refers to enumeration of IMGCNV_ERR_E
	*  Special note
	*  pixelFormat:YUV411Packed,the image width is divisible by 4
	*  pixelFormat:YUV422Packed,the image width is divisible by 2
	*  pixelFormat:YUYVPacked£¨the image width is divisible by 2
	*  converted image£∫The first row of the image is located at the start of the image buffer.This is the default for image taken by a camera.
	*/
	IMGCNV_API IMGCNV_EErr CALLMETHOD IMGCNV_ConvertToBGR24_Ex(unsigned char* pSrcData, IMGCNV_SOpenParam* pOpenParam, unsigned char* pDstData, int* pDstDataSize, IMGCNV_EBayerDemosaic eBayerDemosaic);

	/**
	*  ~chinese
	*  @brief  ◊™ªªŒ™RGB24µƒ◊™ªª∫Ø ˝
	*  @param[in] pSrcData		£∫‘¥ ˝æ›
	*  @param[in] pOpenParam	£∫IMGCNV_SOpenParamΩ·ππÃÅE∏Ò Ω◊™ªªÀ˘–Ë“™µƒ≤Œ ˝
	*  @param[out]pDstData		£∫◊™ªª∫Ûµƒ ˝æ›
	*  @param[out]pDstDataSize	£∫◊™ªª∫Û ˝æ›µƒ≥§∂»
	*  @param[in]eBayerDemosaic £∫—°‘Ò◊™ªªBayer∏Ò ΩÀ˘”√µƒÀ„∑®
	*  @Return:   IMGCNV_ERR_E  : ÕºœÒ∏Ò Ω◊™ªªΩ”ø⁄∑µªÿ÷µ
	*  - IMGCNV_SUCCESS ±˙Êæ÷¥––≥…π¶
	*  - ∆‰À˚÷µº˚IMGCNV_ERR_E√∂æŸ
	*  Ãÿ±Àµ√ÅE
	*  œÒÀÿ∏Ò ΩŒ™YUV411Packedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª4’˚≥˝
	*  œÒÀÿ∏Ò ΩŒ™YUV422Packedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª2’˚≥˝
	*  œÒÀÿ∏Ò ΩŒ™YUYVPackedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª2’˚≥˝
	*  ◊™ªª∫ÛµƒÕºœÅE ˝æ›¥Ê¥¢ «¥”◊˚Âœ√Êµ⁄“ª––ø™ ºµƒ£¨’‚∏ˆ «œ‡ª˙ ˝æ›µƒƒ¨»œ¥Ê¥¢∑ΩœÅE
	*  ~english
	*  @brief  convert to RGB24
	*  @param[in] pSrcData		£∫source data
	*  @param[in] pOpenParam	£∫conversion required paraneters
	*  @param[out]pDstData		£∫converted data
	*  @param[out]pDstDataSize	£∫length of converted data
	*  @param[in]eBayerDemosaic £∫alorithm used for Bayer demosaic
	*  @Return:   IMGCNV_ERR_E	:return value
	*  - IMGCNV_SUCCESS return ok
	*  - Other values refers to enumeration of IMGCNV_ERR_E
	*  Special note
	*  pixelFormat:YUV411Packed,the image width is divisible by 4
	*  pixelFormat:YUV422Packed,the image width is divisible by 2
	*  pixelFormat:YUYVPacked£¨the image width is divisible by 2
	*  converted image£∫The first row of the image is located at the start of the image buffer.This is the default for image taken by a camera.
	*/
	IMGCNV_API IMGCNV_EErr CALLMETHOD IMGCNV_ConvertToRGB24_Ex(unsigned char* pSrcData, IMGCNV_SOpenParam* pOpenParam, unsigned char* pDstData, int* pDstDataSize, IMGCNV_EBayerDemosaic eBayerDemosaic);

	/**
	*  ~chinese
	*  @brief  ◊™ªªŒ™Mono8µƒ◊™ªª∫Ø ˝
	*  @param[in] pSrcData		£∫‘¥ ˝æ›
	*  @param[in] pOpenParam	£∫IMGCNV_SOpenParamΩ·ππÃÅE∏Ò Ω◊™ªªÀ˘–Ë“™µƒ≤Œ ˝
	*  @param[out]pDstData		£∫◊™ªª∫Ûµƒ ˝æ›
	*  @param[out]pDstDataSize	£∫◊™ªª∫Û ˝æ›µƒ≥§∂»
	*  @param[in]eBayerDemosaic £∫—°‘Ò◊™ªªBayer∏Ò ΩÀ˘”√µƒÀ„∑®
	*  @Return:   IMGCNV_ERR_E  : ÕºœÒ∏Ò Ω◊™ªªΩ”ø⁄∑µªÿ÷µ
	*  - IMGCNV_SUCCESS ±˙Êæ÷¥––≥…π¶
	*  - ∆‰À˚÷µº˚IMGCNV_ERR_E√∂æŸ
	*  Ãÿ±Àµ√ÅE
	*  œÒÀÿ∏Ò ΩŒ™YUV411Packedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª4’˚≥˝
	*  œÒÀÿ∏Ò ΩŒ™YUV422Packedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª2’˚≥˝
	*  œÒÀÿ∏Ò ΩŒ™YUYVPackedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª2’˚≥˝
	*  ◊™ªª∫ÛµƒÕºœÅE ˝æ›¥Ê¥¢ «¥”◊˚Âœ√Êµ⁄“ª––ø™ ºµƒ£¨’‚∏ˆ «œ‡ª˙ ˝æ›µƒƒ¨»œ¥Ê¥¢∑ΩœÅE
	*  ~english
	*  @brief  convert to Mono8
	*  @param[in] pSrcData		£∫source data
	*  @param[in] pOpenParam	£∫conversion required paraneters
	*  @param[out]pDstData		£∫converted data
	*  @param[out]pDstDataSize	£∫length of converted data
	*  @param[in]eBayerDemosaic £∫alorithm used for Bayer demosaic
	*  @Return:   IMGCNV_ERR_E	:return value
	*  - IMGCNV_SUCCESS return ok
	*  - Other values refers to enumeration of IMGCNV_ERR_E
	*  Special note
	*  pixelFormat:YUV411Packed,the image width is divisible by 4
	*  pixelFormat:YUV422Packed,the image width is divisible by 2
	*  pixelFormat:YUYVPacked£¨the image width is divisible by 2
	*  converted image£∫The first row of the image is located at the start of the image buffer.This is the default for image taken by a camera.
	*/
	IMGCNV_API IMGCNV_EErr CALLMETHOD IMGCNV_ConvertToMono8_Ex(unsigned char* pSrcData, IMGCNV_SOpenParam* pOpenParam, unsigned char* pDstData, int* pDstDataSize, IMGCNV_EBayerDemosaic eBayerDemosaic);


    /**
    *  ~chinese
    *  @brief  ◊™ªªŒ™BGRA32µƒ◊™ªª∫Ø ˝
    *  @param[in] pSrcData		£∫‘¥ ˝æ›
    *  @param[in] pOpenParam	£∫IMGCNV_SOpenParamΩ·ππÃÅE∏Ò Ω◊™ªªÀ˘–Ë“™µƒ≤Œ ˝
    *  @param[out]pDstData		£∫◊™ªª∫Ûµƒ ˝æ›
    *  @param[out]pDstDataSize	£∫◊™ªª∫Û ˝æ›µƒ≥§∂»
    *  @param[in]eBayerDemosaic £∫—°‘Ò◊™ªªBayer∏Ò ΩÀ˘”√µƒÀ„∑®
    *  @Return:   IMGCNV_ERR_E  : ÕºœÒ∏Ò Ω◊™ªªΩ”ø⁄∑µªÿ÷µ
    *  - IMGCNV_SUCCESS ±˙Êæ÷¥––≥…π¶
    *  - ∆‰À˚÷µº˚IMGCNV_ERR_E√∂æŸ
    *  Ãÿ±Àµ√ÅE
    *  œÒÀÿ∏Ò ΩŒ™YUV411Packedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª4’˚≥˝
    *  œÒÀÿ∏Ò ΩŒ™YUV422Packedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª2’˚≥˝
    *  œÒÀÿ∏Ò ΩŒ™YUYVPackedµƒ ±£¨ÕºœÒø˙ÏÅE‹±ª2’˚≥˝
    *  ◊™ªª∫ÛµƒÕºœÅE ˝æ›¥Ê¥¢ «¥”◊˚Âœ√Êµ⁄“ª––ø™ ºµƒ£¨’‚∏ˆ «œ‡ª˙ ˝æ›µƒƒ¨»œ¥Ê¥¢∑ΩœÅE
    *  ~english
    *  @brief  convert to BGRA32
    *  @param[in] pSrcData		£∫source data
    *  @param[in] pOpenParam	£∫conversion required paraneters
    *  @param[out]pDstData		£∫converted data
    *  @param[out]pDstDataSize	£∫length of converted data
    *  @param[in]eBayerDemosaic £∫alorithm used for Bayer demosaic
    *  @Return:   IMGCNV_ERR_E	:return value
    *  - IMGCNV_SUCCESS return ok
    *  - Other values refers to enumeration of IMGCNV_ERR_E
    *  Special note
    *  pixelFormat:YUV411Packed,the image width is divisible by 4
    *  pixelFormat:YUV422Packed,the image width is divisible by 2
    *  pixelFormat:YUYVPacked£¨the image width is divisible by 2
    *  converted image£∫The first row of the image is located at the start of the image buffer.This is the default for image taken by a camera.
    */
    IMGCNV_API IMGCNV_EErr CALLMETHOD IMGCNV_ConvertToBGRA32_Ex(unsigned char* pSrcData, IMGCNV_SOpenParam* pOpenParam, unsigned char* pDstData, int* pDstDataSize, IMGCNV_EBayerDemosaic eBayerDemosaic);

#ifdef  __cplusplus
}
#endif // end #ifdef  __cplusplus

#endif // end of #ifndef __IMAGE_CONVERT_H_