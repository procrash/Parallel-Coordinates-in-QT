#include "heightmap.h"
#include <QImage>

#include <iostream>


#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

/*
#include "3rdParty/glm/glm/vec3.hpp"
#include "3rdParty/glm/glm/glm.hpp"
*/

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QFile>
#include <QTextStream>

#include <vector>

using namespace std;

HeightMap::HeightMap()
{


}

// To make sure we call OpenGL Functions AFTER the OpenGL Context has been created...
void HeightMap::initialize() {
//    if (initialized)  return;
    if (initialized) {
        glDeleteBuffers(1, &vboVerticesData);
        glDeleteBuffers(1, &vboVerticesData);
    }

    initialized = true;

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(5);

    // Enable Textures...
    glEnable(GL_TEXTURE_2D);

    // glLineWidth(5);

    this->shaderProgramId = compileShaders();
    matrixUniformId = glGetUniformLocation(shaderProgramId, "matrix");


    loadTextures();

    loadVertexData();
    calculateTriangleNormals();
    createBuffers();
}

void HeightMap::loadTextures() {
    heightMapTexture.load(":/images/StoneTexture.jpg");
    // heightMapTexture.load(":/images/snow.jpg");
    heightMapTexture.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_TRILINEAR);
}

void HeightMap::setModelViewMatrix(glm::mat4 matrix) {
    this->modelViewMatrix = matrix;
}

QString HeightMap::readStringFromResourceFile(QString filenameIncludingPath) {
    QString result = "";

    QFile f(filenameIncludingPath);
    if (!f.open(QFile::ReadOnly | QFile::Text)) { return result; }

    QTextStream in(&f);
    //qDebug() << f.size() << in.readAll();
    result = in.readAll();
    f.close();
    return result;
}


void HeightMap::printShaderInfoLog(GLuint obj)
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

void HeightMap::printProgramInfoLog(GLuint obj)
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

GLuint HeightMap::compileShaders() {

    cout << "Compiling Heightmap Shaders" << endl;

    GLuint program_id;

    string heightmapVertexShaderStr = readStringFromResourceFile(":/shaders/heightmapVertexShader.glsl").toStdString();
    string heightmapFragmentShaderStr = readStringFromResourceFile(":/shaders/heightmapFragmentShader.glsl").toStdString();

    const GLchar *heightmapVertexShader_cStr   = (const GLchar *) heightmapVertexShaderStr.c_str();
    const GLchar *heightmapFragmentShader_cStr = (const GLchar *) heightmapFragmentShaderStr.c_str();


    // Create Heightmap Vertex Shader
    GLuint heightmapVertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(heightmapVertexShaderId, 1, &heightmapVertexShader_cStr, NULL);
    glCompileShader(heightmapVertexShaderId);

    // Create Heightmap Fragment Shader
    GLuint heightmapFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(heightmapFragmentShaderId, 1, &heightmapFragmentShader_cStr, NULL);
    glCompileShader(heightmapFragmentShaderId);

    // Print logs of Shaders
    printShaderInfoLog(heightmapVertexShaderId);
    printShaderInfoLog(heightmapFragmentShaderId);

    // Create Program, attach shaders and link
    program_id = glCreateProgram();

    // Attach the shaders to the program...
    glAttachShader(program_id, heightmapVertexShaderId);
    glAttachShader(program_id, heightmapFragmentShaderId);

//    glBindFragDataLocation(program_id, 0, "aColor"); // TODO: check if this is necessary
    glLinkProgram(program_id);

    // Print logs of linker
    printProgramInfoLog(program_id);

    // Free resources as the program has now the shaders...
    glDeleteShader(heightmapVertexShaderId);
    glDeleteShader(heightmapFragmentShaderId);


    return program_id;
}

