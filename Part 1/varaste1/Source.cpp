#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <time.h>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <chrono>
#include <thread>
#include <math.h>

using namespace std;

bool i = 1;
bool Clicked = 0;
bool ClickFlag = 0;

void framebuffer_size_callback(GLFWwindow* window, int PosWidth, int PosHeight);
void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void cursor_enter_callback(GLFWwindow* window, int entered);
void Set();

int Change = 0;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
unsigned int VBO, VAO, EBO, AVN;

double XCurrent = 0;
double YCurrent = 0;

float xx = 256;
float rr = ((rand()) % 128) / xx;
float gg = ((rand()) % 256) / xx;
float b = ((rand()) % 256) / xx;
float a = ((rand()) % 256) / xx;

float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f,   // top left 
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3,   // second Triangle
};

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.54, 0.2, 0.96, 0.6);\n"
"}\n\0";

/*************************/

//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
//    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//    {
//        double xpos, ypos;
//        //getting cursor position
//        glfwGetCursorPos(window, &xpos, &ypos);
//        cout << "Cursor Position at (" << xpos << " : " << ypos<< ")" << endl;
//    }
//}

// set up vertex data (and buffer(s)) and configure vertex attributes




int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Arya Varaste GHW1", NULL, NULL);
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

    // build and compile our shader program
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

  
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    float x = 256;
    srand(time(NULL));
    float r = ((rand()) % 128) / x;
    float g = ((rand()) % 256) / x;
    float b = ((rand()) % 256) / x;
    float a = ((rand()) % 256) / x;
    cout << r << endl;
    cout << g << endl;
    cout << b << endl;
    cout << a << endl;

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetCursorEnterCallback(window, cursor_enter_callback);


    // render loop
 
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glBindVertexArray(0);

        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly

void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); 

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

void Set() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int PosWidth, int PosHeight){
    // make sure the viewport matches the new window dimensions; note that PosWidth and 
    // PosHeight will be significantly larger than specified on retina displays.
    glViewport(0, 0, PosWidth, PosHeight);
}




void cursor_enter_callback(GLFWwindow* window, int entered){
    if (entered){
        printf("Entered To Window.\n");
    }
    else{
        printf("Exited The window!\n");
        ClickFlag = 0;
        Clicked = 0;
    }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        ClickFlag = 1;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        ClickFlag = 0;
        Clicked = 0;
    }
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos){
    int PosHeight;
    int PosWidth;
    glfwGetFramebufferSize(window, &PosWidth, &PosHeight);
    int xxx = PosWidth / 2;
    int yyy = PosHeight / 2;

    float x = (xpos - xxx) / ((float)PosWidth / 2);
    float y = -1.0 * (ypos - yyy) / ((float)PosHeight / 2);
    float Select = 0;
    float Ter = 100;
    if (Clicked && ClickFlag) {
        vertices[Change] = x;
        vertices[Change + 1] = y;
        Set();
    }
    if (!Clicked && ClickFlag) {
        int i = 0;
        while (i <= sizeof(vertices)){
            float xx = vertices[i];
            float yy = vertices[i + 1];
            float dist = (pow((x - xx), 2) + (pow ((y - yy), 2)));
            if (dist <= Ter) {
                Ter = dist;
                Select = i;
            }
            i = i + 3;
        }
        Clicked = 1;
        Change = Select;
    }
    XCurrent = x;
    YCurrent = y;
}

