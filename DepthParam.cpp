#include "DepthParam.h"

	void DepthParam::setParam( const xn::DepthGenerator &depth )
	{
		depth.GetIntProperty ("ZPD", focalLength );
		depth.GetRealProperty ("ZPPS", pixelSize );

	}
	void DepthParam::setParam( const xn::DepthMetaData &depthMD )
	{
		uSize = depthMD.XRes();
		vSize = depthMD.YRes();
		uCenter = uSize / 2;
		vCenter = vSize / 2;

	}
	bool DepthParam::depth2point( const XnDepthPixel* depthMap, XnPoint3D *points )
	{
		double P2R;
		
		double dist;
       
		for (int v = 0; v < 480; ++v)
		{
			for (int u = 0; u < 640; ++u)
			{
				dist = (*depthMap)*0.001;
				points->X = dist * ( u - 320 )/focalLength;
				points->Y = dist * ( v - 240 )/focalLength;
				points->Z = dist;
				++depthMap;
				++points;
			}
		}
		return true;
	}

