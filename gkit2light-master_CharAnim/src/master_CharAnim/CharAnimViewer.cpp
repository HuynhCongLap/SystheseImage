
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>
#include "program.h"
#include "CharAnimViewer.h"
#include "uniforms.h"
using namespace std;
using namespace chara;


CharAnimViewer* CharAnimViewer::psingleton = NULL;


CharAnimViewer::CharAnimViewer() : Viewer(), m_frameNumber(0)
{
	psingleton = this;
}


int CharAnimViewer::init()
{
    Viewer::init();
    cout<<"==>master_CharAnim/CharAnimViewer"<<endl;
    m_camera.lookat( Point(0,0,0), 2000 );
    gl.light( Point(300, 2000, 300 ) );
    gl_skybox.light(Point(0, 0, 0 ));
    init_terrain();
    init_tube();
    init_object();
    init_skyBox();
    init_planet();
    init_fish();
    init_ship();
    init_Lines();


    console = create_text();
    //m_ske.init( m_bvh );
    //m_ske.setPose( m_bvh, -1);// met le skeleton a la pose au repos

    return 0;
}

//Lines
void CharAnimViewer::init_Lines()
{

}
void CharAnimViewer::draw_Lines()
{
    lines = Mesh(GL_LINES);
    lines.color(Color(1, 0, 0)) ;

    for(int i=0; i < position_planet.size(); i++)
    {
        Point now = Point(currentPoint.x,currentPoint.y+1,currentPoint.z);
        lines.vertex(now);
        lines.vertex(position_planet[i]);
        if(length(Vector(now,position_planet[i])) < 2)
        {
            Col = true;
        }

    }
    gl.model(t_lines);
    if(line){
        gl.draw(lines);
    }
}
//Lines
// Tube
void CharAnimViewer::init_tube()
{

    m_tube = Mesh(GL_LINES) ;
    m_cover_tube = Mesh(GL_TRIANGLE_STRIP);
    tube_texture = read_texture(0, "data/road.jpg");

    Point intPoint(Point(100,200,0));
    tubePoints.push_back(intPoint);
    m_tube.vertex(intPoint);
    tubeNormal = Vector(0,1,0);
    for(int i = 1 ; i <= 5000 ; i++ ){
        float angle = 360.0*i/5000;
        Point next = RotationY(angle)(intPoint);
        tubePoints.push_back(next);
        m_tube.vertex(next);
        m_tube.vertex(next);
    }


    int count = 0;
    for(int i = 0; i< tubePoints.size()-1; i++)
    {
         if (count == 100)
            count = 0;
      for(int j = 0; j <= 20 ; j++)
     {
        float ang = (360.0*j)/20;

        Vector next2 = Rotation(tubePoints[i+2] - tubePoints[i+1],ang)(tubeNormal);
        m_cover_tube.normal(Vector(0,1,0));
        m_cover_tube.texcoord((float(j)/20.0),(count+1)/100.0);
        m_cover_tube.vertex(tubePoints[i+1].x + next2.x,tubePoints[i+1].y + next2.y,tubePoints[i+1].z + next2.z);



        Vector next1 = Rotation(tubePoints[i+1] - tubePoints[i],ang)(tubeNormal);
        m_cover_tube.normal(Vector(0,2,0));
        m_cover_tube.texcoord((float(j)/20.0),(count)/100.0);
        m_cover_tube.vertex(tubePoints[i].x + next1.x,tubePoints[i].y + next1.y,tubePoints[i].z + next1.z);


      }
        count++ ;

    }
     //m_cover_tube.restart_strip();
}

void CharAnimViewer::draw_tube()
{

   gl.model(t_tube);
   gl.draw(m_tube);
   //program_use_texture(m_program, "texture0", 0, m_texture);
   gl.texture(tube_texture);
   gl.draw(m_cover_tube);
   //draw(m_cover_tube, m_program, true, false, false, false);
}
// Tube

//---------TERRAIN--------------------------------------------
Vector CharAnimViewer::terrainNormale(const Image &heightmap,const int i,const int j)
{
    height=20;
    int ip=i-1;
    int in=i+1;
    int jp=j-1;
    int jn=j+1;
    Vector a(ip,heightmap(ip,j).r,j);
    Vector b(in,heightmap(in,j).r,j);
    Vector c(i,heightmap(i,jp).r,jp);
    Vector d(i,heightmap(i,jn).r,jn);

    Vector AB=normalize(b-a);
    Vector CD=normalize(d-c);
    Vector m=cross(AB,CD);
    return m;
}

