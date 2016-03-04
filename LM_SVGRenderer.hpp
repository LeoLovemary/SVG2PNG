//
//  LM_SVGRenderer.hpp
//  Laser-Maze
//
//  Created by Pierre WILMOT on 04/03/16.
//
//

#ifndef LM_SVGRenderer_hpp
#define LM_SVGRenderer_hpp

#include <string.h>
#include <stdio.h>
#include <thread>

class LM_SVG_Renderer
{
public:
    typedef struct SVG
    {
        std::string path;
        int w, h;
    } SVG_t;
    
public:
    LM_SVG_Renderer();
    ~LM_SVG_Renderer();
    
    void startRender();
    void renderAll();
    void render(std::string const &path, int w, int h);
    bool isDone();
    
private:
    std::thread _t;
    bool _isDone;
};

#endif /* LM_SVGRenderer_hpp */