void HeightMap::loadVertexData() {

    cout << "Loading Vertex Data" << endl;

    QImage img(":/images/heightmap.jpg");
//    QImage img(":/images/Black.png");

    iRows = img.height();
    iCols = img.width();

    float fTextureU = float(iCols)*0.005f;
    float fTextureV = float(iRows)*0.005f;

    vVertexData = vector<vector<glm::vec3>>(iRows, vector<glm::vec3>(iCols));
    vCoordsData = vector<vector<glm::vec2>> (iRows, vector<glm::vec2>(iCols));

    for ( int row = 0; row < iRows ; ++row ) {
        for ( int col = 0; col < iCols; ++col )
        {
            QColor clrCurrent( img.pixel( row, col ) );

            float red = clrCurrent.red();
            float green = clrCurrent.green();
            float blue = clrCurrent.blue();

            float level = (red + green + blue)/3;

            if (level>255.0f) level=255.0f;

            float fScaleC = float(col)/float(iCols-1);
            float fScaleR = float(row)/float(iRows-1);

            glm::vec3 pixel = glm::vec3(-0.5f+fScaleC,level/255.0f, -0.5f+fScaleR);

            vVertexData[row][col] = pixel;
            vCoordsData[row][col] = glm::vec2(fTextureU*fScaleC, fTextureV*fScaleR);

        }
   }



}

void HeightMap::calculateTriangleNormals() {

    cout << "Calculating Triangle Normals" << endl;

    // Calculate triangle normals
    vector< vector<glm::vec3> > vNormals[2];
     for (int i=0; i<2; i++) {
         vNormals[i] = vector< vector<glm::vec3> >(iRows-1, vector<glm::vec3>(iCols-1));
     }

     for (int i=0; i<iRows-1; i++)
     {
        for (int j=0; j<iCols-1; j++)
        {
           glm::vec3 vTriangle0[] =
           {
              // Lower left Triangle
              vVertexData[i][j],       //   i,j
              vVertexData[i+1][j],     //
              vVertexData[i+1][j+1]    //   i+1,j     i+1, j+1
           };
           glm::vec3 vTriangle1[] =
           {
              // Upper Right Triangle
              vVertexData[i+1][j+1], //    i,j    i, j+1
              vVertexData[i][j+1],   //
              vVertexData[i][j]      //          i+1, j+1
           };

           glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0]-vTriangle0[1], vTriangle0[1]-vTriangle0[2]);
           glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0]-vTriangle1[1], vTriangle1[1]-vTriangle1[2]);

           vNormals[0][i][j] = glm::normalize(vTriangleNorm0);
           vNormals[1][i][j] = glm::normalize(vTriangleNorm1);

        }
     }

     cout << "Calculating Vertex Normals" << endl;

     // Calculate Vertex normals
     this->vFinalNormals = vector< vector<glm::vec3> >(iRows, vector<glm::vec3>(iCols));;

       for (int i=0; i<iRows; i++) {
          for (int j=0; j<iCols; j++)
           {
              // Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
              // of all adjacent triangles' normals

              glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);

              // Look for upper-left triangles
              if(j != 0 && i != 0)
                 for (int k=0; k<2; k++ ) vFinalNormal += vNormals[k][i-1][j-1];
              // Look for upper-right triangles
              if(i != 0 && j != iCols-1) vFinalNormal += vNormals[0][i-1][j];
              // Look for bottom-right triangles
              if(i != iRows-1 && j != iCols-1)
                 for (int k=0; k<2; k++)vFinalNormal += vNormals[k][i][j];
              // Look for bottom-left triangles
              if(i != iRows-1 && j != 0)
                 vFinalNormal += vNormals[1][i][j-1];
              vFinalNormal = glm::normalize(vFinalNormal);

              vFinalNormals[i][j] = vFinalNormal; // Store final normal of j-th vertex in i-th row
           }
       }

       cout << "Finished" << endl;
}

