
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "Shader.h"


using namespace std;
glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
bool i = 1;
bool w = 0;
float Sx = 1.0;
float Sy = 1.0;
float vertical_scale =1;
float Axis =0;
float x0y0 = 1.0;
float x0y1 = 0.0;
float x0y2 = 0.0;
float x1y0 = 0.0;
float x1y1 = 1.0;
float x1y2 = 0.0;
float x2y0 = 0.0;
float x2y1 = 0.0;
float x2y2 = 1.0;
float x3y0 = 1.0;
float x3y1 = 1.0;
float x3y2 = 0.0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
    


int main(){

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   x0y0, x0y1, x0y2,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   x1y0, x1y1, x1y2,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   x2y0, x2y1, x2y2,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   x3y0, x3y1, x3y2,   0.0f, 1.0f  // top left 
    };
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Arya Varaste GHW2", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    Shader ourShader("4.1.texture.vs", "4.1.texture.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes


    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
  

    // load and create a texture 
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("G:/Garya/varaste1/varaste1/Arya.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

     
        // render container
        ourShader.use();
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glfwWaitEventsTimeout(0.01);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        ourShader.use();
        unsigned int arya = glGetUniformLocation(ourShader.ID, "colorChangeValue");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glfwWaitEventsTimeout(0.01);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //cout << arya;
        float timeValue = glfwGetTime();
        float RedValue = (sin(Axis) * sin(Axis)) + 0.5f;
        float GreenValue = (cos(Axis) * sin(Axis) / 2.0f) + 0.5f;
        float BlueValue = (tan(Axis) * cos(Axis) / 2.0f) + 0.5f;
        cout << "RedValue: " << RedValue << endl;
        cout << "GreenValue: " << GreenValue << endl;
        cout << "GreenValue: " << GreenValue << endl;

        int vertexColorLocation = glGetUniformLocation(ourShader.ID, "ourColor");
        //cout << vertexColorLocation;
        glUseProgram(ourShader.ID);
        glUniform4f(vertexColorLocation, RedValue, GreenValue, BlueValue, 1.0f);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
  

    // render the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(window);
    glfwPollEvents();


    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // WASD
   
            // create transformations
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cout << "Press W" << endl;
        Sy = Sy * 1.001;
        transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(Axis), glm::vec3(0.0, 0.0, 1.0));
        transform = glm::scale(transform, glm::vec3(Sx, Sy, 1.0));
    }
    

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cout << "Press S" << endl;
        Sy = Sy * 0.999;
        transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(Axis), glm::vec3(0.0, 0.0, 1.0));
        transform = glm::scale(transform, glm::vec3(Sx, Sy, 1.0));
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cout << "Press A" << endl;
        Sx = Sx * 1.001;
        transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(Axis), glm::vec3(0.0, 0.0, 1.0));
        transform = glm::scale(transform, glm::vec3(Sx, Sy, 1.0));
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cout << "Press D" << endl;
        Sx = Sx * 0.999;
        transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(Axis), glm::vec3(0.0, 0.0, 1.0));
        transform = glm::scale(transform, glm::vec3(Sx, Sy, 1.0));
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cout << "Press Q" << endl;
        Axis = Axis + 0.5;
        transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(Axis), glm::vec3(0.0, 0.0, 1.0));
        transform = glm::scale(transform, glm::vec3(Sx, Sy, 1.0));
        x0y0 = x0y0 *0.5;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        cout << "Press E" << endl;
        Axis = Axis - 0.5;
        transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(Axis), glm::vec3(0.0, 0.0, 1.0));
        transform = glm::scale(transform, glm::vec3(Sx, Sy, 1.0));
       
        //cout << timeValue << endl;
    }


    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        switch (i) {
        case true: {
            i = false;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            cout << "Line" << endl;
            glfwWaitEventsTimeout(2);
            break;
        }
        case false: {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            i = true;
            cout << "Fill" << endl;
            glfwWaitEventsTimeout(2);
            break;
        }
        }
    }

    //    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    //        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //        cout << "fill";
    //    }

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        cout << "Fill" << endl;
        glfwWaitEventsTimeout(2);
    }

    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        cout << "Point" << endl;
        glfwWaitEventsTimeout(2);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}









