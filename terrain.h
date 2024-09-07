
#include "basic_type.h"
#include "basic_camera.h"
#include "basic_array_2d.h"
#include "triangle_list.h"


#if !defined(TERRAIN_H)
#define TERRAIN_H

class BaseTerrain {
    public:
        BaseTerrain(){}

        void initTerrain(float worldScale);

        void render(const BasicCamera& camera);

        void loadFromFile(const char* pFileName);

        float getHeight(int x, int z) const {return mHeightMap.get(x, z);}

        float getWorldScale() const {return mWorldScale;}

    protected:
        void loadHeightMapFile(const char* pFileName);

        int mTerrainSize = 0;
        float mWorldScale = 1.0f;
        Array2D<float> mHeightMap;
        TriangleList mTriangleList;
};


#endif // TERRAIN_H