void CharAnimViewer::init_terrain(){
    m_terrain = Mesh(GL_TRIANGLE_STRIP);
    im = read_image("data/terrain/Clipboard01.png");
    m_terrain_texture = read_texture(0, "data/mandaris_dn.tga");

    float divX=im.width();
    float divY=im.height();
    for(int i=1; i< im.width() -1 ;i++)
    {
        for (int j=1; j< im.height()-1;j++)
        {
             location=im(i,j).r*height;

            m_terrain.texcoord((i+1)/divX,j/divY);
            m_terrain.normal(-terrainNormale(im,(i+1),j));
            m_terrain.vertex(Point((i+1)-divX/2,im(i+1,j).r*height,j-divY/2));

            m_terrain.texcoord(i/divX,j/divY);
            m_terrain.normal(-terrainNormale(im,i,j));
            m_terrain.vertex(Point(i-divX/2,im(i,j).r*height,j-divY/2));

        }

        m_terrain.restart_strip();
    }
    m_program_terrain = read_program("tutos/tuto9_texture1.glsl");
    program_print_errors(m_program_terrain);

}


void CharAnimViewer::draw_terrain()
{
    //gl.model(t_terrain);
    //gl.texture(m_terrain_texture);
    //gl.draw(m_terrain);

    glUseProgram(m_program_terrain);
    Transform model= t_terrain;
    Transform view= m_camera.view();
    Transform projection= m_camera.projection(window_width(), window_height(), 45);

        // . composer les transformations : model, view et projection
    Transform mvp= projection * view * model;

    program_uniform(m_program_terrain, "mvpMatrix", mvp);
    program_uniform(m_program_terrain, "camposition", vec3(m_camera.position()));
    program_use_texture(m_program_terrain, "texture0", 0, m_terrain_texture);
    //glUseProgram(m_program);
    draw(m_terrain, m_program_terrain);
}

//--------TERRAIN-------------------------------------------------------------------------

// SHIP
void CharAnimViewer::init_ship()
{
    m_ship= read_mesh("data/falcon.obj");
    ship_texture= read_texture(0, "data/falcon.jpg");
    m_program_ship = read_program("tutos/tuto9_texture1.glsl");
    program_print_errors(m_program_ship);


    //tt_object= read_texture(0, "data/debug2x2red.png");
}

void CharAnimViewer::draw_ship()
{


    glUseProgram(m_program_ship);
    t_ship = Translation(500,200,2500);
    Transform model= t_ship;
    Transform view= m_camera.view();
    Transform projection= m_camera.projection(window_width(), window_height(), 45);

        // . composer les transformations : model, view et projection
    Transform mvp= projection * view * model;

    program_uniform(m_program_ship, "mvpMatrix", mvp);
    program_uniform(m_program_ship, "camposition", vec3(m_camera.position()));
    program_use_texture(m_program_ship, "texture0", 0, ship_texture);
    //glUseProgram(m_program);
    draw(m_ship, m_program_ship);

    t_ship = Translation(-200,200,1000)*Scale(0.3,0.3,0.3);
    Transform model1= t_ship;
    Transform mvp1= projection * view * model1;
    program_uniform(m_program_ship, "mvpMatrix", mvp1);
    program_uniform(m_program_ship, "camposition", vec3(m_camera.position()));
    program_use_texture(m_program_ship, "texture0", 0, ship_texture);
    draw(m_ship, m_program_ship);


}
//
// OBJECT
void CharAnimViewer::init_object()
{

    text_object[2] = read_texture(0, "data/debug2x2red.png");
    text_object[3] = read_texture(0, "data/xneg.png");
    text_object[4] = read_texture(0, "data/desertsky_lf.tga");

    text_object[0] = read_texture(0, "data/red.png");
    text_object[1] = read_texture(0, "data/white.jpg");


    mesh_objet[0] = read_mesh("data/FC.obj");
    mesh_objet[1] = read_mesh("data/Raider.obj");
    mesh_objet[2] = read_mesh("data/Viper.obj");


    m_objet= mesh_objet[0];
    m_texture= text_object[2];
    m_program= read_program("tutos/tuto9_texture1.glsl");
    program_print_errors(m_program);


    //tt_object= read_texture(0, "data/debug2x2red.png");
}

