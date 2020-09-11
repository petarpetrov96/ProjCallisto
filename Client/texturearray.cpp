#include "texturearray.h"
TextureArray::TextureArray(int count) {
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY,texture);
    glPixelStorei(GL_UNPACK_ROW_LENGTH,256);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAX_ANISOTROPY_EXT,16);
    glTexImage3D(GL_TEXTURE_2D_ARRAY,0,GL_RGBA,256,256,count,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
}
bool TextureArray::loadPNG(int id,const char* filename) {
    std::vector<unsigned char> image;
    unsigned int width,height;
    if(lodepng::decode(image,width,height,filename)!=0)
        return false;
    glBindTexture(GL_TEXTURE_2D_ARRAY,texture);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,0,0,0,id,256,256,1,GL_RGBA,GL_UNSIGNED_BYTE,&image[0]);
    return true;
}
void TextureArray::use(Program p) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY,texture);
    glUniform1i(p.getUniformLocation("textureSamplerArray"),0);
}
void TextureArray::destroy() {
    glDeleteTextures(1,&texture);
}
