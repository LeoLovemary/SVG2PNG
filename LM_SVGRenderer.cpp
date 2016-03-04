//
//  LM_SVGRenderer.cpp
//  Laser-Maze
//
//  Created by Pierre WILMOT on 04/03/16.
//
//


#include "LM_SVGRenderer.hpp"
#include "LM_Define.h" // RATIO is defined here
#include "cocos2d.h"
#include <stdio.h>
#include <float.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvgrast.h"

LM_SVG_Renderer::LM_SVG_Renderer()
:_isDone(false)
{
}

LM_SVG_Renderer::~LM_SVG_Renderer()
{
    _t.join();
}

void LM_SVG_Renderer::startRender()
{
    _t = std::thread(&LM_SVG_Renderer::renderAll, this);
}

void LM_SVG_Renderer::renderAll()
{
    const SVG_t files[10] =
    {
        {"crate.svg", 50, 50},
        {"gear.svg", 50, 50},
        {"laseremiter.svg", 50, 50},
        {"miror.svg", 50, 50},
        {"target_blue.svg", 50, 50},
        {"target_green.svg", 50, 50},
        {"target_orange.svg", 50, 50},
        {"target_purple.svg", 50, 50},
        {"target_red.svg", 50, 50},
        {"target_yellow.svg", 50, 50}
    };
    
    for (auto svg : files)
        render(svg.path, svg.w, svg.h);
    
    cocos2d::FileUtils::getInstance()->addSearchPath(cocos2d::FileUtils::getInstance()->getWritablePath());
    _isDone = true;
}

void LM_SVG_Renderer::render(const std::string &inpath, int width, int height)
{
    NSVGimage *image = NULL;
    NSVGrasterizer *rast = NULL;
    unsigned char* img = NULL;
    int w, h;
    float scale;
    std::string outpath = cocos2d::FileUtils::getInstance()->getWritablePath() + inpath;
    outpath.replace(outpath.find(".svg"), 4, ".png");
    
    image = nsvgParseFromFile(inpath, "px", 96.0f);
    if (image == NULL) {
        printf("Could not open SVG image.\n");
        goto error;
    }
    
    w = width * RATIO;
    h = height * RATIO;
    scale = (RATIO * width / image->height);
    
    rast = nsvgCreateRasterizer();
    if (rast == NULL) {
        printf("Could not init rasterizer.\n");
        goto error;
    }
    
    img = (unsigned char*)malloc(w*h*4);
    if (img == NULL) {
        printf("Could not alloc image buffer.\n");
        goto error;
    }
    
    printf("rasterizing image %d x %d\n", w, h);
    nsvgRasterize(rast, image, 0,0, scale, img, w, h, w*4);
    
    printf("writing %s\n", outpath.c_str());
    stbi_write_png(outpath.c_str(), w, h, 4, img, w*4);
    
error:
    nsvgDeleteRasterizer(rast);
    nsvgDelete(image);
}

bool LM_SVG_Renderer::isDone()
{
    return _isDone;
}