void CharAnimViewer::draw_object()
{


    glUseProgram(m_program);
    Transform model= t_object;
    Transform view= m_camera.view();
    Transform projection= m_camera.projection(window_width(), window_height(), 45);

        // . composer les transformations : model, view et projection
    Transform mvp= projection * view * model;

    program_uniform(m_program, "mvpMatrix", mvp);
    program_uniform(m_program, "camposition", vec3(m_camera.position()));
    program_use_texture(m_program, "texture0", 0, m_texture);
    //glUseProgram(m_program);
    draw(m_objet, m_program);
}
// OBJECT

// FISH
void CharAnimViewer::init_fish()
{
    for(int i = 0; i< tubePoints.size(); i+=200)
    {
        int v1 = rand() % 100;
        int v2 = rand() % (i+1) + (i+5);
        float angle = (360.0*v1)/100;
        Vector newNormal = Rotation(tubePoints[i+1]-tubePoints[i],angle)(Vector(0,v2,0));
        Point newPoint(tubePoints[i].x + newNormal.x,tubePoints[i].y + newNormal.y, tubePoints[i].z + newNormal.z);
        position_fish.push_back(newPoint);
    }
    fish_texture = read_texture(0, "data/eye.jpg");
    m_fish= read_mesh("data/eye.obj");
}

void CharAnimViewer::draw_fish()
{
    gl.texture(fish_texture);
    for(int i=0 ; i < position_fish.size(); i++){

        Transform new_t = Translation(position_fish[i].x,position_fish[i].y,position_fish[i].z)*Scale(0.4,0.4,0.4);

        t_fish = new_t;
        gl.model(t_fish);
        gl.draw(m_fish);
    }

    //for(int i=0 ; i < position_planet.size(); i++)
    //{

        //t_plan[i] = Translation(position_planet[i].x,position_planet[i].y,position_planet[i].z)*Scale(0.01,0.01,0.01);
        //gl.model(t_plan[i]);
        //gl.draw(m_planet);
    //}
}


// PLanet
void CharAnimViewer::init_planet()
{
    for(int i = 0; i< tubePoints.size(); i+=100)
    {
        int v1 = rand() % 100;
        int v2 = rand() % 2 + 4;
        float angle = (360.0*v1)/100;
        Vector newNormal = Rotation(tubePoints[i+1]-tubePoints[i],angle)(Vector(0,v2,0));
        Point newPoint(tubePoints[i].x + newNormal.x,tubePoints[i].y + newNormal.y, tubePoints[i].z + newNormal.z);
        position_planet.push_back(newPoint);
    }
    planet_texture = read_texture(0, "data/4096_earth.jpg");
    m_planet= read_mesh("data/earth.obj");
}

void CharAnimViewer::draw_planet()
{
    gl.texture(planet_texture);
    for(int i=0 ; i < position_planet.size(); i++){

        Transform new_t = Translation(position_planet[i].x,position_planet[i].y,position_planet[i].z)*Scale(0.001,0.001,0.001);
        int v2 = rand() % 1 + i+30;
        int v3 = rand() % 1 + 4;
        t_plan = new_t*RotationY(global_time()/20)*Scale(v3,v3,v3);
        gl.model(t_plan);
        gl.draw(m_planet);

    }

    //for(int i=0 ; i < position_planet.size(); i++)
    //{

        //t_plan[i] = Translation(position_planet[i].x,position_planet[i].y,position_planet[i].z)*Scale(0.01,0.01,0.01);
        //gl.model(t_plan[i]);
        //gl.draw(m_planet);
    //}
}


void CharAnimViewer::update_planet()
{

}
// Planet
//Skybox
void CharAnimViewer::init_skyBox()
{
    static int x= 800;
    static int y=x;
    static int y2=0;

    static float pt[8][3] = { {-x,y2,-x}, {x,y2,-x}, {x,y2,x}, {-x,y2,x}, {-x,y,-x}, {x,y,-x}, {x,y,x}, {-x,y,x} };
    static int f[6][4] = {    {0,1,2,3}, {5,4,7,6}, {2,1,5,6}, {0,3,7,4}, {3,2,6,7}, {1,0,4,5} };
    static float n[6][3] = { {0,-1,0}, {0,1,0}, {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1} };
    int i,j;





   m_cubeFace_texture[0] = read_texture(0, ("data/mandaris_dn.tga")) ;
   m_cubeFace_texture[1] = read_texture(0, ("data/mandaris_up.tga")) ;
   m_cubeFace_texture[2] = read_texture(0, ("data/mandaris_bk.tga")) ;
   m_cubeFace_texture[3] = read_texture(0, ("data/mandaris_ft.tga")) ;
   m_cubeFace_texture[4] =  read_texture(0, ("data/mandaris_lf.tga")) ;
   m_cubeFace_texture[5] = read_texture(0, ("data/mandaris_rt.tga")) ;

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
    gl_skybox.model(t_box);
    //gl.lighting(false);
    for(int i =0 ; i<6; i++)
    {
         gl_skybox.texture(m_cubeFace_texture[i]);
         gl_skybox.draw(m_cube_face[i]);
    }
 }
