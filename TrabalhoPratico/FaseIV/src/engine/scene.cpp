#include "scene.hpp"

#include "light.hpp"

Scene::Scene(Group *main_group) : main_group(main_group) {}

Group *Scene::get_main_group() { return main_group; }

void Scene::add_light(Light *light) {
    glEnable(GL_LIGHT0 + (GLenum)lights.size());
    lights.push_back(light);
}

void Scene::apply_light() {
    GLenum number = 0;
    for (Light *l : lights) l->apply(number++);
}
