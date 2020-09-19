#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#include <glm/glm.hpp>
class material {
public:
    using Vec = glm::vec3;
    material(Vec ambient, Vec diffuse, Vec specular, float shininess)
        : ambient_(ambient), diffuse_(diffuse), specular_(specular), shininess_(shininess) {}

    // private:
    Vec ambient_;
    Vec diffuse_;
    Vec specular_;
    float shininess_;
};

const material
    default_material(glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f);
const material ruby_material(glm::vec3(0.0215, 0.1745, 0.0215),
                             glm::vec3(0.07568, 0.61424, 0.07568),
                             glm::vec3(0.633, 0.727811, 0.633),
                             0.6f * 128.0f);

#endif  // __MATERIAL_H_
