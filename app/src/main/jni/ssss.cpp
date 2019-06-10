#include <jni.h>
#include<stdio.h>
#include <GLES2/gl2.h>
#include <cmath>
#include<stdlib.h>
//#include <vector>
#include<stdlib.h>
#include <string.h>
#include <EGL/egl.h>
#ifdef __cplusplus
extern "C" {
#endif

static GLuint loadShader (GLenum shader_type, const char *const source);


GLuint program;
unsigned char *buffer[6];
GLint vPosition;
GLuint cubeTex[6] ;

float mVerticesData[] =  { 0.5f, 0.5f, 0.0f,
                          -0.5f, 0.5f, 0.0f,
                           0.0f, -0.5f, 0.0f};

int _backingWidth = 200;
int _backingHeight = 200;
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
        "   gl_Position = a_position;  \n"
        "   v_TexCoordinate = a_texCoord;  \n"
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

const GLchar v_shader1[1024] =
        "attribute vec4 position; \n"
        "uniform mat4 modelViewProjMatrix; \n"
        "void main() { \n"
        "gl_Position = modelViewProjMatrix * position; \n"
        "}  \n";


const GLchar p_shader[1024] =
        "precision mediump float;					  \n"
        "uniform sampler2D u_Texture;                 \n"
        "varying vec4 v_Color;                        \n"
        "varying vec2 v_TexCoordinate;                \n”"
        "void main()                                  \n"
        "{                                            \n"
        "  gl_FragColor = texture2D(u_Texture, v_TexCoordinate); \n"
        "}                                            \n";
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
    int tmp;
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

   for (i = 0; i < 6; i++)
   {
       buffer[i] = new unsigned char[_backingHeight * _backingWidth * 4];
       memset(buffer[i],0xff, width*height*4);
       
   }


    for( i = 0; i < height; i++)
    {
        if(i < height/3) {

            for (tmp = (width * 2 - i * 4); tmp < (width * 2 + i * 4); tmp += 4) {
                buffer[0][i * width * 4 + tmp + 1] = 0x0;
                buffer[0][i * width * 4 + tmp + 2] = 0x0;
            }
        }
        else
        {
            for (tmp = (width * 2 - (height/7) * 4); tmp < (width * 2 + (height/7) * 4); tmp += 4) {
                buffer[0][i * width * 4 + tmp + 1] = 0x0;
                buffer[0][i * width * 4 + tmp + 2] = 0x0;
            }

        }

    }
    for( i = 0; i < height; i++)
    {
        if(i < height/3)
        {
            for (tmp = (width * 2 - i * 4); tmp < (width * 2 + i * 4); tmp += 4) {
                buffer[1][i * width * 4 + tmp + 0] = 0x0;
                buffer[1][i * width * 4 + tmp + 2] = 0x0;
            }
        }
        else
        {
            for (tmp = (width * 2 - (height/7) * 4); tmp < (width * 2 + (height/7) * 4); tmp += 4) {
                buffer[1][i * width * 4 + tmp + 0] = 0x0;
                buffer[1][i * width * 4 + tmp + 2] = 0x0;
            }
        }
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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
    mTextureUniformHandle = glGetUniformLocation(program, "u_Texture");
    // mTextureUniformHandle1 = glGetUniformLocation(program, "u_Texture1");
// Load the vertex position
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vVertices);
    // Load the texture coordinate
    glVertexAttribPointer(mTextureCoordinateHandle, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                          &vVertices[3]);

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(mTextureCoordinateHandle);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTex[0]);
    static int cnt = 1;
    int i = 0;
    for (i = 0; i < 6; i++) {
         //   set_color(buffer[i], 0xffff0000, 0xffff00ff);
        // printf("%x %x\n", (cnt%6)<<(cnt%6*4), cnt%6);

        if(cnt%2 == 0) {
            set_color(buffer[i], 0xffffffff & ((0xff) << ((i%4) * 8)),
                      0xffffffff >> ((i%4) * 8));
        } else {

                set_color(buffer[i], 0xffffffff >> ((cnt%4 ) * 8),
                          0xffffffff & ((0xff) << ((cnt%4) * 8)));
        }
    }
    cnt++;


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