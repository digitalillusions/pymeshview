//
// Created by Stefan on 17.03.2020.
//

#include <catch2/catch.hpp>
#include <meshview/Window.h>

TEST_CASE("Creation and deletion of window object", "[window]"){
    Window window();
    Window window2(800, 600);
}

