#include <jni.h>
#include<stdio.h>
#include <GLES2/gl2.h>
#include <cmath>
#include<stdlib.h>
//#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

static GLuint loadShader (GLenum shader_type, const char *const source);


GLuint program;

GLuint vPosition;


float mVerticesData[] =  { 0.5f, 0.5f, 0.0f,
                          -0.5f, 0.5f, 0.0f,
                           0.0f, -0.5f, 0.0f};


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
        "attribute vec4 vPosition;    \n"
        "void main()                  \n"
        "{                            \n"
        "   gl_Position = vPosition;  \n"
        "}                            \n";
const GLchar p_shader[1024] =
        "precision mediump float;					  \n"
        "void main()                                  \n"
        "{                                            \n"
        "  gl_FragColor = vec4 ( 1.0, 0.0, 1.0, 1.0 );\n"
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

    program = createProgram(v_shader, p_shader);
    vPosition = glGetAttribLocation(program, "vPosition");
    glBindAttribLocation(program, 0, "vPosition");
    glLinkProgram(program);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void on_draw_frame() {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0 ,mVerticesData);
    glEnableVertexAttribArray(vPosition);
    glDrawArrays(GL_TRIANGLES, 0, 3);

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