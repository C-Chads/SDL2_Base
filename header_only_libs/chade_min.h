#ifndef CHADPHYS_H
#define CHADPHYS_H
#include "chadphys.h"
#include <stdlib.h>

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
	w->max_ents = max_entities;
	w->n_ents = 0;
	
	w->ents = calloc(1, sizeof(void*) * max_entities);
	if(w->ents == NULL)exit(1);
	w->world.bodies_dynamic = calloc(1, sizeof(void*) * max_entities);
	if(w->world.bodies_dynamic == NULL)	exit(1);
	w->world.bodies_static = calloc(1, sizeof(void*) * max_entities);
	if(w->world.bodies_static == NULL)	exit(1);
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

			/*
			world->world.bodies[i] = &ent->body;
			*/
			if(ent->body.mass > 0){
				int found = 0;
				for(long j = 0; j < world->max_ents; j++)
				if(world->world.bodies_dynamic[j] == NULL)
				{
					found = 1;
					world->world.bodies_dynamic[j] = &ent->body;
					if(world->world.nbodies_dyn <= j) world->world.nbodies_dyn = j+1;
					goto after_insertion;
				}
				exit(1);
			} else {
				int found = 0;
				for(long j = 0; j < world->max_ents; j++)
				if(world->world.bodies_static[j] == NULL)
				{
					found = 1;
					world->world.bodies_static[j] = &ent->body;
					if(world->world.nbodies_static <= j) world->world.nbodies_static = j+1;
					goto after_insertion;
				}
				exit(1);
			}

			after_insertion:
			if(world->n_ents <= i){
				world->n_ents = i + 1;
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
			/*world->world.bodies[i] = NULL;*/
			
			for(long i = 0; i < world->world.nbodies_dyn; i++)
				if(world->world.bodies_dynamic[i] == &ent->body)
					world->world.bodies_dynamic[i] = NULL;

			for(long i = 0; i < world->world.nbodies_static; i++)
				if(world->world.bodies_static[i] == &ent->body)
					world->world.bodies_static[i] = NULL;

			//recalculate nbodies_static
			for(long i = 0; i < world->max_ents; i++)
				if(world->world.bodies_static[i])
					world->world.nbodies_static = i + 1;

			//recalculate nbodies_dynamic
			for(long i = 0; i < world->max_ents; i++)
				if(world->world.bodies_dynamic[i])
					world->world.nbodies_dyn = i + 1;

			//recalculate n_ents
			for(long i = 0; i < world->max_ents; i++)
				if(world->ents[i]) 
					world->n_ents = i + 1;
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