void HeightMap::createBuffers() {


    data.clear();
    indices.clear();

    cout << "Creating Buffers" << endl;


    // First, create a VBO with only vertex data

    // Create VBO
    glGenBuffers(1, &vboVerticesData);
    iSize = iRows*iCols*(2*sizeof(glm::vec3)+sizeof(glm::vec3));
    iCurrentSize = 0;
    data.reserve(iSize);

    for (int i=0; i<iRows;i++)
    {
       for (int j=0; j<iCols; j++)
       {
            data.insert(data.end(), (BYTE*)&vVertexData[i][j], (BYTE*)&vVertexData[i][j]+sizeof(glm::vec3));
            data.insert(data.end(), (BYTE*)&vCoordsData[i][j], (BYTE*)&vCoordsData[i][j]+sizeof(glm::vec2));
            data.insert(data.end(), (BYTE*)&vFinalNormals[i][j], (BYTE*)&vFinalNormals[i][j]+sizeof(glm::vec3));
       }
    }

    GLuint primitiveRestartIndex = iRows*iCols;

    int debug = 0;
    glGenBuffers(1, &vboIndexData);



    for (int i=0; i<iRows;i++)
    {
       for (int j=0; j<iCols; j++)
       {
           for (int k=0; k<2; k++) {
                int iRow = i+(1-k);
                GLuint iIndex = iRow*iCols+j;
                indices.insert(indices.end(), (BYTE*) &iIndex, ((BYTE*) &iIndex)+sizeof(GLuint));
           }
       }
       // Restart triangle strips...
       indices.insert(indices.end(), (BYTE*) &primitiveRestartIndex, ((BYTE*) &primitiveRestartIndex)+sizeof(GLuint));
    }

    glGenVertexArrays(1, &uiVAO);
    glBindVertexArray(uiVAO);

    // Upload Vertex data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesData);
    glBufferData(GL_ARRAY_BUFFER, data.size(), &data[0], GL_STATIC_DRAW);

    // Vertex Coordinates...
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+1*sizeof(glm::vec2), 0);

    // Texture  Coordinates...
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+1*sizeof(glm::vec2), (void*)sizeof(glm::vec3));

    // Normal Vectors...
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+1*sizeof(glm::vec2), (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));


    // Upload Indices data to GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndexData);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

}


void HeightMap::printOpenGLErrors() {

    GLenum error;

    do {
        error = glGetError();
        switch (error) {
         case GL_INVALID_ENUM : cout << "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag." << endl; break;
         case GL_INVALID_VALUE : cout << "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag." << endl; break;
         case GL_INVALID_OPERATION : cout << "The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag." << endl; break;
         case GL_INVALID_FRAMEBUFFER_OPERATION: cout << "The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag." << endl; break;
         case GL_OUT_OF_MEMORY: cout << "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded." << endl; break;
         case GL_STACK_UNDERFLOW: cout << "An attempt has been made to perform an operation that would cause an internal stack to underflow." << endl; break;
         case GL_STACK_OVERFLOW : cout << "An attempt has been made to perform an operation that would cause an internal stack to overflow." << endl; break;
        }
    }
    while (error!=GL_NO_ERROR);

   // assert(glGetError()==GL_NO_ERROR);
}

void HeightMap::render() {

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(uiVAO);
    glUseProgram(this->shaderProgramId);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glUniformMatrix4fv(this->matrixUniformId, 1, GL_FALSE,  glm::value_ptr(modelViewMatrix));

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(iRows*iCols);

    int iSamplerLoc = glGetUniformLocation(shaderProgramId, "gSampler");
    glUniform1i(iSamplerLoc, 0);
    heightMapTexture.bindTexture(0);

    // Set Directional Light
    glm::vec3 sunLightColor(1.0f, 1.0f, 1.0f);

    glm::vec3 sunLightDirection(1.0f, 1.0f, 1.0f);

    glm::vec4 temp = glm::vec4(sunLightDirection, 1.0)*this->modelViewMatrix;
    sunLightDirection.x = temp.x;
    sunLightDirection.y = temp.y;
    sunLightDirection.z = temp.z;


    GLfloat sunLightAmbientIntensity = 0.25f;

    int sunLightVColorLoc = glGetUniformLocation(shaderProgramId, "sunLight.vColor");
    glUniform3fv(sunLightVColorLoc, 1, (GLfloat*)&sunLightColor);

    int sunLightAmbientIntensityLoc = glGetUniformLocation(shaderProgramId, "sunLight.fAmbientIntensity");
    glUniform1fv(sunLightAmbientIntensityLoc, 1, (GLfloat*)&sunLightAmbientIntensity);

    int sunLightVDirectionLoc = glGetUniformLocation(shaderProgramId, "sunLight.vDirection");
    glUniform3fv(sunLightVDirectionLoc, 1, (GLfloat*)&sunLightDirection);






    // glDrawArrays(GL_POINTS, 0, iRows*iCols);

    int iNumIndices = (iRows-1)*iCols*2 + iRows-1;

    glDrawElements(GL_TRIANGLE_STRIP, iNumIndices, GL_UNSIGNED_INT, 0);

    glUseProgram(0);
    glBindVertexArray(0);


    /*
    glBindVertexArray(uiVAO);

   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndexData);



    //cout << "Num:" << iNumIndices << endl;
    glDrawElements(GL_TRIANGLE_STRIP, iNumIndices, GL_UNSIGNED_INT, 0);
    */

}
