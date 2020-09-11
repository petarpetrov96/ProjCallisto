#include "shaders.h"
Shader::Shader(const char* data,GLenum shaderType) {
    shader=glCreateShader(shaderType);
    glShaderSource(shader,1,(const GLchar**)&data,NULL);
    glCompileShader(shader);
    glGetShaderiv(shader,GL_COMPILE_STATUS,&Result);
    glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&length);
    glGetShaderInfoLog(shader,length,NULL,error);
}
bool Shader::getResult(char* result,int* length0) {
    if(result!=NULL)
        memcpy(result,error,length);
    if(length0!=NULL)
        memcpy(length0,&length,sizeof(int));
    return Result;
}
GLuint Shader::getShader() {
    return shader;
}
Program::Program() {
    program=glCreateProgram();
}
bool Program::attachShader(Shader s) {
    char error[1024];
    int length=0;
    if(s.getResult(error,&length)==false) {
        glDeleteShader(s.getShader());
        return false;
    }
    glAttachShader(program,s.getShader());
    shaders[shaderl++]=s.getShader();
    return true;
}
void Program::create() {
    glLinkProgram(program);
    for(int i=0;i<shaderl;i++)
        glDeleteShader(shaders[i]);
}
void Program::use() {
    glUseProgram(program);
}
GLint Program::getUniformLocation(const GLchar* name) {
    return glGetUniformLocation(program,name);
}
void Program::destroy() {
    glDeleteProgram(program);
}
