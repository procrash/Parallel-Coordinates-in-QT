#include "heightmap.h"
#include <QImage>

#include <iostream>


#include <glm/vec3.hpp>
#include <glm/glm.hpp>

/*
#include "3rdParty/glm/glm/vec3.hpp"
#include "3rdParty/glm/glm/glm.hpp"
*/

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLContext>


#include <vector>

using namespace std;

HeightMap::HeightMap()
{
    loadVertexData();
    calculateTriangleNormals();
    createBuffers();
}

void HeightMap::loadVertexData() {

    cout << "Loading Vertex Data" << endl;

    QImage img(":/images/heightmap.jpg");

    iRows = img.height();
    iCols = img.width();



    vVertexData1D = new glm::vec3[iRows+1*iCols+1];// (glm::vec3**) malloc(sizeof(glm::vec3)*(iRows+2)*(iCols+2));

    vVertexData = (glm::vec3**) new glm::vec3[iCols+1];

    for (int i=0; i<iCols+1; i++) {
        vVertexData[i] = &vVertexData1D[0]+sizeof(glm::vec3)*(iRows+1);
    }

    for ( int row = 0; row < iRows ; ++row ) {
        for ( int col = 0; col < iCols; ++col )
        {
            QColor clrCurrent( img.pixel( row, col ) );

            float red = clrCurrent.redF();
            float green = clrCurrent.greenF();
            float blue = clrCurrent.blueF();

//            float level = 0.299 * red + 0.587 * green + 0.114 * blue;
            float level = (red + green + blue)/(255.0f*3);
            if (level>255.0f) level=255.0f;

            float fScaleC = float(col)/float(iCols-1);
            float fScaleR = float(row)/float(iRows-1);

            glm::vec3 pixel = glm::vec3(-0.5f+fScaleC,level/255.0f, -0.5f+fScaleR);


            // cout << "Row:" << row << " Col:" << col << endl;
            vVertexData[row][col] = pixel;

            /*
            std::cout << "Pixel at [" << row << "," << col << "] contains color ("
                      << clrCurrent.red() << ", "
                      << clrCurrent.green() << ", "
                      << clrCurrent.blue() << ", "
                      << clrCurrent.alpha() << ")."
                      << std::endl;
            */
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
              vVertexData[i][j],
              vVertexData[i+1][j],
              vVertexData[i+1][j+1]
           };
           glm::vec3 vTriangle1[] =
           {
              vVertexData[i+1][j+1],
              vVertexData[i][j+1],
              vVertexData[i][j]
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
//     vector< vector<glm::vec3> > vFinalNormals = vector< vector<glm::vec3> >(iRows, vector<glm::vec3>(iCols));

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
    cout << "Crating Buffers" << endl;

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
//           cout << "3 Rows:" << iRows << " Cols:" << iCols << endl;

            data.insert(data.end(), (BYTE*)&vVertexData[i][j], (BYTE*)&vVertexData[i][j]+sizeof(glm::vec3));
            //data.insert(data.end(), (BYTE*)&vCoordsData[i][j], (BYTE*)&vCoordsData[i][j]+sizeof(glm::vec2));
            // data.insert(data.end(), (BYTE*)&vFinalNormals[i][j], (BYTE*)&vFinalNormals[i][j]+sizeof(glm::vec3));

       }
    }

    glGenBuffers(1, &vboIndexData);
    for (int i=0; i<iRows;i++)
    {
       for (int j=0; j<iCols; j++)
       {
           for (int k=0; k<2; k++) {
                int iRow = i+(1-k);
                GLuint iIndex = iRow*iCols+j;
                indices.insert(indices.end(), iIndex);
           }
           // Restart triangle strips...
           indices.insert(indices.end(), iRows*iCols);
       }
    }

    glGenVertexArrays(1, &uiVAO);
    glBindVertexArray(uiVAO);

    // Upload Vertex data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesData);
    glBufferData(GL_ARRAY_BUFFER, data.size(), &data[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 1*sizeof(glm::vec3), 0);

    // Upload Indices data to GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndexData);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices[0], GL_STATIC_DRAW);


}

void HeightMap::render() {
    glBindVertexArray(uiVAO);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(iRows*iCols);
    int iNumIndices = (iRows-1)*iCols*2 + iRows-1;
    glDrawElements(GL_TRIANGLE_STRIP, iNumIndices, GL_UNSIGNED_INT, 0);
}
