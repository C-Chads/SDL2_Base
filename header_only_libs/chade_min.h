#ifndef CHADPHYS_H
#define CHADPHYS_H
#include "chadphys.h"

typedef struct {
	phys_body body; /*body*/
	GLuint dl; /*Rendering details*/
} ChadEntity;

typedef struct{
	phys_world world;
	ChadEntity** ents;
	long n_ents; /*ents and ents_phys must contain the same number of elements.*/
	long max_ents;
} ChadWorld;

static inline void initChadWorld(ChadWorld* w, long max_entities){
	*w = (ChadWorld){0};
	w->world.ms = 1000;/*plenty fast*/
	w->world.g = (vec3){{0,-1,0}};/*plenty realistic*/
	w->ents = calloc(1, sizeof(void*) * max_entities);
	if(w->ents == NULL)exit(1);
	w->max_ents = max_entities;
	w->n_ents = 0;
}

static inline void stepChadWorld(ChadWorld* world, long iter){
	stepPhysWorld(&world->world, iter);
}
static inline long ChadWorld_AddEntity(ChadWorld* world, ChadEntity* ent){
	/*safety check.
	for(long i = 0; i < world->max_ents; i++)
		if(world->ents[i] == ent) return i;
	*/		
	for(long i = 0; i < world->max_ents; i++){
		if(world->ents[i] == NULL){
			world->ents[i] = ent;
			world->world.bodies[i] = &ent->body;
			if(world->n_ents <= i){
				world->n_ents = i + 1;
				world->world.nbodies = i+1;
			}
			return i;
		}
	}
	return -1;

}

static inline void ChadWorld_RemoveEntityByPointer(ChadWorld* world,  ChadEntity* ent){
	for(long i = 0; i < world->max_ents; i++)
		if(world->ents[i] == ent) {
			world->ents[i] = NULL;
			world->world.bodies[i] = NULL;
			//recalculate n_ents
			for(long i = 0; i < world->max_ents; i++)
				if(world->ents[i]) 
					{world->n_ents = i + 1;world->world.nbodies = i+1;}
			return;
		}
}


static inline void renderChadWorld(ChadWorld* world){
	long i; 
	/*The user will already have set up the viewport, the camera, and the lights.*/
	for(i = 0; i < world->n_ents; i++)
		if(world->ents[i]){
			glPushMatrix();
				/*build the transformation matrix*/
				glTranslatef(	world->ents[i]->body.shape.c.d[0],
								world->ents[i]->body.shape.c.d[1],
								world->ents[i]->body.shape.c.d[2]
							);
				glMultMatrixf(world->ents[i]->body.localt.d);
				/*Render that shizzle!*/
				glCallList(
					world->ents[i]->dl
				);
			glPopMatrix();
		}
}
#endif
