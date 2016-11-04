#pragma once
#include <QOpenGLBuffer>
#include <QString>

using namespace std;

typedef enum TextureFilterSetting{
    TEXTURE_FILTER_MAG_NEAREST,
    TEXTURE_FILTER_MAG_BILINEAR,


    TEXTURE_FILTER_MIN_NEAREST,
    TEXTURE_FILTER_MIN_BILINEAR,
    TEXTURE_FILTER_MIN_NEAREST_MIPMAP,
    TEXTURE_FILTER_MIN_BILINEAR_MIPMAP,
    TEXTURE_FILTER_MIN_TRILINEAR
};

class Texture
{
private:
    bool initialized;

    GLuint textureId;
    GLuint samplerId;

public:
    Texture();
    void load(QString path);
    void setFiltering(int a_tfMagnification, int a_tfMinification);
    void bindTexture(int iTextureUnit);
};

