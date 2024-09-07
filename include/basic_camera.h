#include "basic_math.h"

class BasicCamera{

    public:
        BasicCamera(){}

        BasicCamera(int windowWidth, int windowHeight);

        BasicCamera(const PersProjInfo& persProjInfo, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

};