#pragma once

#include <glm/glm.hpp>

#include <cstdint>
#include <functional>
#include <thread>
#include <vector>
#include <iostream>

#ifndef NDEBUG
#include <cassert>
#endif

namespace LG
{

enum class DrawType
{
    Points,
    Lines,
    Line_Strip,
    Line_Loop
};


class VertexFunction
{
public:
    virtual ~VertexFunction() {}
    virtual glm::vec4 operator()(const glm::vec4& in) = 0;
};


//all vertices are 32bit float
//all indices are 32bit unsigned int
//color of line is determined by color of first vertex in line
class Context
{
public:
    Context()
    {
        vertex_pointer = nullptr;
        color_pointer = nullptr;
        index_pointer = nullptr;
        vertex_function = nullptr;

        xres = 256;
        yres = 256;
    }
    virtual ~Context() {}

    void setVertexFunction(VertexFunction* vf)
    {
        vertex_function = vf;
    }
    void setViewPort(const uint32_t x, const uint32_t y)
    {
        xres = x;
        yres = y;
    }

    virtual void laserColor(const uint8_t r, const uint8_t g, const uint8_t b) = 0;
    virtual void laserMove(const uint32_t x, const uint32_t y) = 0;
    virtual void laserOn() = 0;
    virtual void laserOff() = 0;
    virtual void clear() = 0;
    virtual void present() = 0;

    void VertexPointer(const uint32_t size, void* pointer)
    {
        vertex_pointer = pointer;
        vertex_size = size;
    }
    void ColorPointer(void* pointer)
    {
        color_pointer = pointer;
    }
    void IndexPointer(uint32_t* pointer)
    {
        index_pointer = pointer;
    }

    void DrawArray(DrawType drawtype, const uint32_t first, const uint32_t count)
    {
        if (!vertex_pointer)
        {
            return;
        }

        draw_type = drawtype;

        //gather pos into buffer
        work_buff.clear();

        if(vertex_size == 2)
        {
            glm::vec2* vp = reinterpret_cast<glm::vec2*>(vertex_pointer);
            for(uint32_t i = first ;i < count; ++i)
            {
                work_buff.push_back( Vertex{ {vp[i].x, vp[i].y,0.0f,1.0f}, {255,255,255,255} } );
            }
        }
        else if(vertex_size == 3)
        {
            glm::vec3* vp = reinterpret_cast<glm::vec3*>(vertex_pointer);
            for(uint32_t i = first ;i < count; ++i)
            {
                work_buff.push_back( Vertex{ {vp[i].x, vp[i].y,vp[i].z,1.0f}, {255,255,255,255} } );
            }
        }
        else if(vertex_size == 4)
        {
            glm::vec4* vp = reinterpret_cast<glm::vec4*>(vertex_pointer);
            for(uint32_t i = first ;i < count; ++i)
            {
                work_buff.push_back( Vertex{ vp[i], {255,255,255,255} } );
            }
        }

        //gather col into buffer
        if(color_pointer)
        {
            Color* cp = reinterpret_cast<Color*>(color_pointer);
            for(uint32_t i = first; i < count; ++i)
            {
                work_buff[i].col = cp[i];
            }
        }

        vertex_pipeline();
    }

