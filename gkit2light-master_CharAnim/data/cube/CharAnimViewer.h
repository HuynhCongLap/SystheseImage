
#ifndef _CHARANIMVIEWER_H
#define _CHARANIMVIEWER_H


#include "quaternion.h"
#include "Viewer.h"
#include "BVH.h"
#include "Skeleton.h"
#include "TransformQ.h"

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

	void init_skyBox();
	void draw_skyBox();
	Transform t_box;
	Mesh m_cube_face[6];
	GLuint m_cubeFace_texture[6];
	DrawParam gl_skybox ;




private:
	static CharAnimViewer* psingleton;
};



#endif
