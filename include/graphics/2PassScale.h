#ifndef _2_PASS_SCALE_H_
#define _2_PASS_SCALE_H_

//#include <math.h>

#include "Filters.h"
#include "HareGraphicsType.h"
namespace hare
{

	typedef struct
	{
	double *Weights;  // Normalized weights of neighboring pixels
	int Left,Right;   // Bounds of source pixels window
	} ContributionType;  // Contirbution information for a single pixel

	typedef struct
	{
	ContributionType *ContribRow; // Row (or column) of contribution weights
	uint32 WindowSize,              // Filter window size (of affecting source pixels)
			LineLength;              // Length of line (no. or rows / cols)
	} LineContribType;               // Contribution information for an entire line (row or column)

	typedef bool (*ProgressAnbAbortCallBack)(uint8 bPercentComplete);

	template <class FilterClass, class ColorType> //仅支持 argb 和 rgb
	class C2PassScale
	{
	public:

		C2PassScale (ProgressAnbAbortCallBack callback = NULL) :
			m_Callback (callback) {}

		virtual ~C2PassScale() {}

		ColorType * AllocAndScale (
			ColorType   *pOrigImage,
			uint32        uOrigWidth,
			uint32        uOrigHeight,
			uint32        uNewWidth,
			uint32        uNewHeight);

		ColorType * Scale (
			ColorType   *pOrigImage,
			uint32        uOrigWidth,
			uint32        uOrigHeight,
			ColorType   *pDstImage,
			uint32        uNewWidth,
			uint32        uNewHeight);

	private:

		ProgressAnbAbortCallBack    m_Callback;
		bool                        m_bCanceled;

		LineContribType *AllocContributions (   uint32 uLineLength,
												uint32 uWindowSize);

		void FreeContributions (LineContribType * p);

		LineContribType *CalcContributions (    uint32    uLineSize,
												uint32    uSrcSize,
												double  dScale);

		void ScaleRow ( ColorType           *pSrc,
						uint32                uSrcWidth,
						ColorType           *pRes,
						uint32                uResWidth,
						uint32                uRow,
						LineContribType    *Contrib);

		void HorizScale (   ColorType           *pSrc,
							uint32                uSrcWidth,
							uint32                uSrcHeight,
							ColorType           *pDst,
							uint32                uResWidth,
							uint32                uResHeight);

		void ScaleCol ( ColorType           *pSrc,
						uint32                uSrcWidth,
						ColorType           *pRes,
						uint32                uResWidth,
						uint32                uResHeight,
						uint32                uCol,
						LineContribType    *Contrib);

		void VertScale (    ColorType           *pSrc,
							uint32                uSrcWidth,
							uint32                uSrcHeight,
							ColorType           *pDst,
							uint32                uResWidth,
							uint32                uResHeight);
	};

	template <class FilterClass, class ColorType>
	LineContribType *
	C2PassScale<FilterClass, ColorType>::
	AllocContributions (uint32 uLineLength, uint32 uWindowSize)
	{
		LineContribType *res = new LineContribType;
			// Init structure header
		res->WindowSize = uWindowSize;
		res->LineLength = uLineLength;
			// Allocate list of contributions
		res->ContribRow = new ContributionType[uLineLength];
		for (uint32 u = 0 ; u < uLineLength ; u++)
		{
			// Allocate contributions for every pixel
			res->ContribRow[u].Weights = new double[uWindowSize];
		}
		return res;
	}

	template <class FilterClass, class ColorType>
	void
	C2PassScale<FilterClass, ColorType>::
	FreeContributions (LineContribType * p)
	{
		for (uint32 u = 0; u < p->LineLength; u++)
		{
			// Free contribs for every pixel
			delete [] p->ContribRow[u].Weights;
		}
		delete [] p->ContribRow;    // Free list of pixels contribs
		delete p;                   // Free contribs header
	}