    void DrawElements(DrawType drawtype, const uint32_t count)
    {
        if (!index_pointer || !vertex_pointer)
        {
            return;
        }

        draw_type = drawtype;

        //gather pos into buffer
        work_buff.clear();

        if(vertex_size == 2)
        {
            glm::vec2* vp = reinterpret_cast<glm::vec2*>(vertex_pointer);
            for(uint32_t i = 0 ;i < count; ++i)
            {
                work_buff.push_back( Vertex{ {vp[index_pointer[i]].x,vp[index_pointer[i]].y, 0.0f, 1.0f} , {255,255,255,255} } );
            }
        }
        else if(vertex_size == 3)
        {
            glm::vec3* vp = reinterpret_cast<glm::vec3*>(vertex_pointer);
            for(uint32_t i = 0 ;i < count; ++i)
            {
                work_buff.push_back( Vertex{ {vp[index_pointer[i]].x, vp[index_pointer[i]].y, vp[index_pointer[i]].z, 1.0f} , {255,255,255,255} } );
            }
        }
        else if(vertex_size == 4)
        {
            glm::vec4* vp = reinterpret_cast<glm::vec4*>(vertex_pointer);
            for(uint32_t i = 0 ;i < count; ++i)
            {
                work_buff.push_back( Vertex{ {vp[index_pointer[i]]} , {255,255,255,255} } );
            }
        }

        //gather col into buffer
        if(color_pointer)
        {
            Color* cp = reinterpret_cast<Color*>(color_pointer);
            for(uint32_t i = 0; i < count; ++i)
            {
                work_buff[i].col = cp[index_pointer[i]];
            }
        }

        vertex_pipeline();
    }

protected:
    struct Color
    {
        uint8_t r, g, b, a;

        Color(uint8_t c_r = 0, uint8_t c_g = 0, uint8_t c_b = 0, uint8_t c_a = 0) :
            r(c_r), g(c_g), b(c_b), a(c_a)
        {

        }
    };
    struct Vertex
    {
        glm::vec4 pos;
        Color col;

        Vertex(glm::vec4 p = {0.0f,0.0f,0.0f,0.0f}, Color c = {255,255,255,255}) :
            pos(p),
            col(c)
        {

        }

    };




    uint32_t xres, yres;
    void* vertex_pointer;
    void* color_pointer;
    uint32_t* index_pointer;

    uint32_t vertex_size;

    DrawType draw_type;

    VertexFunction* vertex_function;

    std::vector<Vertex> work_buff;


    std::vector<Vertex> convert_to_lines(const std::vector<Vertex>& in, DrawType dt)
    {
        std::vector<Vertex> out;

        if(dt == DrawType::Points)
        {
            for(uint32_t i = 0; i  <in.size(); ++i)
            {
                out.push_back(in[i]);
                out.push_back(in[i]);
            }
        }
        else if(dt == DrawType::Lines)
        {
            out = in;
        }
        else if(dt == DrawType::Line_Strip)
        {
            for(uint32_t i = 0; i < in.size() - 1; ++i)
            {
                out.push_back( in[i] );
                out.push_back( in[i + 1] );
            }
        }
        else if (dt == DrawType::Line_Loop)
        {
            for(uint32_t i = 0; i < in.size() - 1; ++i)
            {
                out.push_back( in[i] );
                out.push_back( in[i + 1] );
            }
            out.push_back(in[in.size() - 1]);
            out.push_back(in[0]);
        }

        return out;
    }


    void vertex_pipeline()
    {
        std::vector<Vertex> clip_buff = run_vertex_function(work_buff);
        std::vector<Vertex> line_buff = convert_to_lines(clip_buff, draw_type);
        std::vector<Vertex> ndc_buff = run_clip_function(line_buff);
        std::vector<Vertex> wt_buff = run_ndc_function(ndc_buff);
        std::vector<Vertex> draw_buff = run_windowtransform_function(wt_buff);
        run_draw_function(draw_buff);
    }

