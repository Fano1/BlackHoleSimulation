#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//utilities function

int main(void){

    //init and hint
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 800;
    int height = 800;

    GLFWwindow* window = glfwCreateWindow(width, height, "toturial window", NULL, NULL);

    if (window == NULL){
        std::cout << "error occured" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    //setting up the buffer and other things
    gladLoadGL();
    glViewport(0, 0, 800, 800); //from (0, 0) -> (800, 800)
    glClearColor(0.1f, 0.3f, 0.6f, 1);

    glClear(GL_COLOR_BUFFER_BIT); //color buffer

    glfwSwapBuffers(window);

    //true loop

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents(); //process all the events
    }

    //terminate the program
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}