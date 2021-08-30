// pngResourceDll.cpp : 定义 DLL 的导出函数。
//

#include "pch.h"
#include "framework.h"
#include "pngResourceDll.h"
#include "resource.h"

//#include <windows.h>
//#include <Gdiplus.h>
//using namespace Gdiplus;
//#pragma comment(lib,"gdiplus.lib")

#include <wincodec.h>
#include <opencv2/opencv.hpp>

void t1();

BOOL HBitmap2Mat(HBITMAP& _hBmp, cv::Mat& _mat)
{
	BITMAP bmp;
	GetObject(_hBmp, sizeof(BITMAP), &bmp);
	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? 1 : 8;
	cv::Mat v_mat;
	v_mat.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8UC3, nChannels));
	GetBitmapBits(_hBmp, bmp.bmHeight*bmp.bmWidth*nChannels, v_mat.data);
	_mat = v_mat;
	return FALSE;
}

// 这是导出变量的一个示例
PNGRESOURCEDLL_API int npngResourceDll=0;

// 这是导出函数的一个示例。
PNGRESOURCEDLL_API int fnpngResourceDll(void)
{
	HBITMAP gHmp;
	cv::Mat gMat;
	gHmp = LoadBitmap(GetModuleHandleW(L"PNGRESOURCEDLL.dll"), MAKEINTRESOURCE(IDB_BITMAP1));
	if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
	HBitmap2Mat(gHmp, gMat);

	//gHmp = LoadBitmap(GetModuleHandleW(L"PNGRESOURCEDLL.dll"), MAKEINTRESOURCE(IDB_PNG1));
	//if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
	//HBitmap2Mat(gHmp, gMat);

	//gHmp = LoadResource(GetModuleHandleW(L"PNGRESOURCEDLL.dll"), MAKEINTRESOURCE(IDB_PNG2));
	//if (gHmp == NULL)throw"LoadSource Get Resource From Dll HBitmap faile";
	//HBitmap2Mat(gHmp, gMat);

	HMODULE h=GetModuleHandleA("PNGRESOURCEDLL.dll");
	int a = (int)MAKEINTRESOURCE(IDB_PNG1);
	HRSRC hResource = ::FindResource(h, MAKEINTRESOURCE(IDB_PNG2), L"PNG");
	Sleep(10000);

	t1();

	//if (!hResource)
	//{
	//	DWORD error = GetLastError();
	//	return -1;
	//}

	//DWORD imageSize = ::SizeofResource(GetModuleHandleW(L"PNGRESOURCEDLL.dll"), hResource);
	//if (!imageSize)
	//	return -2;

	//const void* pResourceData = ::LockResource(::LoadResource(GetModuleHandleW(L"PNGRESOURCEDLL.dll"),
	//	hResource));
	//if (!pResourceData)
	//	return -3;

	//// Lock the resource to retrieve memory pointer.
	//if (SUCCEEDED(hr)) {
	//	pImageFile = LockResource(imageResDataHandle);
	//	hr = (pImageFile ? S_OK : E_FAIL);
	//}

	//// Calculate the size.
	//if (SUCCEEDED(hr)) {
	//	imageFileSize = SizeofResource(NULL, imageResHandle);
	//	hr = (imageFileSize ? S_OK : E_FAIL);
	//}


	//Gdiplus::Bitmap* m_pBitmap;

	//HGLOBAL m_hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);

	//if (m_hBuffer)
	//{
	//	void* pBuffer = ::GlobalLock(m_hBuffer);
	//	if (pBuffer)
	//	{
	//		CopyMemory(pBuffer, pResourceData, imageSize);

	//		IStream* pStream = NULL;
	//		if (::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
	//		{
	//			m_pBitmap = Gdiplus::Bitmap::FromStream(pStream);
	//			pStream->Release();
	//			if (m_pBitmap)
	//			{
	//				if (m_pBitmap->GetLastStatus() == Gdiplus::Ok)
	//					return true;

	//				delete m_pBitmap;
	//				m_pBitmap = NULL;
	//			}
	//		}
	//		m_pBitmap = NULL;
	//		::GlobalUnlock(m_hBuffer);
	//	}
	//	::GlobalFree(m_hBuffer);
	//	m_hBuffer = NULL;
	//}

	return 1;
}