    std::vector<Vertex> run_vertex_function(const std::vector<Vertex>& in)
    {
        std::vector<Vertex> out;
        for (auto& i : in)
        {
            out.push_back(  Vertex{ vertex_function[0](i.pos) , i.col }  );
        }
        return out;
    }
    std::vector<Vertex> run_clip_function(const std::vector<Vertex>& in)
    {
        std::vector<Vertex> out;

        for(uint32_t i = 0; i < in.size() - 1; i = i + 2)
        {
            Vertex pi1, pi2, po1, po2;
            pi1 = in[i];
            pi2 = in[i+1];
            bool pi1in = clip_point(pi1);
            bool pi2in = clip_point(pi2);
            if(pi1in && pi2in)
            {
                out.push_back(pi1);
                out.push_back(pi2);
            }
            else if(pi1in || pi2in)
            {
                clip_line_component(pi1,pi2, 0, 1.0f, po1, po2);
                clip_line_component(po1,po2, 0, -1.0f, pi1, pi2);
                clip_line_component(pi1,pi2, 1, 1.0f, po1, po2);
                clip_line_component(po1,po2, 1, -1.0f, pi1, pi2);
                clip_line_component(pi1,pi2, 2, 1.0f, po1, po2);
                clip_line_component(po1,po2, 2, -1.0f, pi1, pi2);

                out.push_back( pi1 );
                out.push_back( pi2 );
            }


        }

        return  out;
    }
    std::vector<Vertex> run_ndc_function(const std::vector<Vertex>& in)
    {
        std::vector<Vertex> out;
        for (auto& i : in)
        {
            out.push_back( Vertex{ { i.pos / i.pos.w }, i.col });
        }
        return out;
    }
    std::vector<Vertex> run_windowtransform_function(const std::vector<Vertex>& in)
    {
        std::vector<Vertex> out;
        for (auto& i : in)
        {
            out.push_back
                    (
                        Vertex
                        {
                            {
                              ((xres / 2.0f) * i.pos.x) + (xres / 2.0f),
                              -((yres / 2.0f) * i.pos.y) + (yres / 2.0f),
                              ((1.0f / 2.0f) * i.pos.z) + (1.0f / 2.0f),
                              i.pos.w
                            },
                            {i.col}
                        }
                    );
        }
        return out;
    }

    void run_draw_function(const std::vector<Vertex>& in)
    {
        if(in.empty())
        {
            return;
        }


        for(uint32_t i = 0; i < in.size() - 1; i = i + 2)
        {
            laserOff();
            laserMove(in[i].pos.x, in[i].pos.y);
            laserColor(in[i].col.r, in[i].col.g, in[i].col.b);
            laserOn();

            laserMove(in[i+1].pos.x, in[i+1].pos.y);
            //laserColor(in[i+1].col.r, in[i+1].col.g, in[i].col.b);
        }

    }

    // returns true if point is inside volume
    bool clip_point(const Vertex& in)
    {
        if ((in.pos.x <= -in.pos.w ||
             in.pos.x >= in.pos.w ||
             in.pos.y <= -in.pos.w ||
             in.pos.y >= in.pos.w ||
             in.pos.z <= -in.pos.w ||
             in.pos.z >= in.pos.w))
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    void clip_line_component(const Vertex& q1, const Vertex& q2,
                             const int index, const float factor,
                             Vertex& q1new, Vertex& q2new)
    {
        q1new = q1;
        q2new = q2;

        Vertex previousVertex = q2;
        float previousComponent = previousVertex.pos[index] * factor;
        bool previousInside = previousComponent <= previousVertex.pos.w;

        Vertex currentVertex = q1;
        float currentComponent = currentVertex.pos[index] * factor;
        bool currentInside = currentComponent <= currentVertex.pos.w;

        if((currentInside) && (!previousInside))
        {
            float lerpAmount = (previousVertex.pos.w - previousComponent) /
                    ((previousVertex.pos.w - previousComponent) -
                    (currentVertex.pos.w - currentComponent));
            q2new.pos = glm::mix(previousVertex.pos, currentVertex.pos, lerpAmount);

            //glm::vec3 p2v3 = glm::mix(glm::vec3(previousVertex.pos), glm::vec3(currentVertex.pos), lerpAmount);
            //q2new.pos = glm::vec4(p2v3, previousVertex.pos.w);

            return;;

        }
        else if((!currentInside) && (previousInside))
        {
            float lerpAmount = (currentVertex.pos.w - currentComponent) /
                    ((currentVertex.pos.w - currentComponent) -
                    (previousVertex.pos.w - previousComponent));
            q1new.pos = glm::mix(currentVertex.pos, previousVertex.pos, lerpAmount);

            //glm::vec3 p1v3 = glm::mix(glm::vec3(currentVertex.pos), glm::vec3(previousVertex.pos), lerpAmount);
            //q1new.pos = glm::vec4(p1v3, currentVertex.pos.w);

            return;
        }
        return;
    }


};
}
