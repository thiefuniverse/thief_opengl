#ifndef __LIGHT_H_
#define __LIGHT_H_

#include <glm/glm.hpp>
class light {
public:
    using Vec = glm::vec3;
    light(Vec ambient, Vec diffuse, Vec specular) : ambient_(ambient), diffuse_(diffuse), specular_(specular) {}

    // private:
    Vec ambient_;
    Vec diffuse_;
    Vec specular_;
};

const light default_light(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));

#endif  // __LIGHT_H_
