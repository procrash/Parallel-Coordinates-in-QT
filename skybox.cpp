#include "skybox.h"

#include <vector>
#include <glm/vec3.hpp>
#include <iostream>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <QFile>
#include <QTextStream>

Skybox::Skybox()
{

}

void Skybox::initialize() {

    loadTextures();

    loadVertexData();

    this->shaderProgramId = compileShaders();
    matrixUniformId = glGetUniformLocation(shaderProgramId, "matrix");



    createBuffers();

}

void Skybox::render() {

    glDepthMask(0);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(5);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(uiVAO);
    glUseProgram(this->shaderProgramId);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glUniformMatrix4fv(this->matrixUniformId, 1, GL_FALSE,  glm::value_ptr(modelViewMatrix));

    int iSamplerLoc = glGetUniformLocation(shaderProgramId, "gSampler");
    glUniform1i(iSamplerLoc, 0);
    cmTexture.bindTexture(0);


    glDrawArrays(GL_TRIANGLES, 0, vVertexData.size());

    glUseProgram(0);
    glBindVertexArray(0);

    glPointSize(0);
    glDisable(GL_POINT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDepthMask(1);
}

void Skybox::loadVertexData() {
    if (initialized) {
        vVertexData.clear();
        glDeleteBuffers(1,&vboVerticesData);
        glDeleteVertexArrays(1, &uiVAO);
    }

    GLfloat skyboxVertices[] = {
            // Positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        vVertexData.clear();

        for (int i=0; i<sizeof(skyboxVertices)/3;i++) {
            glm::vec3 point;

            point.x = skyboxVertices[0+i*3]*4;
            point.y = skyboxVertices[1+i*3]*4;
            point.z = skyboxVertices[2+i*3]*4;

            vVertexData.push_back(point);
        }
}

void Skybox::createBuffers() {
    // Create VBO
    glGenBuffers(1, &vboVerticesData);

    glGenVertexArrays(1, &uiVAO);
    glBindVertexArray(uiVAO);

    // Upload Vertex data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesData);
    glBufferData(GL_ARRAY_BUFFER, vVertexData.size()*3, &vVertexData[0], GL_STATIC_DRAW);

    // Vertex Coordinates...
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0 , 0);


}


void Skybox::printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

void Skybox::printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}


GLuint Skybox::compileShaders() {
    std::cout << "Compiling Heightmap Shaders" << std::endl;

    GLuint program_id;

    std::string skyboxVertexShaderStr = readStringFromResourceFile(":/shaders/vertexShaderSkybox.glsl").toStdString();
    std::string skyboxFragmentShaderStr = readStringFromResourceFile(":/shaders/fragmentShaderSkybox.glsl").toStdString();

    const GLchar *skyboxVertexShader_cStr   = (const GLchar *) skyboxVertexShaderStr.c_str();
    const GLchar *skyboxFragmentShader_cStr = (const GLchar *) skyboxFragmentShaderStr.c_str();

    // Create Skybox Vertex Shader
    GLuint skyboxVertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(skyboxVertexShaderId, 1, &skyboxVertexShader_cStr, NULL);
    glCompileShader(skyboxVertexShaderId);

    // Create Skybox Fragment Shader
    GLuint skyboxFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(skyboxFragmentShaderId, 1, &skyboxFragmentShader_cStr, NULL);
    glCompileShader(skyboxFragmentShaderId);

    // Print logs of Shaders
    printShaderInfoLog(skyboxVertexShaderId);
    printShaderInfoLog(skyboxFragmentShaderId);

    // Create Program, attach shaders and link
    program_id = glCreateProgram();

    // Attach the shaders to the program...
    glAttachShader(program_id, skyboxVertexShaderId);
    glAttachShader(program_id, skyboxFragmentShaderId);

    glLinkProgram(program_id);

    // Print logs of linker
    printProgramInfoLog(program_id);

    // Free resources as the program has now the shaders...
    glDeleteShader(skyboxVertexShaderId);
    glDeleteShader(skyboxFragmentShaderId);


    return program_id;
}

QString Skybox::readStringFromResourceFile(QString filenameIncludingPath) {
    QString result = "";

    QFile f(filenameIncludingPath);
    if (!f.open(QFile::ReadOnly | QFile::Text)) { return result; }

    QTextStream in(&f);
    //qDebug() << f.size() << in.readAll();
    result = in.readAll();
    f.close();
    return result;
}

void Skybox::setModelViewMatrix(glm::mat4 matrix) {
    this->modelViewMatrix = matrix;
}

void Skybox::loadTextures() {

    QString paths[] = {":/images/space_bck.png",
                       ":/images/space_bttm.png",
                       ":/images/space_frnt.png",
                       ":/images/space_left.png",
                       ":/images/space_rgt.png",
                       ":/images/space_top.png"};


    cmTexture.load(paths);
}
