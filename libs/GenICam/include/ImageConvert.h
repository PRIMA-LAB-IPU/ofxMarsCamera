/**
* @file   ImageConvert.h
* @brief  ͼ���ʽת��.
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

	/** ��ҁE�ρE*/
#ifdef WIN32  // win64λ�£��ᱨ��def�ظ�����ľ���EIn Win 64bit, it will report warning for duplicate definition
#  ifdef _USRDLL // ��̬�⵼��EExport DLL
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

	typedef void* IMGCNV_HANDLE;        ///< תEPIʹ�õľ䱁EHandle for using conversion API

	/**
	* @enum IMGCNV_EErr
	* @brief �ӿڷ���ֵ Return value
	* @attention �� No
	*/
	typedef enum tagIMGCNV_EErr
	{
		IMGCNV_SUCCESS,
		IMGCNV_ILLEGAL_PARAM,			 ///< �Ƿ����� Illegal parameter
		IMGCNV_ERR_ORDER,                ///< ���ýӿ�˳����ESequence error for calling interfaces
		IMGCNV_NO_MEMORY,				 ///< �ڴ治ׁENo memory
		IMGCNV_NOT_SUPPORT,              ///< ��֧��   Not support
	}IMGCNV_EErr;

	typedef struct tagIMGCNV_SOpenParam
	{
		int width;							///< ͼ��EImage width
		int height;							///< ͼ��� Image Height
		int paddingX;						///< ͼ�����ρEPadding X
		int paddingY;						///< ͼ�����ρEPadding Y
		int dataSize;						///< ͼ���С   Image size
		unsigned int pixelForamt;			///< Դͼ���ͼ���ʽ Image format of source image
	}IMGCNV_SOpenParam;

	typedef enum tagIMGCNV_EBayerDemosaic
	{
		IMGCNV_DEMOSAIC_NEAREST_NEIGHBOR,		 ///< ��فE� Nearest neighbor
		IMGCNV_DEMOSAIC_BILINEAR,				 ///< ˫���� Bilinear
		IMGCNV_DEMOSAIC_EDGE_SENSING,			 ///< ��Ե��E�EEdge sensing
		IMGCNV_DEMOSAIC_NOT_SUPPORT = 255,       ///< ��֧�� Not support
	}IMGCNV_EBayerDemosaic;

	/**
    *  ~chinese  
	*  @brief  ת��ΪBGR24��ת������
	*  @param[in] pSrcData		��Դ����
	*  @param[in] pOpenParam	��IMGCNV_SOpenParam�ṹ́E��ʽת������Ҫ�Ĳ���
	*  @param[out]pDstData		��ת���������	
	*  @param[out]pDstDataSize	��ת�������ݵĳ���	
	*  @Return:   IMGCNV_ERR_E  : ͼ���ʽת���ӿڷ���ֵ
	*  - IMGCNV_SUCCESS ���ִ�гɹ�
	*  - ����ֵ��IMGCNV_ERR_Eö��
	*  �ر�˵ÁE
	*  ���ظ�ʽΪYUV411Packed��ʱ��ͼ����Eܱ�4����
	*  ���ظ�ʽΪYUV422Packed��ʱ��ͼ����Eܱ�2����
	*  ���ظ�ʽΪYUYVPacked��ʱ��ͼ����Eܱ�2����
	*  ת�����ͼρE���ݴ洢�Ǵ��������һ�п�ʼ�ģ������������ݵ�Ĭ�ϴ洢��ρE
	*  ~english  
	*  @brief  convert to BGR24
	*  @param[in] pSrcData		��source data
	*  @param[in] pOpenParam	��conversion required paraneters
	*  @param[out]pDstData		��converted data
	*  @param[out]pDstDataSize	��length of converted data 
	*  @Return:   IMGCNV_ERR_E	:return value
	*  - IMGCNV_SUCCESS return ok
	*  - Other values refers to enumeration of IMGCNV_ERR_E 
	*  Special note
	*  pixelFormat:YUV411Packed,the image width is divisible by 4
	*  pixelFormat:YUV422Packed,the image width is divisible by 2
	*  pixelFormat:YUYVPacked��the image width is divisible by 2
	*  converted image��The first row of the image is located at the start of the image buffer.This is the default for image taken by a camera.
	*/
	IMGCNV_API IMGCNV_EErr CALLMETHOD IMGCNV_ConvertToBGR24(unsigned char* pSrcData, IMGCNV_SOpenParam* pOpenParam, unsigned char* pDstData, int* pDstDataSize);

	/**
    *  ~chinese  
	*  @brief  ת��ΪRGB24��ת������
	*  @param[in] pSrcData		��Դ����
	*  @param[in] pOpenParam	��IMGCNV_SOpenParam�ṹ́E��ʽת������Ҫ�Ĳ���
	*  @param[out]pDstData		��ת���������	
	*  @param[out]pDstDataSize	��ת�������ݵĳ���	
	*  @Return:   IMGCNV_ERR_E  : ͼ���ʽת���ӿڷ���ֵ
	*  - IMGCNV_SUCCESS ���ִ�гɹ�
	*  - ����ֵ��IMGCNV_ERR_Eö��
	*  �ر�˵ÁE
	*  ���ظ�ʽΪYUV411Packed��ʱ��ͼ����Eܱ�4����
	*  ���ظ�ʽΪYUV422Packed��ʱ��ͼ����Eܱ�2����
	*  ���ظ�ʽΪYUYVPacked��ʱ��ͼ����Eܱ�2����
	*  ת�����ͼρE���ݴ洢�Ǵ��������һ�п�ʼ�ģ������������ݵ�Ĭ�ϴ洢��ρE
	*  ~english  
	*  @brief  convert to RGB24
	*  @param[in] pSrcData		��source data
	*  @param[in] pOpenParam	��conversion required paraneters
	*  @param[out]pDstData		��converted data
	*  @param[out]pDstDataSize	��length of converted data 
	*  @Return:   IMGCNV_ERR_E	:return value
	*  - IMGCNV_SUCCESS return ok
	*  - Other values refers to enumeration of IMGCNV_ERR_E 
	*  Special note
	*  pixelFormat:YUV411Packed,the image width is divisible by 4
	*  pixelFormat:YUV422Packed,the image width is divisible by 2
	*  pixelFormat:YUYVPacked��the image width is divisible by 2
	*  converted image��The first row of the image is located at the start of the image buffer.This is the default for image taken by a camera.
	*/
	IMGCNV_API IMGCNV_EErr CALLMETHOD IMGCNV_ConvertToRGB24(unsigned char* pSrcData, IMGCNV_SOpenParam* pOpenParam, unsigned char* pDstData, int* pDstDataSize);

	/**
	*  ~chinese
	*  @brief  ת��ΪMono8��ת������
	*  @param[in] pSrcData		��Դ����
	*  @param[in] pOpenParam	��IMGCNV_SOpenParam�ṹ́E��ʽת������Ҫ�Ĳ���
	*  @param[out]pDstData		��ת���������
	*  @param[out]pDstDataSize	��ת�������ݵĳ���
	*  @Return:   IMGCNV_ERR_E  : ͼ���ʽת���ӿڷ���ֵ
	*  - IMGCNV_SUCCESS ���ִ�гɹ�
	*  - ����ֵ��IMGCNV_ERR_Eö��
	*  �ر�˵ÁE
	*  ���ظ�ʽΪYUV411Packed��ʱ��ͼ����Eܱ�4����
	*  ���ظ�ʽΪYUV422Packed��ʱ��ͼ����Eܱ�2����
	*  ���ظ�ʽΪYUYVPacked��ʱ��ͼ����Eܱ�2����
	*  ת�����ͼρE���ݴ洢�Ǵ��������һ�п�ʼ�ģ������������ݵ�Ĭ�ϴ洢��ρE
	*  ~english
	*  @brief  convert to Mono8
	*  @param[in] pSrcData		��source data
	*  @param[in] pOpenParam	��conversion required paraneters
	*  @param[out]pDstData		��converted data
	*  @param[out]pDstDataSize	��length of converted data
	*  @Return:   IMGCNV_ERR_E	:return value
	*  - IMGCNV_SUCCESS return ok
	*  - Other values refers to enumeration of IMGCNV_ERR_E
	*  Special note
	*  pixelFormat:YUV411Packed,the image width is divisible by 4
	*  pixelFormat:YUV422Packed,the image width is divisible by 2
	*  pixelFormat:YUYVPacked��the image width is divisible by 2
	*  converted image��The first row of the image is located at the start of the image buffer.This is the default for image taken by a camera.
	*/
	IMGCNV_API IMGCNV_EErr CALLMETHOD IMGCNV_ConvertToMono8(unsigned char* pSrcData, IMGCNV_SOpenParam* pOpenParam, unsigned char* pDstData, int* pDstDataSize);

	/**
	*  ~chinese
	*  @brief  ת��ΪBGR24��ת������
	*  @param[in] pSrcData		��Դ����
	*  @param[in] pOpenParam	��IMGCNV_SOpenParam�ṹ́E��ʽת������Ҫ�Ĳ���
	*  @param[out]pDstData		��ת���������
	*  @param[out]pDstDataSize	��ת�������ݵĳ���
	*  @param[in]eBayerDemosaic ��ѡ��ת��Bayer��ʽ���õ��㷨
	*  @Return:   IMGCNV_ERR_E  : ͼ���ʽת���ӿڷ���ֵ
	*  - IMGCNV_SUCCESS ���ִ�гɹ�
	*  - ����ֵ��IMGCNV_ERR_Eö��
	*  �ر�˵ÁE
	*  ���ظ�ʽΪYUV411Packed��ʱ��ͼ����Eܱ�4����
	*  ���ظ�ʽΪYUV422Packed��ʱ��ͼ����Eܱ�2����
	*  ���ظ�ʽΪYUYVPacked��ʱ��ͼ����Eܱ�2����
	*  ת�����ͼρE���ݴ洢�Ǵ��������һ�п�ʼ�ģ������������ݵ�Ĭ�ϴ洢��ρE
	*  ~english
	*  @brief  convert to BGR24
	*  @param[in] pSrcData		��source data
	*  @param[in] pOpenParam	��conversion required paraneters
	*  @param[out]pDstData		��converted data
	*  @param[out]pDstDataSize	��length of converted data
	*  @param[in]eBayerDemosaic ��alorithm used for Bayer demosaic
	*  @Return:   IMGCNV_ERR_E	:return value
	*  - IMGCNV_SUCCESS return ok
	*  - Other values refers to enumeration of IMGCNV_ERR_E
	*  Special note
	*  pixelFormat:YUV411Packed,the image width is divisible by 4
	*  pixelFormat:YUV422Packed,the image width is divisible by 2
	*  pixelFormat:YUYVPacked��the image width is divisible by 2
	*  converted image��The first row of the image is located at the start of the image buffer.This is the default for image taken by a camera.
	*/
	IMGCNV_API IMGCNV_EErr CALLMETHOD IMGCNV_ConvertToBGR24_Ex(unsigned char* pSrcData, IMGCNV_SOpenParam* pOpenParam, unsigned char* pDstData, int* pDstDataSize, IMGCNV_EBayerDemosaic eBayerDemosaic);

	/**
	*  ~chinese
	*  @brief  ת��ΪRGB24��ת������
	*  @param[in] pSrcData		��Դ����
	*  @param[in] pOpenParam	��IMGCNV_SOpenParam�ṹ́E��ʽת������Ҫ�Ĳ���
	*  @param[out]pDstData		��ת���������
	*  @param[out]pDstDataSize	��ת�������ݵĳ���
	*  @param[in]eBayerDemosaic ��ѡ��ת��Bayer��ʽ���õ��㷨
	*  @Return:   IMGCNV_ERR_E  : ͼ���ʽת���ӿڷ���ֵ
	*  - IMGCNV_SUCCESS ���ִ�гɹ�
	*  - ����ֵ��IMGCNV_ERR_Eö��
	*  �ر�˵ÁE
	*  ���ظ�ʽΪYUV411Packed��ʱ��ͼ����Eܱ�4����
	*  ���ظ�ʽΪYUV422Packed��ʱ��ͼ����Eܱ�2����
	*  ���ظ�ʽΪYUYVPacked��ʱ��ͼ����Eܱ�2����
	*  ת�����ͼρE���ݴ洢�Ǵ��������һ�п�ʼ�ģ������������ݵ�Ĭ�ϴ洢��ρE
	*  ~english
	*  @brief  convert to RGB24
	*  @param[in] pSrcData		��source data
	*  @param[in] pOpenParam	��conversion required paraneters
	*  @param[out]pDstData		��converted data
	*  @param[out]pDstDataSize	��length of converted data
	*  @param[in]eBayerDemosaic ��alorithm used for Bayer demosaic
	*  @Return:   IMGCNV_ERR_E	:return value
	*  - IMGCNV_SUCCESS return ok
	*  - Other values refers to enumeration of IMGCNV_ERR_E
	*  Special note
	*  pixelFormat:YUV411Packed,the image width is divisible by 4
	*  pixelFormat:YUV422Packed,the image width is divisible by 2
	*  pixelFormat:YUYVPacked��the image width is divisible by 2
	*  converted image��The first row of the image is located at the start of the image buffer.This is the default for image taken by a camera.
	*/
	IMGCNV_API IMGCNV_EErr CALLMETHOD IMGCNV_ConvertToRGB24_Ex(unsigned char* pSrcData, IMGCNV_SOpenParam* pOpenParam, unsigned char* pDstData, int* pDstDataSize, IMGCNV_EBayerDemosaic eBayerDemosaic);

	/**
	*  ~chinese
	*  @brief  ת��ΪMono8��ת������
	*  @param[in] pSrcData		��Դ����
	*  @param[in] pOpenParam	��IMGCNV_SOpenParam�ṹ́E��ʽת������Ҫ�Ĳ���
	*  @param[out]pDstData		��ת���������
	*  @param[out]pDstDataSize	��ת�������ݵĳ���
	*  @param[in]eBayerDemosaic ��ѡ��ת��Bayer��ʽ���õ��㷨
	*  @Return:   IMGCNV_ERR_E  : ͼ���ʽת���ӿڷ���ֵ
	*  - IMGCNV_SUCCESS ���ִ�гɹ�
	*  - ����ֵ��IMGCNV_ERR_Eö��
	*  �ر�˵ÁE
	*  ���ظ�ʽΪYUV411Packed��ʱ��ͼ����Eܱ�4����
	*  ���ظ�ʽΪYUV422Packed��ʱ��ͼ����Eܱ�2����
	*  ���ظ�ʽΪYUYVPacked��ʱ��ͼ����Eܱ�2����
	*  ת�����ͼρE���ݴ洢�Ǵ��������һ�п�ʼ�ģ������������ݵ�Ĭ�ϴ洢��ρE
	*  ~english
	*  @brief  convert to Mono8
	*  @param[in] pSrcData		��source data
	*  @param[in] pOpenParam	��conversion required paraneters
	*  @param[out]pDstData		��converted data
	*  @param[out]pDstDataSize	��length of converted data
	*  @param[in]eBayerDemosaic ��alorithm used for Bayer demosaic
	*  @Return:   IMGCNV_ERR_E	:return value
	*  - IMGCNV_SUCCESS return ok
	*  - Other values refers to enumeration of IMGCNV_ERR_E
	*  Special note
	*  pixelFormat:YUV411Packed,the image width is divisible by 4
	*  pixelFormat:YUV422Packed,the image width is divisible by 2
	*  pixelFormat:YUYVPacked��the image width is divisible by 2
	*  converted image��The first row of the image is located at the start of the image buffer.This is the default for image taken by a camera.
	*/
	IMGCNV_API IMGCNV_EErr CALLMETHOD IMGCNV_ConvertToMono8_Ex(unsigned char* pSrcData, IMGCNV_SOpenParam* pOpenParam, unsigned char* pDstData, int* pDstDataSize, IMGCNV_EBayerDemosaic eBayerDemosaic);


    /**
    *  ~chinese
    *  @brief  ת��ΪBGRA32��ת������
    *  @param[in] pSrcData		��Դ����
    *  @param[in] pOpenParam	��IMGCNV_SOpenParam�ṹ́E��ʽת������Ҫ�Ĳ���
    *  @param[out]pDstData		��ת���������
    *  @param[out]pDstDataSize	��ת�������ݵĳ���
    *  @param[in]eBayerDemosaic ��ѡ��ת��Bayer��ʽ���õ��㷨
    *  @Return:   IMGCNV_ERR_E  : ͼ���ʽת���ӿڷ���ֵ
    *  - IMGCNV_SUCCESS ���ִ�гɹ�
    *  - ����ֵ��IMGCNV_ERR_Eö��
    *  �ر�˵ÁE
    *  ���ظ�ʽΪYUV411Packed��ʱ��ͼ����Eܱ�4����
    *  ���ظ�ʽΪYUV422Packed��ʱ��ͼ����Eܱ�2����
    *  ���ظ�ʽΪYUYVPacked��ʱ��ͼ����Eܱ�2����
    *  ת�����ͼρE���ݴ洢�Ǵ��������һ�п�ʼ�ģ������������ݵ�Ĭ�ϴ洢��ρE
    *  ~english
    *  @brief  convert to BGRA32
    *  @param[in] pSrcData		��source data
    *  @param[in] pOpenParam	��conversion required paraneters
    *  @param[out]pDstData		��converted data
    *  @param[out]pDstDataSize	��length of converted data
    *  @param[in]eBayerDemosaic ��alorithm used for Bayer demosaic
    *  @Return:   IMGCNV_ERR_E	:return value
    *  - IMGCNV_SUCCESS return ok
    *  - Other values refers to enumeration of IMGCNV_ERR_E
    *  Special note
    *  pixelFormat:YUV411Packed,the image width is divisible by 4
    *  pixelFormat:YUV422Packed,the image width is divisible by 2
    *  pixelFormat:YUYVPacked��the image width is divisible by 2
    *  converted image��The first row of the image is located at the start of the image buffer.This is the default for image taken by a camera.
    */
    IMGCNV_API IMGCNV_EErr CALLMETHOD IMGCNV_ConvertToBGRA32_Ex(unsigned char* pSrcData, IMGCNV_SOpenParam* pOpenParam, unsigned char* pDstData, int* pDstDataSize, IMGCNV_EBayerDemosaic eBayerDemosaic);

#ifdef  __cplusplus
}
#endif // end #ifdef  __cplusplus

#endif // end of #ifndef __IMAGE_CONVERT_H_