	template <class FilterClass, class ColorType>
	LineContribType *
	C2PassScale<FilterClass, ColorType>::
	CalcContributions (uint32 uLineSize, uint32 uSrcSize, double dScale)
	{
		FilterClass CurFilter;

		double dWidth;
		double dFScale = 1.0;
		double dFilterWidth = CurFilter.GetWidth();

		if (dScale < 1.0)
		{    // Minification
			dWidth = dFilterWidth / dScale;
			dFScale = dScale;
		}
		else
		{    // Magnification
			dWidth= dFilterWidth;
		}

		// Window size is the number of sampled pixels
		int iWindowSize = 2 * (int)ceil(dWidth) + 1;

		// Allocate a new line contributions strucutre
		LineContribType *res = AllocContributions (uLineSize, iWindowSize);

		for (uint32 u = 0; u < uLineSize; u++)
		{   // Scan through line of contributions
			double dCenter = (double)u / dScale;   // Reverse mapping
			// Find the significant edge points that affect the pixel
			int iLeft = max (0, (int)floor (dCenter - dWidth));
			res->ContribRow[u].Left = iLeft;
			int iRight = min ((int)ceil (dCenter + dWidth), int(uSrcSize) - 1);
			res->ContribRow[u].Right = iRight;

			// Cut edge points to fit in filter window in case of spill-off
			if (iRight - iLeft + 1 > iWindowSize)
			{
				if (iLeft < (int(uSrcSize) - 1 / 2))
				{
					iLeft++;
				}
				else
				{
					iRight--;
				}
			}
			double dTotalWeight = 0.0;  // Zero sum of weights
			for (int iSrc = iLeft; iSrc <= iRight; iSrc++)
			{   // Calculate weights
				dTotalWeight += (res->ContribRow[u].Weights[iSrc-iLeft] =
									dFScale * CurFilter.Filter (dFScale * (dCenter - (double)iSrc)));
			}
			assert (dTotalWeight >= 0.0);   // An error in the filter function can cause this
			if (dTotalWeight > 0.0)
			{   // Normalize weight of neighbouring points
				for (int iSrc = iLeft; iSrc <= iRight; iSrc++)
				{   // Normalize point
					res->ContribRow[u].Weights[iSrc-iLeft] /= dTotalWeight;
				}
			}
	}
	return res;
	}


	template <class FilterClass, class ColorType>
	void
	C2PassScale<FilterClass, ColorType>::
	ScaleRow (  ColorType           *pSrc,
				uint32                uSrcWidth,
				ColorType           *pRes,
				uint32                uResWidth,
				uint32                uRow,
				LineContribType    *Contrib)
	{
		ColorType *pSrcRow = &(pSrc[uRow * uSrcWidth]);
		ColorType *pDstRow = &(pRes[uRow * uResWidth]);


		for (uint32 x = 0; x < uResWidth; x++)
		{   // Loop through row
			HarePixelType<HPF_A8R8G8B8> clr = {0,0,0,0};
			int iLeft = Contrib->ContribRow[x].Left;    // Retrieve left boundries
			int iRight = Contrib->ContribRow[x].Right;  // Retrieve right boundries
			for (int i = iLeft; i <= iRight; i++)
			{
				HarePixelType<HPF_A8R8G8B8> tmp;
				ConvertPixelFormat(pSrcRow[i], tmp);
				//带线性插值的算法
				//clr.r += (Byte)(Contrib->ContribRow[x].Weights[i-iLeft] * (float)tmp.r);
				//clr.g += (Byte)(Contrib->ContribRow[x].Weights[i-iLeft] * (float)tmp.g);
				//clr.b += (Byte)(Contrib->ContribRow[x].Weights[i-iLeft] * (float)tmp.b);
				//clr.a += (Byte)(Contrib->ContribRow[x].Weights[i-iLeft] * (float)tmp.a);
				//不带线性插值
				clr.a = tmp.a;
				clr.r = tmp.r;
				clr.g = tmp.g;
				clr.b = tmp.b;
			}
			//pDstRow[x] = RGB(r,g,b); // Place result in destination pixel
			ConvertPixelFormat(clr, pDstRow[x]);
		}
	}

	template <class FilterClass, class ColorType>
	void
	C2PassScale<FilterClass, ColorType>::
	HorizScale (    ColorType           *pSrc,
					uint32                uSrcWidth,
					uint32                uSrcHeight,
					ColorType           *pDst,
					uint32                uResWidth,
					uint32                uResHeight)
	{
		printf ("Performing horizontal scaling...\n");
		if (uResWidth == uSrcWidth)
		{   // No scaling required, just copy
			memcpy (pDst, pSrc, sizeof (ColorType) * uSrcHeight * uSrcWidth);
		}
		// Allocate and calculate the contributions
		LineContribType * Contrib = CalcContributions (uResWidth, uSrcWidth, double(uResWidth) / double(uSrcWidth));
		for (uint32 u = 0; u < uResHeight; u++)
		{   // Step through rows
			if (NULL != m_Callback)
			{
				//
				// Progress and report callback supplied
				//
				if (!m_Callback (uint8(double(u) / double (uResHeight) * 50.0)))
				{
					//
					// User wished to abort now
					//
					m_bCanceled = true;
					FreeContributions (Contrib);  // Free contributions structure
					return;
				}
			}

			ScaleRow (  pSrc,
						uSrcWidth,
						pDst,
						uResWidth,
						u,
						Contrib);    // Scale each row
		}
		FreeContributions (Contrib);  // Free contributions structure
	}

