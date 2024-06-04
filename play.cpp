// #define GLM_FORCE_SWIZZLE
// #define GLM_SWIZZLE_XYZW
// #define GLM_ENABLE_EXPERIMENTAL
// #include<iostream>
// #include <glm/vec3.hpp>
// #include <glm/glm.hpp>
// #include <glm/gtx/string_cast.hpp>

// int main(){
//     glm::vec3 A(1.0, 6.0, 20.0);
//     glm::vec3 B(2.8);
//     glm::mat4 matrix(1.0);
//     glm::vec3 C = glm::cross(A, B);

//     std::cout << " C Values: " << glm::to_string(C) << std::endl;

//     std::cout << "A Values: " << glm::to_string(A.xy()) << std::endl;
//     std::cout << "Length of A: " << glm::length(A) << std::endl;

//     float dotproduct = glm::dot(glm::normalize(A), glm::normalize(B));
//     std::cout << "Dot Product Output: " << dotproduct << std::endl;
//     float angle = std::acos(dotproduct) * 180/M_PI;
//     std::cout << "Angle between A & B is " << angle << std::endl;

//     std::cout << "Vector Output: " << glm::to_string(A) << std::endl;
//     std::cout << "Vector Output: " << glm::to_string(B) << std::endl;
//     std::cout << "Matrix Output: " << glm::to_string(matrix) << "\n" << std::endl;

//     glm::vec4 vertex(1.0, 5.0, 1., 1.);
//     glm::mat4 model(1.0);

//     // Scaling Matrix
//     glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
//     std::cout << "Scale matrix: " << glm::to_string(scaleMatrix) << std::endl;

//     // Calculating the model on world space vertex
//     glm::vec4 worldSpaceVertex = (scaleMatrix * vertex);
//     std::cout << "Vertex in world space\n";
//     std::cout << glm::to_string(worldSpaceVertex) << "\n" << std::endl;

//     // Rotating Matrix
//     glm::mat4 rotatingMatrix = glm::rotate(glm::mat4(1.), glm::radians(180.0f), glm::vec3(2. , 2., 2.));
//     std::cout << "Rotating Matrix: " <<glm::to_string(rotatingMatrix) <<std::endl;

//     // Calculating the model on world space vertex
//     worldSpaceVertex = (rotatingMatrix * vertex);
//     std::cout << "Vertex in world space after rotating\n";
//     std::cout << glm::to_string(worldSpaceVertex) << "\n" << std::endl;

//     // Translating Matrix
//     glm::mat4 translatingMatrix = glm::translate(glm::mat4(1.), glm::vec3(0. , 0., -2.));
//     std::cout << "Translating Matrix: " <<glm::to_string(translatingMatrix) <<std::endl;

//     // Calculating the model on world space vertex
//     worldSpaceVertex = (translatingMatrix * vertex);
//     std::cout << "Vertex in world space after translating\n";
//     std::cout << glm::to_string(worldSpaceVertex) << "\n" << std::endl;

//     model = scaleMatrix * rotatingMatrix * translatingMatrix;
//     worldSpaceVertex = (model * vertex);
//     std::cout << "Vertex in world space after translating, rotating, scaling\n";
//     std::cout << glm::to_string(worldSpaceVertex) << "\n" << std::endl;

//     return 0;
// }