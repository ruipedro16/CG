#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

#include "group.hpp"
#include "light.hpp"

class Scene {
private:
    std::vector<Light *> lights;
    Group *main_group;

public:
    Scene(Group *main_group);
    Group *get_main_group();
    void add_light(Light *light);
    void apply_light();
};

#endif
