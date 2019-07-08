#include <jni.h>
#include<stdio.h>
#include <GLES2/gl2.h>
#include <cmath>
#include<stdlib.h>
//#include <vector>
#include<stdlib.h>
#include <string.h>
#include <EGL/egl.h>
#include <fcntl.h>
#include <GLES2/gl2.h>
#ifdef __cplusplus
extern "C" {
#endif
#include<unistd.h>
static GLuint loadShader (GLenum shader_type, const char *const source);
void readyuv420(int num, unsigned char *bufout, int w, int h);


GLuint program;
unsigned char *buffer[6];
GLint vPosition;
GLuint cubeTex[6] ;

float mVerticesData[] =  { 0.5f, 0.5f, 0.0f,
                           -0.5f, 0.5f, 0.0f,
                           0.0f, -0.5f, 0.0f};

GLfloat vVertices[] = { -1.0f,  1.0f, 0.0f,  // Position 0
                        0.0f,  0.0f,        // TexCoord 0
                        -1.0f, 1.0f/3, 0.0f,  // Position 1
                        0.0f,  1.0f,        // TexCoord 1
                        0.0f,  1.0f/3, 0.0f,  // Position 2
                        1.0f,  1.0f,        // TexCoord 2
                        0.0f,  1.0f, 0.0f,  // Position 3
                        1.0f,  0.0f,         // TexCoord 3

                        0.0f,  1.0f, 0.0f,  // Position 0
                        0.0f,  0.0f,        // TexCoord 0
                        0.0f,  1.0f/3, 0.0f,  // Position 1
                        0.0f,  1.0f,        // TexCoord 1
                        1.0f,  1.0f/3, 0.0f,  // Position 2
                        1.0f,  1.0f,        // TexCoord 2
                        1.0f,  1.0f, 0.0f,  // Position 3
                        1.0f,  0.0f,         // TexCoord 3

                        -1.0f, 1.0f/3, 0.0f,  // Position 0
                        0.0f,  0.0f,        // TexCoord 0
                        -1.0f,  -1.0f/3, 0.0f,  // Position 1
                        0.0f,  1.0f,        // TexCoord 1
                        0.0f,  -1.0f/3, 0.0f,  // Position 2
                        1.0f,  1.0f,        // TexCoord 2
                        0.0f,  1.0f/3, 0.0f,  // Position 3
                        1.0f,  0.0f,         // TexCoord 3

                        0.0f,  1.0f/3, 0.0f,  // Position 0
                        0.0f,  0.0f,        // TexCoord 0
                        0.0f,  -1.0f/3, 0.0f,  // Position 1
                        0.0f,  1.0f,        // TexCoord 1
                        1.0f,  -1.0f/3, 0.0f,  // Position 2
                        1.0f,  1.0f,        // TexCoord 2
                        1.0f,  1.0f/3, 0.0f,  // Position 3
                        1.0f,  0.0f,         // TexCoord 3

                        -1.0f, -1.0f/3, 0.0f,  // Position 0
                        0.0f,  0.0f,        // TexCoord 0
                        -1.0f,  -1.0f, 0.0f,  // Position 1
                        0.0f,  1.0f,        // TexCoord 1
                        0.0f,  -1.0, 0.0f,  // Position 2
                        1.0f,  1.0f,        // TexCoord 2
                        0.0f,  -1.0f/3, 0.0f,  // Position 3
                        1.0f,  0.0f,         // TexCoord 3

                        0.0f,  -1.0f/3, 0.0f,  // Position 0
                        0.0f,  0.0f,        // TexCoord 0
                        0.0f,  -1.0f, 0.0f,  // Position 1
                        0.0f,  1.0f,        // TexCoord 1
                        1.0f,  -1, 0.0f,  // Position 2
                        1.0f,  1.0f,        // TexCoord 2
                        1.0f,  -1.0f/3, 0.0f,  // Position 3
                        1.0f,  0.0f         // TexCoord 3
};

//点的索引，因为是画三角形，所以需要分两次画成矩形，所以，0，1，2为vVertices里面的对应索引，，，分两次画三角形成为矩形
GLushort indices[][6] = { 0, 1, 2, 0, 2, 3,
                          4, 5, 6, 4, 6, 7,
                          8, 9, 10, 8, 10, 11,
                          12, 13, 14, 12, 14, 15,
                          16, 17, 18, 16, 18, 19,
                          20, 21, 22, 20, 22, 23 };



#define NUM_W 8
#define NUM_H 8

unsigned char num0[11][NUM_H][NUM_W]= {
        0,0,0,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,0,0,0,0,

        0,0,0,0,1,0,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,1,1,1,0,0,
        0,0,0,0,0,0,0,0,

        0,0,0,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,0,0,1,0,0,
        0,0,0,1,1,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,1,1,1,0,0,
        0,0,0,0,0,0,0,0,


        0,0,0,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,0,0,1,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,0,0,0,0,

        0,0,0,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,1,0,0,0,
        0,0,1,0,1,0,0,0,
        0,0,1,1,1,1,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,0,0,0,0,

        0,0,0,1,1,1,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,0,0,0,0,

        0,0,0,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,0,0,0,0,

        0,0,1,1,1,1,0,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,1,0,0,0,0,
        0,0,0,1,0,0,0,0,
        0,0,0,1,0,0,0,0,
        0,0,0,1,0,0,0,0,
        0,0,0,0,0,0,0,0,

        0,0,0,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,0,0,0,0,

        0,0,0,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,1,0,0,
        0,0,0,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,0,0,0,0,

        0,0,1,1,1,1,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,1,1,1,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,1,1,1,0,0,
        0,0,0,0,0,0,0,0
/*
        0,0,0,1,1,1,0,0,
        0,0,1,0,0,0,1,0,
        0,0,0,0,0,0,1,0,
        0,0,0,1,1,1,1,0,
        0,0,1,0,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,0,1,1,1,0,1,
        0,0,0,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,1,1,1,0,0,0,
        0,0,1,0,0,1,0,0,
        0,0,1,0,0,1,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,0,0,0,0  */

};


int _backingWidth = 320;
int _backingHeight = 240;
int width=_backingWidth;
int height=_backingHeight;
GLuint loadShader(GLenum shaderType, const char* shaderSource)
{
    GLuint shader = glCreateShader(shaderType);
    if (shader)
    {
        glShaderSource(shader, 1, &shaderSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen)
            {
                char * buf = (char*) malloc(infoLen);
                if (buf)
                {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

const GLchar v_shader[1024] =
        /*attribute 变量只用于顶点着色器中，用来存储顶点着色器中每个顶点的输入（per-vertex inputs）。attribute
         * 通常用来存储位置坐标、法向量、纹理坐标和颜色等。注意 attribute 是用来存储单个顶点的信息。*/
        "attribute vec4 a_position;    \n" /*由应用程序传输给顶点着色器的逐顶点的数据*/
        "attribute vec2 a_texCoord;   \n"
        "varying vec2 v_TexCoordinate;     \n" /*由顶点着色器传输给片段着色器中的插值数据,顶点着色器和片段着色器中都会有 varying 变量的声明，
 * 由于 varying 是顶点着色器的输出且是片段着色器的输入，所以两处声明必须一致*/
        "void main()                  \n"
        "{                            \n"
        "   gl_Position = a_position;  \n"// gl_Position是vertex shader的内建变量，gl_Position中的顶点值最终输出到渲染管线中
        "   v_TexCoordinate = a_texCoord;  \n"//将a_texCoord的值通过v_TexCoordinate传递给fragment shader
        "}                            \n";
const GLchar p_shader1[1024] =
        "precision mediump float;					  \n"
        "uniform sampler2D u_Texture;                 \n" /*在图元处理过程中其值保持不变，由应用程序传输给着色器*/
        "uniform sampler2D u_Texture1;                 \n"
        "varying vec2 v_TexCoordinate;                \n"
        "void main()                                  \n"
        "{                                            \n"
        "    gl_FragColor = texture2D(u_Texture, v_TexCoordinate); \n"
        "}                                            \n";
/*gl_Position，内建变量，顶点着色器的输出值，而且是必须要赋值的变量。对 gl_Position 设置的值会成为该顶点着色器的输出。
gl_FragColor，和 gl_Position 一样，也是内建变量，对应片段的色值。*/
#define W 320
#define H 240

unsigned char vbuf[W*H];
unsigned char ubuf[W*H];

//unsigned char *addr = NULL;
int uoff = W*H;
int voff = W*H+(W*H/4);
//char *yuvfile[6]={"1.yuv", "2.yuv", "3.yuv", "4.yuv", "5.yuv", "6.yuv"};
char *yuvfile[6]={"/sdcard/1.yuv","/sdcard/2.yuv","/sdcard/3.yuv","/sdcard/4.yuv","/sdcard/5.yuv","/sdcard/6.yuv"};
int framecnt[6]={0,0,0,0,0,0};
unsigned char addr[W*H*6/4];
//yuv420 to RGB
void readyuv420(int num, unsigned char *bufout, int w, int h)
{

    //   unsigned char *bmp = NULL;
    //   addr = (unsigned char*)malloc(w*h*6/4);
    //  memset(addr, 0x0, w*h*6/4);
    //  memset(vbuf, 0x0, w*h);
    //  memset(ubuf, 0x0, w*h);

//YYYY YYYY YYYY YYYY YYYY YYYY ...UUUU.... VVVV ...
    int i = 0;
    FILE* fp;
    fp = fopen(yuvfile[num],"r+");
    if(fp == NULL)
    {
        //   exit(0);
    }

    int off;
    long total = 0;
    fseek(fp, 0, SEEK_END);
    total = ftell(fp);
    off = framecnt[num]*W*H*6/4;

    if(off > total)
    {
        off=0;
        framecnt[num]=0;
    }
    fseek(fp, off, SEEK_SET);

    size_t ret;
    ret = fread(addr, 1, size_t(w*h*6/4), fp);
    fclose(fp);
    framecnt[num]+=1;
    printf("REt %ld\n", ret);

    int cnt = 0;

    for(i = 0; i < w*h; i+=1)
    {
        //get u/v index while loop in Y plain
        if((i/w)%2 == 0)
        {
            cnt = (w/2)*((i/w)/2)+(i%w)/2;
        }
        else
        {
            cnt = (w/2)*(((i/w)/2)-1)+(i%w)/2;
        }

        int R =  (int)((298*addr[i]+408*(addr[voff+cnt]-128)+128)>>8);
        int G =  (int)((298*addr[i]-100*(addr[uoff+cnt]-128)-208*(addr[voff+cnt]-128)+128)>>8);
        int B =  (int)((298*addr[i]+516*(addr[uoff+cnt]-128)+128)>>8);


        if (R < 0){ R = 0; }
        if (G < 0){ G = 0; }
        if (B < 0){ B = 0; }
        if (R > 255) { R = 255; }
        if (G > 255) { G = 255; }
        if (B > 255) { B = 255; }

        bufout[i*4+0] = (unsigned char ) (R);//0xff;//addr[i];
        bufout[i*4+1] = (unsigned char ) (G);//vbuf[i];
        bufout[i*4+2] = (unsigned char ) (B);//ubuf[i];
        // bufout[i*4+3] = 0xff;


    }

    //   free(addr);
    //  return bmp;

}

GLuint createProgram(const char* vertexSource, const char * fragmentSource)
{
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader)
    {
        return 0;
    }
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fragmentShader)
    {
        return 0;
    }
    GLuint program = glCreateProgram();
    if (program)
    {
        glAttachShader(program , vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program , GL_LINK_STATUS, &linkStatus);
        if( linkStatus != GL_TRUE)
        {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength)
            {
                char* buf = (char*) malloc(bufLength);
                if (buf)
                {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

void display_num(unsigned char *buf,int w, int h, unsigned char num, unsigned int color)
{
    int i;
    int j;
    int w1;
    int h1;
    w1 = w/NUM_W;
    h1 = h/NUM_H;

    for(i = 0; i < h; i++)
    {
        for( j = 0; j < w*4; j+=4)
        {

            if(num < 10) {
                buf[i * w * 4 + j] = num0[num][i / h1][(j / 4) / w1] * ((color >> 0) & 0xff);//R
                buf[i * w * 4 + j + 1] = num0[num][i / h1][(j / 4) / w1] * ((color >> 8) & 0xff);//G
                buf[i * w * 4 + j + 2] =
                        num0[num][i / h1][(j / 4) / w1] * ((color >> 16) & 0xff);//B
                buf[i * w * 4 + j + 3] =
                        num0[num][i / h1][(j / 4) / w1] * ((color >> 24) & 0xff);//A
            }
            else
            {
                buf[i * w * 4 + j] = num0[10][i / h1][(j / 4) / w1] * ((color >> 0) & 0xff);//R
                buf[i * w * 4 + j + 1] = num0[10][i / h1][(j / 4) / w1] * ((color >> 8) & 0xff);//G
                buf[i * w * 4 + j + 2] =
                        num0[10][i / h1][(j / 4) / w1] * ((color >> 16) & 0xff);//B
                buf[i * w * 4 + j + 3] =
                        num0[10][i / h1][(j / 4) / w1] * ((color >> 24) & 0xff);//A
            }
        }
    }
}
void on_surface_created() {

    // GLuint vertex_shader = loadShader(GL_VERTEX_SHADER, v_shader);
    // GLuint fragment_shader = loadShader(GL_FRAGMENT_SHADER, p_shader);
    ///  glAttachShader(program, vertex_shader);
    //  glAttachShader(program, fragment_shader);

    program = createProgram(v_shader, p_shader1);

    glLinkProgram(program);
    glEnable(GL_BLEND);

    int i;
    int j;
    static int tmp;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    for (i = 0; i < 6; i++)
    {
        buffer[i] = new unsigned char[_backingHeight * _backingWidth * 4];
        memset(buffer[i],0x00, width*height*4);

    }

    glGenTextures(6, cubeTex);
    for(i = 0; i < 6; i++) {
        glBindTexture(GL_TEXTURE_2D, cubeTex[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer[i]);
    }
    //使用此函数之后，buffer[0]即可释放，即便修改buffer内容也不会影响显示内容.


}


void set_color(unsigned char *buf, unsigned int color1, unsigned int color2)
{

    int i;
    int j;
    int tmp;
    memset(buf,0xff, width*height*4);
    for (i = 0; i < height; i++) {
        if (i < height / 3) {

            for (tmp = (width * 2 - i * 4); tmp < (width * 2 + i * 4); tmp += 4) {
                for (j = 0; j < 3; j++) {
                    // if (cnt % 3
                    buf[i * width * 4 + tmp + 0] = color1&0xff;
                    buf[i * width * 4 + tmp + 1] = (color1>>8)&0xff;
                    buf[i * width * 4 + tmp + 2] = (color1>>16)&0xff;
                    buf[i * width * 4 + tmp + 3] = (color1>>24)&0xff;
                }
            }
        } else {
            for (tmp = (width * 2 - (height / 7) * 4);
                 tmp < (width * 2 + (height / 7) * 4); tmp += 4) {
                for (j = 0; j < 3; j++) {
                    buf[i * width * 4 + tmp + 0] = color2&0xff;
                    buf[i * width * 4 + tmp + 1] = (color2>>8)&0xff;
                    buf[i * width * 4 + tmp + 2] = (color2>>16)&0xff;
                    buf[i * width * 4 + tmp + 3] = (color2>>24)&0xff;
                }
            }
        }
    }
}

void on_draw_frame() {

    glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glUseProgram(program);


    //  glViewport(0, 0, width, height); //Maybe should not keep this in on_draw_frame
    glClear(GL_COLOR_BUFFER_BIT);
    //glActiveTexture(GL_TEXTURE0);
    int mTextureUniformHandle;
    int mTextureUniformHandle1;
    int mTextureCoordinateHandle;
    //两个顶点 一个fragement
    vPosition = glGetAttribLocation(program, "a_position");
    mTextureCoordinateHandle = glGetAttribLocation(program, "a_texCoord");

    //如果两个shader中都声明了同一个uniform变量，那么他们值是相同的。CPU通过变量名获取uniform变量在GPU的地址，
    // 使用glUniform1i命令向其赋值。通常CPU通过uniform常量向GPU传递纹理等数据块。
    //  mTextureUniformHandle = glGetUniformLocation(program, "u_Texture");
    //   glUniform1i(mTextureUniformHandle, 0);


    //向顶点shader赋值模型的顶点数据
    //shader会依次对每个顶点通过一系列矩阵操作，确定其最终位置，赋值给顶点shader内建的输出变量gl_position供渲染管线下一步使用。
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vVertices);

    // Load the texture coordinate
    glVertexAttribPointer(mTextureCoordinateHandle, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                          &vVertices[3]);

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(mTextureCoordinateHandle);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTex[0]);
    static unsigned char  cnt = 1;
    static int tmp = 0;
    unsigned char i = 0;
    unsigned int num_color = 0xffffffff;
    // if(tmp % 41 == 0)
    {
        //   if(tmp%82 == 0)
        {
            num_color = num_color&((0xff<<(tmp%25)));
        }
        for (i = 0; i < 6; i++) {
            //  display_num(buffer[i], width, height, cnt+i>9?cnt+i-10:cnt+i, num_color);
            //display_num(buffer[i], width, height, 10,num_color);
            readyuv420(i, buffer[i], W, H);
        }
    }

    cnt++;
    tmp++;
    if(cnt >= 10)
    {
        cnt = 0;
    }


    for(i = 0; i < 6; i++)
    {

        glActiveTexture ( GL_TEXTURE0 ); //TODO GL_TEXTURE0??
        glBindTexture(GL_TEXTURE_2D, cubeTex[i]);
        //glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,buffer[1]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer[i]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices[i]);

    }


}

void on_surface_changed(int width, int height) {
    int w;
    int h;
    w = width;
    h = height;
    //surface大小变化时，视口变换
    glViewport(0, 0, w, h);
}

static GLuint loadShader1 (GLenum shader_type, const GLchar * source)
{
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    return shader;
}
JNIEXPORT void JNICALL Java_com_example_myapplication_GLESWrapper_on_1surface_1created
        (JNIEnv * env, jclass cls) {
    on_surface_created();
}

JNIEXPORT void JNICALL Java_com_example_myapplication_GLESWrapper_on_1surface_1changed(JNIEnv * env, jclass cls, jint width, jint height) {
    on_surface_changed(width, height);
}

JNIEXPORT void JNICALL Java_com_example_myapplication_GLESWrapper_on_1draw_1frame
        (JNIEnv * env, jclass cls) {
    on_draw_frame();
}




#ifdef __cplusplus
}
#endif