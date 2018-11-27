
#ifndef _CHARANIMVIEWER_H
#define _CHARANIMVIEWER_H


#include "quaternion.h"
#include "Viewer.h"
#include "BVH.h"
#include "Skeleton.h"
#include "TransformQ.h"
#include "wavefront.h"
#include "text.h"
#include <PhysicalWorld.h>

class CharAnimViewer : public Viewer
{
public:
    CharAnimViewer();

    int init();
    int render();
    int update( const float time, const float delta );

	static CharAnimViewer& singleton() { return *psingleton;  }

protected:

    chara::BVH m_bvh;
    int m_frameNumber;

    Skeleton m_ske;

    PhysicalWorld m_world;

	void draw_skeleton(const Skeleton& );

    // tube
        Mesh m_tube;
        Mesh m_cover_tube;
        GLuint tube_texture;
        Transform t_tube;
        void init_tube();
        void draw_tube();
        std::vector <Point> tubePoints;
        Vector tubeNormal;
        int position = 0;

    // tube

	// terrai part --------------------------
    Image im;
    int height;
    int location;
    void init_terrain();
    void draw_terrain();
    Vector terrainNormale(const Image &heightmap,const int i,const int j);
    Mesh m_terrain;
    GLuint m_terrain_texture;
    Transform t_terrain = Scale(7,7,7) ;
    GLuint m_program_terrain;


    // obj
    Mesh m_object;
    Mesh m_objet;
    Transform m_transform;
    GLuint m_texture;
    GLuint m_program;
    GLuint tt_object;
    Point currentPoint;
    Mesh mesh_objet[3];
    GLuint text_object[5];
    Transform t_object;
    void init_object();
    void draw_object();
    void update_object();
    int lane = 0;
    float rotation ;
    bool flag = false ;
    int textIndex = 2;
    //Point position(0,0,0); //current position

     //Line
    Mesh lines;
    Transform t_lines;
    void init_Lines();
    void draw_Lines();
    bool line = false;
     //Line

    // Skybox
    void init_skyBox();
	void draw_skyBox();
	Transform t_box;
	Mesh m_cube_face[6];
	GLuint m_cubeFace_texture[6];
	DrawParam gl_skybox ;


	// Obstacle

	Mesh m_planet;
	Transform t_planet = Scale(0.01,0.01,0.01);
	Transform t_planet1 = Translation(10,0,0)*Scale(0.01,0.01,0.01);
	GLuint planet_texture;
	Transform t_plan;
	std::vector <Point> position_planet;
	void init_planet();
	void draw_planet();
	void update_planet();


	Mesh m_fish;
	GLuint fish_texture;
	Transform t_fish;
	std::vector <Point> position_fish;
	void init_fish();
	void draw_fish();

	// construction
	Mesh m_ship;
	Transform t_ship;
	GLuint ship_texture;
	GLuint m_program_ship;
	void init_ship();
	void draw_ship();
    bool camFollow =true;
    bool skyBox = false;
    bool inGame = false;

	//Score
	bool Col = false;
	int duration = 20;
	Text console;
	Text Collision;
private:
	static CharAnimViewer* psingleton;
};



#endif
