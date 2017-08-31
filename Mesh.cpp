#include "Mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <float.h>

using namespace std;

namespace cam2vr {
    
    Mesh::Mesh(): m_vbo(0), m_ibo(0),
    m_initialized(false) 
    {
        vertices.clear();
        indices.clear();
    }
    
    Mesh::~Mesh() {
        if(m_vbo > 0) {
            glDeleteBuffers(1,&m_vbo);
            glDeleteBuffers(1,&m_ibo);
        }
        vertices.clear();
        indices.clear();
    }
    
    void Mesh::setup(GLSLProgram* program){
        
        if(vertices.size() < 1)
            return;
        
        //glGenVertexArrays(1, &m_vao);
        //glBindVertexArray(m_vao);

        // create vbo
        glGenBuffers(1,&m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
        
        unsigned int val;
        
        val = glGetAttribLocation(program->getHandle(), "position");
        glEnableVertexAttribArray(val);
        glVertexAttribPointer( val,  2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
        
        val = glGetAttribLocation(program->getHandle(), "texCoord");
        glEnableVertexAttribArray(val);
        glVertexAttribPointer( val,  3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
        
        // create ibo
        glGenBuffers(1,&m_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);
        
        //glBindVertexArray(0);
    }
    
    void Mesh::render(GLSLProgram* program, Texture* texture, float* viewportOffsetScale) {

        if(!program || !texture)
            return;
        
        //if(!m_vao)
        if(!m_vbo)
            setup(program);
        
        //glEnable(GL_DEPTH_TEST);
        
        program->bind();
        texture->bind();

        program->setUniform("viewportOffsetScale", viewportOffsetScale);
        program->setUniform("diffuse", (int)texture->index);
             
        //glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        
        glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0 );
        
        //for testing
        //glEnable(GL_PROGRAM_POINT_SIZE_EXT);
        //glPointSize(10);
        //glDrawArrays( GL_POINTS, 0, vertices.size()/5 );
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        //glBindVertexArray(0);
        texture->unbind();
        program->unbind();
    }
        
}; //namespace
