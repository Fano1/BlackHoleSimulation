#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//utilities function and shaders 
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main(void){

    GLfloat vertices[] = {
	-0.5f, -0.5f* float(sqrt(3))/3, 0.0f,
	0.5f, -0.5f* float(sqrt(3))/3, 0.0f,
	0.0f, 0.5f* float(sqrt(3))*2, 0.0f,
    };

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
    
    //setting up the buffer and shaders jj

    gladLoadGL();
    glViewport(0, 0, 800, 800); //from (0, 0) -> (800, 800)

    GLuint vertextShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertextShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertextShader);

    glShaderSource(FragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(FragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertextShader);
    glAttachShader(shaderProgram, FragmentShader);

    glLinkProgram(shaderProgram);

    glDeleteShader(vertextShader);
    glDeleteShader(FragmentShader);

    //draw a triangle 

    GLuint VAO, VBO; 

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);





    //color the screen 
    glClearColor(0.1f, 0.3f, 0.6f, 1);
    glClear(GL_COLOR_BUFFER_BIT); //color buffer
    glfwSwapBuffers(window);



    //true loop
    while(!glfwWindowShouldClose(window)){

        glClearColor(0.1f, 0.3f, 0.6f, 1);
        glClear(GL_COLOR_BUFFER_BIT); //color buffer
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);




        glfwPollEvents(); //process all the events
    }

    //terminate the program
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}