#include <wincodec.h>

void t1()
{
	HRESULT hr = S_OK;

	IWICStream *pIWICStream = NULL;
	IWICBitmapDecoder *pIDecoder = NULL;
	IWICBitmapFrameDecode *pIDecoderFrame = NULL;
	IWICImagingFactory *m_pIWICFactory = NULL;
	IWICBitmapSource *bitmap_source = NULL;
	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void *pImageFile = NULL;
	DWORD imageFileSize = 0;

	


	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

	CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pIWICFactory)
	);

	imageResHandle = FindResource(GetModuleHandleW(L"PNGRESOURCEDLL.dll"), MAKEINTRESOURCE(IDB_PNG1), L"PNG");
	hr = (imageResHandle ? S_OK : E_FAIL);

	if (SUCCEEDED(hr)) {
		imageResDataHandle = LoadResource(GetModuleHandleW(L"PNGRESOURCEDLL.dll"), imageResHandle);
		hr = (imageResDataHandle ? S_OK : E_FAIL);
	}

	// Lock the resource to retrieve memory pointer.
	if (SUCCEEDED(hr)) {
		pImageFile = LockResource(imageResDataHandle);
		hr = (pImageFile ? S_OK : E_FAIL);
	}

	// Calculate the size.
	if (SUCCEEDED(hr)) {
		imageFileSize = SizeofResource(GetModuleHandleW(L"PNGRESOURCEDLL.dll"), imageResHandle);
		hr = (imageFileSize ? S_OK : E_FAIL);
	}

	// Create a WIC stream to map onto the memory.
	if (SUCCEEDED(hr)) {
		hr = m_pIWICFactory->CreateStream(&pIWICStream);
	}

	// Initialize the stream with the memory pointer and size.
	if (SUCCEEDED(hr)) {
		hr = pIWICStream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(pImageFile),
			imageFileSize);
	}

	// Create a decoder for the stream.
	if (SUCCEEDED(hr)) {
		hr = m_pIWICFactory->CreateDecoderFromStream(
			pIWICStream,                   // The stream to use to create the decoder
			NULL,                          // Do not prefer a particular vendor
			WICDecodeMetadataCacheOnLoad,  // Cache metadata when needed
			&pIDecoder);                   // Pointer to the decoder
	}

	// Retrieve the initial frame.
	if (SUCCEEDED(hr)) {
		hr = pIDecoder->GetFrame(0, &pIDecoderFrame);
	}


	bitmap_source = pIDecoderFrame;

	UINT width = 0, height = 0, depht=4;
	bitmap_source->GetSize(&width, &height);

	cv::Mat gMat;

	{
		std::vector<BYTE> buffer(width * height * depht);
		bitmap_source->CopyPixels(NULL, width * depht, buffer.size(), buffer.data());

		HBITMAP bitmap = CreateBitmap(width, height, 1, 8 * depht, buffer.data());

		HBitmap2Mat(bitmap, gMat);

		Sleep(2000);
		buffer.resize(0);
		buffer.reserve(0);

		DeleteObject(bitmap);
	}

	DeleteObject(bitmap_source);

	Sleep(2000);

	cvtColor(gMat, gMat, cv::COLOR_RGBA2RGB);
	Sleep(2000);
	Sleep(2000);
	Sleep(2000);
	return;

	//SafeRelease(m_pIWICFactory);
}

// 这是已导出类的构造函数。
CpngResourceDll::CpngResourceDll()
{
    return;
}

#include <vector>
void asd()
{
	//std::vector<std::vector<int>> D;
	int A[10][10] = 
	{
		{0,1,0},
		{ 4 ,0,2},

	};
	int nD[10] = { 0 };
	int nU[10] = { 0 };

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			nD[i] += A[i][j];
		}
	}
}

