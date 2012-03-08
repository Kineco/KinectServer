#ifndef WebSocketServer_Vector3D_h
#define WebSocketServer_Vector3D_h
#include <cmath>

class Vector3D {

    
public:
    float X;
    float Y;
    float Z;
    
    Vector3D()
    {
        X=0; Y=0; Z=0;
    }
    Vector3D(float x, float y, float z)
    {
        X = x; Y = y; Z = z;
    }
    Vector3D(const Vector3D& v)
    {
        X = v.X; Y = v.Y; Z = v.Z;
    }
    inline Vector3D& operator=(const Vector3D& v)
    {
        X = v.X; Y =v.Y; Z=v.Z; 
        return *this;
    }
    
    inline Vector3D operator+(const Vector3D& v) const 
    {
        return Vector3D(v.X+X, Y+v.Y, Z+v.Z);
    }
    
    inline Vector3D operator-(const Vector3D& v) const 
    {
        return Vector3D(X-v.X, Y- v.Y, Z-v.Z);
    }
    //内積
    inline float operator*(const Vector3D& v) const 
    {
        return X*v.X + Y*v.Y + Z*v.Z;
    }
    
    inline Vector3D operator/(float a) const
    {
        return Vector3D(X/a, Y/a, Z/a);
    }
    
    inline float size()
    {
        return sqrt(X*X + Y*Y + Z*Z);
    }
    
    
    
    inline ~Vector3D()
    {
        
    }
    
    
};


#endif
