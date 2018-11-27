
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "CharAnimViewer.h"

using namespace std;
using namespace chara;


CharAnimViewer* CharAnimViewer::psingleton = NULL;


CharAnimViewer::CharAnimViewer() : Viewer(), m_frameNumber(0)
{
	psingleton = this;
}
void CharAnimViewer::init_skyBox()
{
    static int x= 500;
    static int y=x;
    static int y2=0;

    static float pt[8][3] = { {-x,y2,-x}, {x,y2,-x}, {x,y2,x}, {-x,y2,x}, {-x,y,-x}, {x,y,-x}, {x,y,x}, {-x,y,x} };
    static int f[6][4] = {    {0,1,2,3}, {5,4,7,6}, {2,1,5,6}, {0,3,7,4}, {3,2,6,7}, {1,0,4,5} };
    static float n[6][3] = { {0,-1,0}, {0,1,0}, {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1} };
    int i,j;





   m_cubeFace_texture[0] = read_texture(0, ("data/yneg.png")) ;
   m_cubeFace_texture[1] = read_texture(0, ("data/ypos.png")) ;
   m_cubeFace_texture[2] = read_texture(0, ("data/xneg.png")) ;
   m_cubeFace_texture[3] = read_texture(0, ("data/xpos.png")) ;
   m_cubeFace_texture[4] =  read_texture(0, ("data/zneg.png")) ;
   m_cubeFace_texture[5] = read_texture(0, ("data/zpos.png")) ;

    for (i=0;i<6;i++)
    {
        m_cube_face[i] = Mesh(GL_TRIANGLE_STRIP);




         m_cube_face[i].texcoord(0,1);
         m_cube_face[i].vertex(pt[ f[i][3] ][0], pt[ f[i][3] ][1], pt[ f[i][3] ][2] );

         m_cube_face[i].texcoord(1,1);
         m_cube_face[i].vertex( pt[ f[i][2] ][0], pt[ f[i][2] ][1], pt[ f[i][2] ][2] );


         m_cube_face[i].texcoord( 0,0 );
         m_cube_face[i].vertex( pt[ f[i][0] ][0], pt[ f[i][0] ][1], pt[ f[i][0] ][2] );

         m_cube_face[i].texcoord( 1,0);
         m_cube_face[i].vertex( pt[ f[i][1] ][0], pt[ f[i][1] ][1], pt[ f[i][1] ][2] );

        m_cube_face[i].restart_strip();
    }
}
 void CharAnimViewer::draw_skyBox()
 {
    gl2.model(t_box);
    //gl.lighting(false);
    for(int i =0 ; i<6; i++)
    {
         gl2.texture(m_cubeFace_texture[i]);
         gl2.draw(m_cube_face[i]);
    }
 }
int CharAnimViewer::init()
{
    Viewer::init();
    cout<<"==>master_CharAnim/CharAnimViewer"<<endl;
    m_camera.lookat( Point(0,100,10), 1000 );
    gl.light( Point(0, 0, 0 ) );
    gl2.light( Point(0, 10, 0 ) );
    //b_draw_grid = false;

    init_skyBox();


    init_cylinder();
    init_sphere();


    //m_bvh.init("data/bvh/Robot.bvh" );
	m_bvh.init( smart_path("data/bvh/danse.bvh") );

    m_frameNumber = 0;
    cout<<endl<<"========================"<<endl;
    cout<<"BVH decription"<<endl;
    cout<<m_bvh<<endl;
    cout<<endl<<"========================"<<endl;

    //m_ske.init( m_bvh );
    //m_ske.setPose( m_bvh, -1);// met le skeleton a la pose au repos

    return 0;
}



void CharAnimViewer::draw_skeleton(const Skeleton& )
{
    // TODO
}



int CharAnimViewer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    Viewer::manageCameraLight();
    gl.camera(m_camera);
    gl2.camera(m_camera);
    draw_skyBox();

	// Affiche le skeleton
    draw_skeleton( m_ske );

    return 1;
}


int CharAnimViewer::update( const float time, const float delta )
{
    // time est le temps ecoule depuis le demarrage de l'application, en millisecondes,
    // delta est le temps ecoule depuis l'affichage de la derniere image / le dernier appel a draw(), en millisecondes.

	if (key_state('n')) { m_frameNumber++; cout << m_frameNumber << endl; }
	if (key_state('b')) { m_frameNumber--; cout << m_frameNumber << endl; }

	m_ske.setPose( m_bvh, m_frameNumber );

    m_world.update(0.1f);

    return 0;
}