//#include <iostream>
//#include <string>
//#include <Gdiplus.h>
//using namespace Gdiplus;
//
//void GetPNG_Res(Graphics &g, String pngName)
//{
//
//	TResourceStream *Res1 = new TResourceStream((int)HInstance, pngName, "png");
//	HRSRC hRsrc = ::FindResourceA((HMODULE)HInstance, pngName.c_str(), "png");
//	if (!hRsrc)
//		return;
//
//	//load resource into memory
//	DWORD len = SizeofResource((HMODULE)HInstance, hRsrc);
//	BYTE* lpRsrc = (BYTE*)LoadResource((HMODULE)HInstance, hRsrc);
//	if (!lpRsrc)
//		return;
//
//	//Allocate global memory on which to create stream
//	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
//	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
//	memcpy(pmem, lpRsrc, len);
//	IStream* pstm;
//	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
//	Gdiplus::Bitmap * Image = Gdiplus::Bitmap::FromStream(pstm); //Gdiplus::Bitmap::FromStream(pstm);
//
//	RectF r(0, 0, Image->GetWidth(), Image->GetHeight());
//	g.DrawImage(Image, r);
//	//free/release stuff
//	//GlobalUnlock(m_hMem);
//	pstm->Release();
//	GlobalFree(m_hMem);
//	FreeResource(lpRsrc);
//
//	//m_Image->GetWidth(),m_Image->GetHeight()
//	delete Res1;
//	delete Image;
//}
//
//#include <Windows.h>
//#include <Gdiplus.h>
////#pragma comment(lib, "Gdiplus.lib")
//using namespace Gdiplus;
//using namespace DllExport;
//
//void DrawImage(HDC hdc, RECT& rc, UINT nIDResource, CString lpType)
//{
//	HINSTANCE hInst = AfxGetResourceHandle();
//	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nIDResource), lpType);
//	if (!hRsrc)
//		return;
//
//	//资源大小
//	DWORD len = SizeofResource(hInst, hRsrc);
//	//转换图片资源为字节数组
//	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
//	if (!lpRsrc)
//		return;
//
//	IImagingFactory *pImgFactory = NULL;
//	IImage *pImage = NULL;
//
//	CoInitializeEx(NULL, COINIT_MULTITHREADED);
//
//	if (SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory,
//		NULL,
//		CLSCTX_INPROC_SERVER,
//		IID_IImagingFactory,
//		(void **)&pImgFactory)))
//	{
//		if (SUCCEEDED(pImgFactory->CreateImageFromBuffer(lpRsrc, len, BufferDisposalFlagNone, &pImage)))
//		{
//			pImage->Draw(hdc, &rc, NULL);
//			pImage->Release();
//		}
//		pImgFactory->Release();
//	}
//	CoUninitialize();
//
//	//释放资源
//	FreeResource(hRsrc);
//}
//
//void er()
//{
//
//	ULONG_PTR gdipid;
//	// 初始化
//	GdiplusStartInput gsi;
//	GdiplusStart(&gsi, &gdipid);
//	// 加载图片（常用格式均支持）
//	// API版
//	GpBitmap *bmp;
//	GdipCreateBitmapFormFile(L"文件名.png", &bmp);
//	HBITMAP hBmp;
//	SIZE sBmp;
//	GdipGetImageWidth((GpImage*)bmp, LPUINT(sBmp.cx));
//	GdipGetImageHeight((GpImage*)bmp, LPUINT(sBmp.cy));
//	GdipCreateHBITMAPFormBitmap(bmp, 0, &hBmp);
//	GdipDi***Image((GpImage*)bmp); // 记不到了所以打了*
//	// 类版
//	Bitmap *bmp = Bitmap::FormFile(L"文件名.png");
//	SIZE sBmp = { bmp->GetWidth(), bmp->GetHeight() };
//	HBITMAP hBmp = bmp->GetHBITMAP(0);
//	delete bmp;
//	// 这样就把png加载到了，HBITMAP里面，注意啊，画png一定要用AlphaBlend或GdiAlphaBlend函数画，才有Alpha通道。
//	// 卸载GDI+
//	GdiplusShutdown(gdipid);
//
//
//	HBITMAP hBmp; // 加载过程已有这里省去
//	HDC hMemDC = CreateCompatibleDC(0);
//	BLENDFUNCTION bf = { 0, 0, 255, AC_SRC_ALPHA };
//	SelectObject(hMemDC, hBmp);
//	//AlphaBlend(目标DC, 目标x, 目标y, 目标宽度, 目标高度, hMemDC, 来源x, 来源y, 来源宽度, 来源高度, bf);
//	DeleteDC(hMemDC);
//}
//
//void test()
//{
//	{
//		// TODO: Add extra cleanup here
//		HMODULE hDll = LoadLibrary("456.dll");
//
//		if (!hDll)
//			return;
//		HRSRC hrsRC = FindResource(hDll, MAKEINTRESOURCE(PNG1), "PNG");
//		if (!hrsRC)
//		{
//			int err = GetLastError();
//			return;
//		}
//
//		HGLOBAL hgb = LoadResource(NULL, hrsRC);
//		if (!hgb)
//		{
//			return;
//		}
//		LPVOID Buf = LockResource(hgb);
//		FILE* fp = fopen("c://idr.png", "wb+");
//		if (fwrite(Buf, SizeofResource(NULL, hrsRC), 1, fp) == -1)
//		{
//			return;
//		}
//		fclose(fp);
//		FreeLibrary(hDll);
//	}
//	{
//		TResourceStream* stream;
//		try
//		{
//			stream = new TResourceStream((UINT)HInstance, "E1", "EXEFILE"); //
//			if (stream)
//			{
//				stream->SaveToFile("c:/456.exe");
//			}
//		}
//		__finally
//		{
//			delete stream;
//		}
//	}
//}

//HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
//
//IWICImagingFactory *piFactory = NULL;
//IWICBitmapDecoder *piDecoder = NULL;
//
//// Create the COM imaging factory.
//if (SUCCEEDED(hr))
//{
//	hr = CoCreateInstance(
//		CLSID_WICImagingFactory,
//		NULL,
//		CLSCTX_INPROC_SERVER,
//		IID_PPV_ARGS(&piFactory)
//	);
//
//}
//
//// Create the decoder.
//if (SUCCEEDED(hr))
//{
//	hr = piFactory->CreateDecoderFromFilename(L"001.png", NULL, GENERIC_READ,
//		WICDecodeMetadataCacheOnDemand, //For JPEG lossless decoding/encoding.
//		&piDecoder);
//}
//
//// Variables used for encoding.
//IWICStream *piFileStream = NULL;
//IWICBitmapEncoder *piEncoder = NULL;
//
//
//WICPixelFormatGUID pixelFormat = { 0 };
//UINT count = 0;
//double dpiX, dpiY = 0.0;
//UINT width, height = 0;
//
//// Create a file stream.
//if (SUCCEEDED(hr))
//{
//	hr = piFactory->CreateStream(&piFileStream);
//}
//else
//{
//	int asd = GetLastError();
//	return asd;
//}
//
//// Initialize our new file stream.
//if (SUCCEEDED(hr))
//{
//	hr = piFileStream->InitializeFromFilename(L"001.png", GENERIC_WRITE);
//}
//
//// Create the encoder.
//if (SUCCEEDED(hr))
//{
//	hr = piFactory->CreateEncoder(GUID_ContainerFormatJpeg, NULL, &piEncoder);
//}
//// Initialize the encoder
//if (SUCCEEDED(hr))
//{
//	hr = piEncoder->Initialize(piFileStream, WICBitmapEncoderNoCache);
//}
//
//if (SUCCEEDED(hr))
//{
//	hr = piDecoder->GetFrameCount(&count);
//}
//
//if (SUCCEEDED(hr))
//{
//	// Process each frame of the image.
//	for (UINT i = 0; i < count && SUCCEEDED(hr); i++)
//	{
//		// Frame variables.
//		IWICBitmapFrameDecode *piFrameDecode = NULL;
//		IWICBitmapFrameEncode *piFrameEncode = NULL;
//		IWICMetadataQueryReader *piFrameQReader = NULL;
//		IWICMetadataQueryWriter *piFrameQWriter = NULL;
//
//		// Get and create the image frame.
//		if (SUCCEEDED(hr))
//		{
//			hr = piDecoder->GetFrame(i, &piFrameDecode);
//		}
//		if (SUCCEEDED(hr))
//		{
//			hr = piEncoder->CreateNewFrame(&piFrameEncode, NULL);
//		}
//
//		// Initialize the encoder.
//		if (SUCCEEDED(hr))
//		{
//			hr = piFrameEncode->Initialize(NULL);
//		}
//		// Get and set the size.
//		if (SUCCEEDED(hr))
//		{
//			hr = piFrameDecode->GetSize(&width, &height);
//		}
//		if (SUCCEEDED(hr))
//		{
//			hr = piFrameEncode->SetSize(width, height);
//		}
//		// Get and set the resolution.
//		if (SUCCEEDED(hr))
//		{
//			piFrameDecode->GetResolution(&dpiX, &dpiY);
//		}
//		if (SUCCEEDED(hr))
//		{
//			hr = piFrameEncode->SetResolution(dpiX, dpiY);
//		}
//		// Set the pixel format.
//		if (SUCCEEDED(hr))
//		{
//			piFrameDecode->GetPixelFormat(&pixelFormat);
//		}
//		if (SUCCEEDED(hr))
//		{
//			hr = piFrameEncode->SetPixelFormat(&pixelFormat);
//		}
//
//		// Check that the destination format and source formats are the same.
//		bool formatsEqual = FALSE;
//		if (SUCCEEDED(hr))
//		{
//			GUID srcFormat;
//			GUID destFormat;
//
//			hr = piDecoder->GetContainerFormat(&srcFormat);
//			if (SUCCEEDED(hr))
//			{
//				hr = piEncoder->GetContainerFormat(&destFormat);
//			}
//			if (SUCCEEDED(hr))
//			{
//				if (srcFormat == destFormat)
//					formatsEqual = true;
//				else
//					formatsEqual = false;
//			}
//		}
//
//		if (SUCCEEDED(hr) && formatsEqual)
//		{
//			// Copy metadata using metadata block reader/writer.
//			//if (SUCCEEDED(hr))
//			//{
//			//	piFrameDecode->QueryInterface(IID_PPV_ARGS(&piBlockReader));
//			//}
//			//if (SUCCEEDED(hr))
//			//{
//			//	piFrameEncode->QueryInterface(IID_PPV_ARGS(&piBlockWriter));
//			//}
//			//if (SUCCEEDED(hr))
//			//{
//			//	piBlockWriter->InitializeFromBlockReader(piBlockReader);
//			//}
//		}
//
//		if (SUCCEEDED(hr))
//		{
//			hr = piFrameEncode->GetMetadataQueryWriter(&piFrameQWriter);
//		}
//		if (SUCCEEDED(hr))
//		{
//			// Add additional metadata.
//			PROPVARIANT    value;
//			value.vt = VT_LPWSTR;
//			value.pwszVal = (LPWSTR)"Metadata Test Image.";
//			hr = piFrameQWriter->SetMetadataByName(L"/xmp/dc:title", &value);
//		}
//		if (SUCCEEDED(hr))
//		{
//			hr = piFrameEncode->WriteSource(
//				static_cast<IWICBitmapSource*> (piFrameDecode),
//				NULL); // Using NULL enables JPEG loss-less encoding.
//		}
//
//		// Commit the frame.
//		if (SUCCEEDED(hr))
//		{
//			hr = piFrameEncode->Commit();
//		}
//
//		if (piFrameDecode)
//		{
//			piFrameDecode->Release();
//		}
//
//		if (piFrameEncode)
//		{
//			piFrameEncode->Release();
//		}
//
//		if (piFrameQReader)
//		{
//			piFrameQReader->Release();
//		}
//
//		if (piFrameQWriter)
//		{
//			piFrameQWriter->Release();
//		}
//	}
//}
//
//if (SUCCEEDED(hr))
//{
//	piEncoder->Commit();
//}
//
//if (SUCCEEDED(hr))
//{
//	piFileStream->Commit(STGC_DEFAULT);
//}
//
//if (piFileStream)
//{
//	piFileStream->Release();
//}
//if (piEncoder)
//{
//	piEncoder->Release();
//}
//
//return 0;