#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include <string>

using std::vector;
using std::string;

#include "Program.h"
#include "Texture.h"


namespace cam2vr {
    
    class Mesh {
        
    private:
        unsigned int m_vbo;
        unsigned int m_ibo;
        //unsigned int m_vao;
        bool m_initialized;        
        
    public:
        vector<float> vertices;
        vector<unsigned int> indices;
        
    private:
        void setup(GLSLProgram* program);
        
    public:
        Mesh();
        ~Mesh();
        
        void render(GLSLProgram* program, Texture* texture, float* viewportOffsetScale);
    };
    
}; //namespace

#endif
