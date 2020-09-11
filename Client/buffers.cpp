#include "buffers.h"
Buffer::Buffer() {
    glGenBuffers(1,&buffer);
}
void Buffer::saveData(void* data,int len) {
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glBufferData(GL_ARRAY_BUFFER,len*sizeof(GLfloat),data,GL_DYNAMIC_DRAW);
}
void Buffer::use(int location,int size) {
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glVertexAttribPointer(
        location,
        size,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );
}
void Buffer::free(int location) {
    glDisableVertexAttribArray(location);
}
void Buffer::destroy() {
    glDeleteBuffers(1,&buffer);
}