	template <class FilterClass, class ColorType>
	void
	C2PassScale<FilterClass, ColorType>::
	ScaleCol (  ColorType           *pSrc,
				uint32                uSrcWidth,
				ColorType           *pRes,
				uint32                uResWidth,
				uint32                uResHeight,
				uint32                uCol,
				LineContribType    *Contrib)
	{
		for (uint32 y = 0; y < uResHeight; y++)
		{    // Loop through column
			HarePixelType<HPF_A8R8G8B8> clr = {0,0,0,0};
			int iLeft = Contrib->ContribRow[y].Left;    // Retrieve left boundries
			int iRight = Contrib->ContribRow[y].Right;  // Retrieve right boundries
			for (int i = iLeft; i <= iRight; i++)
			{   // Scan between boundries
				// Accumulate weighted effect of each neighboring pixel
				ColorType pCurSrc = pSrc[i * uSrcWidth + uCol];
				HarePixelType<HPF_A8R8G8B8> tmp;
				ConvertPixelFormat(pCurSrc, tmp);
				//clr.r += (Byte)(Contrib->ContribRow[y].Weights[i-iLeft] * (float)tmp.r);
				//clr.g += (Byte)(Contrib->ContribRow[y].Weights[i-iLeft] * (float)tmp.g);
				//clr.b += (Byte)(Contrib->ContribRow[y].Weights[i-iLeft] * (float)tmp.b);
				//clr.a += (Byte)(Contrib->ContribRow[y].Weights[i-iLeft] * (float)tmp.a);
				clr.a = tmp.a;
				clr.r = tmp.r;
				clr.g = tmp.g;
				clr.b = tmp.b;
			}
			ConvertPixelFormat(clr, pRes[y * uResWidth + uCol]);;   // Place result in destination pixel
		}
	}


	template <class FilterClass, class ColorType>
	void
	C2PassScale<FilterClass, ColorType>::
	VertScale ( ColorType           *pSrc,
				uint32                uSrcWidth,
				uint32                uSrcHeight,
				ColorType           *pDst,
				uint32                uResWidth,
				uint32                uResHeight)
	{
		printf ("Performing vertical scaling...");

		if (uSrcHeight == uResHeight)
		{   // No scaling required, just copy
			memcpy (pDst, pSrc, sizeof (ColorType) * uSrcHeight * uSrcWidth);
		}
		// Allocate and calculate the contributions
		LineContribType * Contrib = CalcContributions (uResHeight, uSrcHeight, double(uResHeight) / double(uSrcHeight));
		for (uint32 u = 0; u < uResWidth; u++)
		{   // Step through columns
			if (NULL != m_Callback)
			{
				//
				// Progress and report callback supplied
				//
				if (!m_Callback (uint8(double(u) / double (uResWidth) * 50.0) + 50))
				{
					//
					// User wished to abort now
					//
					m_bCanceled = true;
					FreeContributions (Contrib);  // Free contributions structure
					return;
				}
			}
			ScaleCol (  pSrc,
						uSrcWidth,
						pDst,
						uResWidth,
						uResHeight,
						u,
						Contrib);   // Scale each column
		}
		FreeContributions (Contrib);     // Free contributions structure
	}

	template <class FilterClass, class ColorType>
	ColorType *
	C2PassScale<FilterClass, ColorType>::
	AllocAndScale (
		ColorType   *pOrigImage,
		uint32        uOrigWidth,
		uint32        uOrigHeight,
		uint32        uNewWidth,
		uint32        uNewHeight)
	{
		// Scale source image horizontally into temporary image
		m_bCanceled = false;
		ColorType *pTemp = new ColorType [uNewWidth * uOrigHeight];
		HorizScale (pOrigImage,
					uOrigWidth,
					uOrigHeight,
					pTemp,
					uNewWidth,
					uOrigHeight);
		if (m_bCanceled)
		{
			delete [] pTemp;
			return NULL;
		}
		// Scale temporary image vertically into result image
		ColorType *pRes = new ColorType [uNewWidth * uNewHeight];
		VertScale ( pTemp,
					uNewWidth,
					uOrigHeight,
					pRes,
					uNewWidth,
					uNewHeight);
		if (m_bCanceled)
		{
			delete [] pTemp;
			delete [] pRes;
			return NULL;
		}
		delete [] pTemp;
		return pRes;
	}

	template <class FilterClass, class ColorType>
	ColorType *
	C2PassScale<FilterClass, ColorType>::
	Scale (
		ColorType   *pOrigImage,
		uint32        uOrigWidth,
		uint32        uOrigHeight,
		ColorType   *pDstImage,
		uint32        uNewWidth,
		uint32        uNewHeight)
	{
		// Scale source image horizontally into temporary image
		m_bCanceled = false;
		ColorType *pTemp = new ColorType [uNewWidth * uOrigHeight];
		HorizScale (pOrigImage,
					uOrigWidth,
					uOrigHeight,
					pTemp,
					uNewWidth,
					uOrigHeight);
		if (m_bCanceled)
		{
			delete [] pTemp;
			return NULL;
		}

		// Scale temporary image vertically into result image
		VertScale ( pTemp,
					uNewWidth,
					uOrigHeight,
					pDstImage,
					uNewWidth,
					uNewHeight);
		delete [] pTemp;
		if (m_bCanceled)
		{
			return NULL;
		}
		return pDstImage;
	}

}
#endif //   _2_PASS_SCALE_H_
