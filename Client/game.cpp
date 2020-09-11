#include "game.h"
bool Game::init() {
    //GLFW
    if(!glfwInit()) {
        return false;
    }
    monitor=glfwGetPrimaryMonitor();
    glfwWindowHint(GLFW_SAMPLES,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_ANY_PROFILE);
    window=glfwCreateWindow(1024,576,"Project Callisto",NULL,NULL);
    if(!window) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    //OpenGL
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEdgeFlag(GL_FALSE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA);
    glAlphaFunc(GL_ALWAYS,0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    //GLEW
    glewExperimental=GL_TRUE;
    GLenum err=glewInit();
    if(err!=GLEW_OK) {
        glfwTerminate();
        return false;
    }
    glGenVertexArrays(1,&VertexArrayID);
	glBindVertexArray(VertexArrayID);
    return true;
}
void Game::mainMenu() {
    Program prog;
    char vertex[]="#version 330 core\n\
    layout(location=0) in vec3 vertexPosition_modelspace;\n\
    layout(location=1) in vec2 vertexUV;\n\
    out vec2 UV;\n\
    void main() {\n\
    gl_Position=vec4(vertexPosition_modelspace,1);\n\
    UV=vertexUV;\n\
    }\n";
    char fragment[]="#version 330 core\n\
    in vec2 UV;\n\
    out vec4 color;\n\
    uniform sampler2D textureSampler;\n\
    void main() {\n\
    color=texture(textureSampler,UV).rgba;\n\
    }\n";
    prog.attachShader(Shader(vertex,GL_VERTEX_SHADER));
    prog.attachShader(Shader(fragment,GL_FRAGMENT_SHADER));
    prog.create();
    Buffer v,u;
    GLfloat triangles[]={  -1.0f,  1.0f, 0.0f,
                            1.0f,  1.0f, 0.0f,
                           -1.0f, -1.0f, 0.0f,
                            1.0f,  1.0f, 0.0f,
                           -1.0f, -1.0f, 0.0f,
                            1.0f, -1.0f, 0.0f };
    GLfloat uv[]={0.0f, 0.0f,
                  1.0f, 0.0f,
                  0.0f, 1.0f,
                  1.0f, 0.0f,
                  0.0f, 1.0f,
                  1.0f, 1.0f};
    v.saveData(triangles,18);
    u.saveData(uv,12);
    Font f("font.png");
    Texture t;
    t.loadPNG("test.png");
    Button connect("connect.png",285,620,750,50);
    connect.setResolution(1024,576);
    Button exit("exit.png",1070,620,750,50);
    exit.setResolution(1024,576);
    bool pressed[10]={false};
    bool pressedb=false,pressedp=false;
    time_t start,end;
    start=time(0);
    bool quit=true;
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        prog.use();
        t.use(prog);
        v.use(0,3);
        u.use(1,2);
        glDrawArrays(GL_TRIANGLES,0,6);
        v.free(0);
        u.free(1);
        f.draw(prog);
        connect.draw(prog);
        exit.draw(prog);
        glfwSwapBuffers(window);
        glfwPollEvents();
        for(int i=0;i<=9;i++) {
            if(glfwGetKey(window,48+i)==GLFW_PRESS && pressed[i]==false) {
                f.add(i);
                pressed[i]=true;
            }
            else if(glfwGetKey(window,48+i)==GLFW_RELEASE)
                pressed[i]=false;
        }
        if(glfwGetKey(window,GLFW_KEY_PERIOD)==GLFW_PRESS && pressedp==false) {
            f.add(10);
            pressedp=true;
        }
        else if(glfwGetKey(window,GLFW_KEY_PERIOD)==GLFW_RELEASE)
            pressedp=false;
        if(glfwGetKey(window,GLFW_KEY_BACKSPACE)==GLFW_PRESS && pressedb==false) {
            f.rem();
            pressedb=true;
        }
        else if(glfwGetKey(window,GLFW_KEY_BACKSPACE)==GLFW_RELEASE)
            pressedb=false;
        end=time(0);
        if(difftime(start,end)!=0) {
            f.show=!f.show;
            start=end;
        }
        double xpos,ypos;
        glfwGetCursorPos(window,&xpos,&ypos);
        if(connect.check((int)(xpos),(int)(ypos)) && glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS) {
            quit=false;
            break;
        }
        if(exit.check((int)(xpos),(int)(ypos)) && glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS)
            break;
        Sleep(16);
    }
    f.destroy();
    connect.destroy();
    exit.destroy();
    t.destroy();
    v.destroy();
    u.destroy();
    prog.destroy();
    if(!quit) {
        char ip[15];
        f.getIP(ip,NULL);
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);
        multiplayer(ip);
    }
}
void Game::multiplayer(char* ip) {
    loadBlockInfo();
    if(!TCP.connectTo(ip)) {
        mainMenu();
        return;
    }
    Program prog;
    char vertex[]="#version 330 core\n\
    layout(location=0) in vec3 vertexPosition_modelspace;\n\
    layout(location=1) in vec3 vertexUV;\n\
	layout(location=2) in vec3 vertexNormal;\n\
    out vec3 UV;\n\
	out vec3 coords;\n\
	out vec3 normal;\n\
    uniform mat4 MVP;\n\
    void main() {\n\
    gl_Position=MVP*vec4(vertexPosition_modelspace,1);\n\
	coords=vertexPosition_modelspace;\n\
    UV=vertexUV;\n\
	normal=vertexNormal;\n\
    }\n";
    char fragment[]="#version 330 core\n\
    in vec3 UV;\n\
	in vec3 coords;\n\
	in vec3 normal;\n\
    out vec4 color;\n\
    uniform sampler2DArray textureSamplerArray;\n\
    void main() {\n\
	vec3 normalVector=normal;\n\
	vec3 lightPos={0.0f,128.0f,0.0f};\n\
	vec3 lightVector=normalize(lightPos-coords);\n\
	float d=distance(coords,lightPos);\n\
	vec4 textureColor=texture(textureSamplerArray,UV).rgba;\n\
	float nDotVP;\n\
	nDotVP=max(0.2f,dot(normalVector,lightVector));\n\
	textureColor=textureColor*nDotVP;\n\
	textureColor.a=1.0f;\n\
    color=textureColor;\n\
    }\n";
    prog.attachShader(Shader(vertex,GL_VERTEX_SHADER));
    prog.attachShader(Shader(fragment,GL_FRAGMENT_SHADER));
    prog.create();
    TextureArray t(1);
    t.loadPNG(0,"texture13.png");
    mat4 Projection=perspective(45.0f,16.0f/9.0f,0.1f,300.0f);
    mat4 Model=mat4(1.0f);
    mat4 View;
    mat4 MVP;
    std::queue<int> q;
    int last=0;
    std::thread thr;
    ChunkManager chunk;
    chunk.update(0,0);
    TCP.login("peter_ew","AAAAAAAAAAAAAAAA");
    float x=0.0f,y=64.0f,z=0.0f;
    float rotx=0.0f,roty=0.0f;
    int cur=0,cur0=0;
    int tx,ty,tz;
    float gspeed=0.16f;
    bool gravity=true;
    float jump=1.5f;
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        View=lookAt(vec3(x,y+1.5f,z),vec3(x+5*cos(rotx),y,z+5*sin(rotx)),vec3(0,1,0));
        MVP=Projection*View*Model;
        glUniformMatrix4fv(prog.getUniformLocation("MVP"),1,GL_FALSE,&MVP[0][0]);
        prog.use();
        t.use(prog);
        chunk.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(TCP.isReady() && thr.joinable()) {
            thr.join();
            chunk.finish(last);
        }
        if(TCP.isReady() && (last=chunk.getQueueElement())!=-1) {
            thr=std::thread(&GameTCP::requestChunk,&TCP,chunk.getChunk(last));
        }
        if(glfwGetKey(window,GLFW_KEY_UP)==GLFW_PRESS) {
                x+=0.15f*cos(rotx);
                z+=0.15f*sin(rotx);
        }
        if(glfwGetKey(window,GLFW_KEY_LEFT)==GLFW_PRESS)
            rotx-=0.03f;
        if(glfwGetKey(window,GLFW_KEY_RIGHT)==GLFW_PRESS)
            rotx+=0.03f;
        if(glfwGetKey(window,GLFW_KEY_SPACE)==GLFW_PRESS) {
            y+=0.5f;
            gravity=false;
        }
        else
            gravity=true;
        if(x>=0) tx=x/8;
        else if((int)(x)%8==0) tx=x/8;
        else tx=x/8-1;
        if(z>=0) tz=z/8;
        else if((int)(z)%8==0) tz=z/8;
        else tz=z/8-1;
        cur=chunk.getChunk(tx,tz);
        if(cur!=cur0) {
            chunk.update(tx,tz);
            cur0=cur;
        }
        if(cur!=-1) {
            if(x>=0) tx=(int)(x)%8;
            else if((int)(x)%8==0) tx=0;
            else tx=8+(int)(x)%8;
            ty=(int)(y-gspeed);
            if(z>=0) tz=(int)(z)%8;
            else if((int)(z)%8==0) tz=0;
            else tz=8+(int)(z)%8;
            if(gravity && chunk.getChunk(cur)->getBlock(tx,ty,tz)==0)
                y-=gspeed;
        }
        Sleep(16);
    }
	if(thr.joinable())
		thr.join();
    TCP.disconnect();
    prog.destroy();
    t.destroy();
    chunk.destroy();
}
void Game::exit() {
    glDeleteVertexArrays(1,&VertexArrayID);
    glfwDestroyWindow(window);
    glfwTerminate();
}

