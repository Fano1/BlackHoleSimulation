#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(void){

    int width = 800; 
    int height = 800;
    
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "BlackHoleSimulation", NULL, NULL);

    if (window == NULL){
        std::cout << "Error occured" << std::endl;
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(window);

    //use glad to render shit
    //current frame -> current screen -> front buffer
    //next frame -> next screen -> back buffer
    //then swar the current & next buffer toi change the view 

    gladLoadGL(); //init
    glViewport(0, 0 , 800, 800); //setting boundry
    glClearColor(0.4f, 0.6f, 0.9f, 0); //back buffer calculation
    glClear(GL_COLOR_BUFFER_BIT); //specity bit
    glfwSwapBuffers(window); //swap


    //main loop
    while(!glfwWindowShouldClose(window)){

        glfwPollEvents();
    };


    glfwDestroyWindow(window);
    glfwTerminate();



    return 0;
}
