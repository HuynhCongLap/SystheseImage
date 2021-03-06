
//! \file tuto5GL_samplers.cpp

#include "window.h"
#include "vec.h"
#include "mat.h"

#include "program.h"
#include "uniforms.h"

#include "mesh.h"
#include "wavefront.h"

#include "orbiter.h"
#include "image_io.h"

#include <stdio.h>


GLuint program;

GLuint texture;
GLuint sampler;
GLuint sampler_linear;
GLuint sampler_aniso;

GLuint vao;
GLuint vertex_buffer;
GLuint texcoord_buffer;
unsigned int vertex_count;

Orbiter camera;


int init( )
{
    // etape 1 : shaders
    program= read_program("tutos/tuto5GL.glsl");
    program_print_errors(program);

    // etape 2 : charger un mesh, (avec des texcoords), vao + vertex buffer
    Mesh mesh= read_mesh("data/cube.obj");
    if(mesh.vertex_count() == 0)
        return -1;      // gros probleme, pas de sommets...

    vertex_count= mesh.vertex_count();

    camera.lookat(Point(0, 0, 0), 5);

    // vertex format : position + texcoord,
    // cf tuto4GL_normals.cpp
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vertex buffer
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertex_buffer_size(), mesh.vertex_buffer(), GL_STATIC_DRAW);

    // configurer l'attribut position
    GLint position= glGetAttribLocation(program, "position");
    if(position < 0)
        return -1;
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(position);

    // texcoord buffer
    glGenBuffers(1, &texcoord_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.texcoord_buffer_size(), mesh.texcoord_buffer(), GL_STATIC_DRAW);

    // configurer l'attribut texcoord
    GLint texcoord= glGetAttribLocation(program, "texcoord");
    if(texcoord < 0)
        return -1;
    glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(texcoord);

    // nettoyage
    mesh.release();
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // etape 3 : sampler
    glGenSamplers(1, &sampler);

    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

#if 1
    glGenSamplers(1, &sampler_linear);

    glSamplerParameteri(sampler_linear, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_linear, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler_linear, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(sampler_linear, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glGenSamplers(1, &sampler_aniso);

    glSamplerParameteri(sampler_aniso, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_aniso, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler_aniso, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(sampler_aniso, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glSamplerParameterf(sampler_aniso, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
#endif

    // etape 4 : texture
    ImageData image= read_image_data("data/debug2x2red.png");

    GLenum data_format= GL_RGBA;
    GLenum data_type= GL_UNSIGNED_BYTE;
    if(image.channels == 3)
        data_format= GL_RGB;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0,
        GL_RGBA, image.width, image.height, 0,
        data_format, data_type, image.buffer() );

    glGenerateMipmap(GL_TEXTURE_2D);

    // nettoyage
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

    // etat par defaut
    glClearColor(0.2f, 0.2f, 0.2f, 1);
    glClearDepthf(1);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    return 0;
}

int quit( )
{
    release_program(program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &texco