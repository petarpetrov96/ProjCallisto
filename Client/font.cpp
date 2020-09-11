#include "font.h"
int fontsize[12]={65,39,58,60,67,57,64,58,62,65,30,20};
int fontstart[12]={560,0,40,100,160,230,290,360,420,490,630,660};
float convertX(int x) {
    float org=2048;
    if(x<org/2)
        return -(org/2.f-x)/(org/2.f);
    return (x-org/2.f)/(org/2.f);
}
float convertY(int y) {
    float org=1152;
    if(y<org/2)
        return (org/2.f-y)/(org/2.f);
    return -(y-org/2.f)/(org/2.f);
}
void convertCoords(int x,int y,int number,GLfloat* vertex,GLfloat* uvc) {
    GLfloat tr[18]={convertX(x),convertY(y),0.0f,
                    convertX(x+fontsize[number]),convertY(y),0.0f,
                    convertX(x),convertY(y+120),0.0f,
                    convertX(x+fontsize[number]),convertY(y),0.0f,
                    convertX(x),convertY(y+120),0.0f,
                    convertX(x+fontsize[number]),convertY(y+120),0.0f};
    GLfloat uv[12]={fontstart[number]/1024.f,0.0f,
                    (fontstart[number]+fontsize[number])/1024.f,0.0f,
                    fontstart[number]/1024.f,1.0f,
                    (fontstart[number]+fontsize[number])/1024.f,0.0f,
                    fontstart[number]/1024.f,1.0f,
                    (fontstart[number]+fontsize[number])/1024.f,1.0f};
    memcpy(vertex,tr,18*sizeof(GLfloat));
    memcpy(uvc,uv,12*sizeof(GLfloat));
}
Font::Font(const char* filename) {
    t.loadPNG(filename);
    convertCoords(300,495,11,tr,uv);
    u.saveData(uv,12);
    v.saveData(tr,18);
}
void Font::add(int n) {
    if(num==15)
        return;
    convertCoords(300+cursize,495,n,tr+num*18,uv+num*12);
    nums[num++]=n;
    cursize+=fontsize[n];
    convertCoords(300+cursize,495,11,tr+num*18,uv+num*12);
    u.saveData(uv,(num+1)*12);
    v.saveData(tr,(num+1)*18);
}
void Font::rem() {
    if(num>0) {
        num--;
        cursize-=fontsize[nums[num]];
        convertCoords(300+cursize,495,11,tr+num*18,uv+num*12);
        u.saveData(uv,(num+1)*12);
        v.saveData(tr,(num+1)*18);
    }
}
void Font::draw(Program p) {
    t.use(p);
    v.use(0,3);
    u.use(1,2);
    glDrawArrays(GL_TRIANGLES,0,(num+show)*6);
    v.free(0);
    u.free(0);
}
void Font::destroy() {
    t.destroy();
    v.destroy();
    u.destroy();
}
void Font::getIP(char* ip,int* len) {
    if(len!=NULL)
        memcpy(len,&num,sizeof(int));
    for(int i=0;i<num;i++)
        if(nums[i]>=0 && nums[i]<=9)
            *(ip+i)=nums[i]+'0';
        else
            *(ip+i)='.';
    *(ip+num)='\0';
}
Button::Button(const char* filename,int x,int y,int w,int h) {
    bx=x;
    by=y;
    bw=w;
    bh=h;
    t.loadPNG(filename);
    GLfloat tr[18]={convertX(x),convertY(y),0.0f,
                    convertX(x+w),convertY(y),0.0f,
                    convertX(x),convertY(y+h),0.0f,
                    convertX(x+w),convertY(y),0.0f,
                    convertX(x),convertY(y+h),0.0f,
                    convertX(x+w),convertY(y+h),0.0f};
    GLfloat uv1[12]={0.0f,0.0f,
                     1.0f,0.0f,
                     0.0f,0.5f,
                     1.0f,0.0f,
                     0.0f,0.5f,
                     1.0f,0.5f};
    GLfloat uv2[12]={0.0f,0.5f,
                     1.0f,0.5f,
                     0.0f,1.0f,
                     1.0f,0.5f,
                     0.0f,1.0f,
                     1.0f,1.0f};
    v.saveData(tr,18);
    u1.saveData(uv1,12);
    u2.saveData(uv2,12);
}
void Button::setResolution(int w,int h) {
    sw=w;
    sh=h;
}
bool Button::check(int x,int y) {
    int cx=x*(2048.f/sw);
    int cy=y*(1152.f/sh);
    if(cx>bx && cx<bx+bw && cy>by && cy<by+bh) {
        hover=true;
        return true;
    }
    else {
        hover=false;
        return false;
    }
}
void Button::draw(Program p) {
    t.use(p);
    v.use(0,3);
    if(hover)
        u2.use(1,2);
    else
        u1.use(1,2);
    glDrawArrays(GL_TRIANGLES,0,6);
    v.free(0);
    if(hover)
        u2.free(1);
    else
        u1.free(1);
}
void Button::destroy() {
    t.destroy();
    v.destroy();
    u1.destroy();
    u2.destroy();
}
