
#include "pch.h"

#include "Animation.hpp"


std::string removeExtension(const std::string filename) 
{
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot); 
}





int main()
{
        Entity entity[3];

        entity[0].ImportStatic("/media/djoker/code/assets/Resources/Meshes/AlchemistRoom_01.x",true,1.0f);
        entity[1].ImportStatic("/media/djoker/code/assets/Resources/Meshes/AlchemistRoom_02.x",true,0.01f);
        entity[2].ImportStatic("/media/djoker/code/assets/Resources/Meshes/AlchemistRoom_03.x",true,0.01f);
        entity[0].Scale(0.01f,0.01f,0.01f);
        entity[1].Scale(0.01f,0.01f,0.01f);

        entity[2].Scale(0.01f,0.01f,0.01f);

        entity[0].Rotate(90.0f,0.0f,0.0f);
        entity[1].Rotate(90.0f,0.0f,0.0f);
        entity[2].Rotate(90.0f,0.0f,0.0f);

        entity[0].CombineSurfaces();
        entity[1].CombineSurfaces();
        entity[2].CombineSurfaces();

        entity[0].CleanSurfaces();
        entity[1].CleanSurfaces();
        entity[2].CleanSurfaces();
        

      //  
        entity[0].SaveStatic("/media/djoker/code/python/opengl/assets/models/AlchemistRoom_cpp_01.h3d");
        entity[1].SaveStatic("/media/djoker/code/python/opengl/assets/models/AlchemistRoom_cpp_02.h3d");
        entity[2].SaveStatic("/media/djoker/code/python/opengl/assets/models/AlchemistRoom_cpp_03.h3d");




    return 0;
}