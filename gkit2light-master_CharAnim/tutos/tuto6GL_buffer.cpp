
//! \file tuto6GL_buffer.cpp utilisation de glDrawArraysInstanced() et attributs d'instance sotckes dans un buffer

#include <cstdlib>

#include "vec.h"
#include "mat.h"

#include "mesh.h"
#include "wavefront.h"

#include "program.h"
#include "uniforms.h"

#include "orbiter.h"
#include "app.h"        // classe Application a deriver


class DrawInstanceBuffer : public App
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    DrawInstanceBuffer( ) : App(1024, 640) {}
    
    int init( )
    {
        // charge un mesh
        Mesh mesh= read_mesh("data/cube.obj");
        if(mesh == Mesh::error()) return -1;
        
        Point pmin, pmax;
        mesh.bounds(pmin, pmax);
        m_camera.lookat(pmin - Vector(40, 40, 40), pmax + Vector(40, 40, 40));
        
        m_vertex_count= mesh.vertex_count();

        // cree les buffers et le vao
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);
        
        // buffer : positions + normals
        glGenBuffers(1, &m_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
        
        size_t size= mesh.vertex_buffer_size() + mesh.normal_buffer_size();
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
        
        // transfere les positions des sommets
        size_t offset= 0;
        size= mesh.vertex_buffer_size();
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, mesh.vertex_buffer());
        // configure l'attribut 0, vec3 position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, /* stride */ 0, (const GLvoid *) offset);
        glEnableVertexAttribArray(0);

        // transfere les normales des sommets
        offset= offset + size;
        size= mesh.normal_buffer_size();
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, mesh.normal_buffer());
        // configure l'attribut 2, vec3 normal
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, /* stride */ 0, (const GLvoid *) offset);
        glEnableVertexAttribArray(2);


        // instance buffer, position aleatoire des cubes...
        srand(time(NULL));
        
        std::vector<vec3> positions;
        for(int i= 0; i < 50; i++)
        {
            float x= rand() % 11 - 5;
            float y= rand() % 11 - 5;
            float z= rand() % 11 - 5;
            
            positions.push_back(vec3(x *4, y *4, z *4));
        }
        
        m_instance_count= (int) positions.size();
        
        // cree et initialise le buffer d'instance
        glGenBuffers(1, &m_instance_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_instance_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * positions.size(), &positions.front().x, GL_STATIC_DRAW);

        // configure le vao pour l'attribut d'instance
        // configure l'attribut 1, vec3 instance_position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, /* stride */ 0, /* offset */ 0);
        glVertexAttribDivisor(1, 1);    // !! c'est la seule difference entre un attribut de sommet et un attribut d'instance !!
        glEnableVertexAttribArray(1);
        
        //
        mesh.release();
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // shaders
        m_program= read_program("tutos/instance_buffer.glsl");
        program_print_errors(m_program);
        
        
        // etat openGL par defaut
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre
        
        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest

        return 0;   // ras, pas d'erreur
    }
    
    // destruction des objets de l'application
    int quit( )
    {
        release_program(m_program);
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_buffer);
        glDeleteBuffers(1, &m_instance_buffer);
        return 0;
    }
    
    int update