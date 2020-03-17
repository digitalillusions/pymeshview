//
// Created by Stefan on 17.03.2020.
//

#include <catch2/catch.hpp>
#include <meshview/Window.h>

TEST_CASE("Creation and deletion of window object", "[window]"){
    Window window;
    window.run(1);

    // Test case passes when this point is reached
    REQUIRE(true);
}


TEST_CASE("Creation and deletion of window object with size parameters", "[window]"){
    Window window(800, 600);
    window.run(1);

    // Test case passes when this point is reached
    REQUIRE(true);
}

TEST_CASE("Creation of an invisible window with size params", "[window]"){
    Window window(800, 600, false);
    window.run(1);
}
