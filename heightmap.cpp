#include "heightmap.h"
#include <QImage>

#include <iostream>


#include <glm/vec3.hpp>
#include <glm/glm.hpp>

/*
#include "3rdParty/glm/glm/vec3.hpp"
#include "3rdParty/glm/glm/glm.hpp"
*/

#include <vector>

using namespace std;

HeightMap::HeightMap()
{
    QImage img(":/images/heightmap.jpg");


    vVertexData = (glm::vec3**) malloc(sizeof(glm::vec3)*img.height()*img.width());

    for ( int row = 1; row < img.height() + 1; ++row ) {
        for ( int col = 1; col < img.width() + 1; ++col )
        {
            QColor clrCurrent( img.pixel( row, col ) );

            float red = clrCurrent.redF();
            float green = clrCurrent.greenF();
            float blue = clrCurrent.blueF();

//            float level = 0.299 * red + 0.587 * green + 0.114 * blue;
            float level = (red + green + blue)/(255.0f*3);
            if (level>255.0f) level=255.0f;

            float fScaleC = float(col)/float(img.width()-1);
            float fScaleR = float(row)/float(img.height()-1);

            glm::vec3 pixel = glm::vec3(-0.5f+fScaleC,level/255.0f, -0.5f+fScaleR);

            vVertexData[row][col] = pixel;

            std::cout << "Pixel at [" << row << "," << col << "] contains color ("
                      << clrCurrent.red() << ", "
                      << clrCurrent.green() << ", "
                      << clrCurrent.blue() << ", "
                      << clrCurrent.alpha() << ")."
                      << std::endl;
        }
   }

}

void HeightMap::calculateTriangleNormals() {

    int iRows = 20;
    int iCols = 20;

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

     // Calculate Vertex normals
     vector< vector<glm::vec3> > vFinalNormals = vector< vector<glm::vec3> >(iRows, vector<glm::vec3>(iCols));

       for (int i=0; i<iRows; i++) {
          for (int j=0; i<iCols; j++)
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




}