// Skybox
void CharAnimViewer::draw_skeleton(const Skeleton& )
{
    // TODO
}



int CharAnimViewer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Viewer::manageCameraLight();
    gl.camera(m_camera);
    gl_skybox.camera(m_camera);





        // . parametres "supplementaires" :
        //   . utilisation d'une texture configuree sur l'unite 0, le fragment shader declare "uniform sampler2D texture0;"


    if (key_state('1')) { inGame = true;}
    if (key_state('0')) { inGame = false;}

    if (key_state('t')) { line = true;}
    if (key_state('y')) { line = false;}

    if(inGame){

    draw_terrain();
    draw_tube();
    draw_object();
    draw_planet();
    draw_fish();

    draw_Lines();

    if (key_state('k')) { skyBox = true;}
    if (key_state('l')) { skyBox = false;}
    if(skyBox){
        draw_skyBox();
    }
    //draw_skyBox();
    draw_ship();

    clear(console);
    printf(console, 50, 5, "Score: %d",position);
    draw(console, window_width(), window_height());
    }
    else
    {

        if (key_state('2')) { m_texture= text_object[2] ; textIndex = 2;}
        if (key_state('3')) { m_texture= text_object[3] ; textIndex = 3;}
        if (key_state('4')) { m_texture= text_object[4] ; textIndex = 4;}

        if (key_state('5')) { m_objet= mesh_objet[0] ;}
        if (key_state('6')) { m_objet= mesh_objet[1];}
        if (key_state('7')) { m_objet= mesh_objet[2];}

        t_object = RotationY(global_time()/20);
        draw_object();
    }

    return 1;
}
void CharAnimViewer::update_object()
{
    if (position == tubePoints.size()-1)
        position = 0;

    Vector next_direction = normalize(Vector(tubePoints[position+1] - tubePoints[position]));

    float angle_d = (dot(Vector(0,0,1),next_direction));
    float angle = (acos(angle_d)*180.0)/3.1419;
    float goc = -angle;

    rotation = -360.0*lane/100;
    if (key_state('a'))
    {
        if (lane == 99)
            lane = 0;
        else
            lane++;
        rotation = -360.0*lane/100;
    }

      if (key_state('d'))
    {
        if (lane == 0)
            lane = 99;
        else
            lane--;
        rotation = -360.0*lane/100;
    }
    if (position > tubePoints.size()/2)
        goc = -goc;
    Vector t = normalize(Rotation(next_direction,rotation)(Vector(0,1,0)));
    currentPoint = Point(tubePoints[position].x + t.x*1.5, tubePoints[position].y + t.y*1.5, tubePoints[position].z +t.z*1.5 );
    t_object = Translation(currentPoint.x,currentPoint.y,currentPoint.z)*Rotation(next_direction,rotation)*RotationY(goc)*Scale(0.01,0.01,0.01);
    Point cam = Point(tubePoints[position].x + 0, tubePoints[position].y + 5, tubePoints[position].z + 0) ;




    //for(int i=0; i < position_planet.size(); i++)
    //{
       // lines.vertex(currentPoint);
       // lines.vertex(position_planet[i]);
       // lines.vertex(position_planet[i]);
    //}



    if (key_state('c')) { camFollow = true;}
    if (key_state('v')) { camFollow = false;}
    if(camFollow){

        m_camera.up(cam,15, goc);
    }

}
int CharAnimViewer::update( const float time, const float delta )
{
    // time est le temps ecoule depuis le demarrage de l'application, en millisecondes,
    // delta est le temps ecoule depuis l'affichage de la derniere image / le dernier appel a draw(), en millisecondes.

	if (key_state('n')) { m_frameNumber++; cout << m_frameNumber << endl; }
	if (key_state('b')) { m_frameNumber--; cout << m_frameNumber << endl; }
	if (key_state('m')) { position += 5; }



    //object angle
    if(inGame){
    update_object();

        if(Col)
        {
            m_texture = text_object[duration%2] ;
            duration--;
            if(duration == 0)
            {
                Col = false;
                duration = 20;
            }
        }
        else
        {
            m_texture = text_object[textIndex];
        }
    }
    position += 1;
    return 0;
}



