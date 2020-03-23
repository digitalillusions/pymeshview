#include <meshview/Window.h>
#include <meshview/MeshLoader.h>

int main(){
    meshview::Window window;

    // meshview::MeshLoader mesh;

    // window.run();
    vec3 axis = {0.0, 0.0, 1.0};
    window.highlight(axis, 4);
    return 0;
}