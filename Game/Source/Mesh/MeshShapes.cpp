#include "GamePCH.h"

#include "Mesh.h"
#include "OBJLoader.h"
#include "Texture.h"
#include "VertexFormat.h"

void Mesh::CreateBox(vec2 size, vec2 offset)
{
    VertexFormat vertexAttributes[] =
    {
        VertexFormat( vec2( -size.x/2, -size.y/2 ) + offset, MyColor( 255, 255, 255, 255 ), vec2( 0.0f, 0.0f ) ),
        VertexFormat( vec2( -size.x/2,  size.y/2 ) + offset, MyColor( 255, 255, 255, 255 ), vec2( 0.0f, 1.0f ) ),
        VertexFormat( vec2(  size.x/2,  size.y/2 ) + offset, MyColor( 255, 255, 255, 255 ), vec2( 1.0f, 1.0f ) ),
        VertexFormat( vec2(  size.x/2, -size.y/2 ) + offset, MyColor( 255, 255, 255, 255 ), vec2( 1.0f, 0.0f ) ),
    };

    unsigned int indices[6] = { 0,1,2, 0,2,3 };

    Init( vertexAttributes, 4, indices, 6, GL_TRIANGLES, GL_STATIC_DRAW );
};

void Mesh::CreatePlane(vec2 worldSize, ivec2 vertexCount, vec2 pivotPoint)
{
    int numVerts = vertexCount.x * vertexCount.y;
    int numIndices = (vertexCount.x - 1) * (vertexCount.y - 1) * 6;

    VertexFormat* verts = new VertexFormat[numVerts];
    unsigned int* indices = new unsigned int[numIndices];

    float stepX = worldSize.x / ( vertexCount.y - 1 );
    float stepY = worldSize.y / ( vertexCount.y - 1 );

    float uvStepX = 5.0f / ( vertexCount.y - 1 );
    float uvStepY = 5.0f / ( vertexCount.y - 1 );

    for( int y = 0; y < vertexCount.y; y++ )
    {
        for( int x = 0; x < vertexCount.x; x++ )
        {
            verts[y * vertexCount.x + x].m_Pos.Set( x * stepX - pivotPoint.x, 0, y * stepY - pivotPoint.y );
            verts[y * vertexCount.x + x].m_UV.Set( x * uvStepX, y * uvStepY );;
        }
    }

    for( int y = 0; y < vertexCount.y - 1; y++ )
    {
        for( int x = 0; x < vertexCount.x - 1; x++ )
        {
            int bottomLeftIndex = y * vertexCount.x + x;
            int elementIndex = ( y * (vertexCount.x-1) + x ) * 6;

            indices[ elementIndex + 0 ] = bottomLeftIndex + 0;
            indices[ elementIndex + 1 ] = bottomLeftIndex + vertexCount.x;
            indices[ elementIndex + 2 ] = bottomLeftIndex + vertexCount.x + 1;

            indices[ elementIndex + 3 ] = bottomLeftIndex + 0;
            indices[ elementIndex + 4 ] = bottomLeftIndex + vertexCount.x + 1;
            indices[ elementIndex + 5 ] = bottomLeftIndex + 1;
        }
    }

    Init( verts, numVerts, indices, numIndices, GL_TRIANGLES, GL_STATIC_DRAW );

    delete[] verts;
    delete[] indices;
}

void Mesh::CreateOBJ(const char* objFilename, vec3 scale)
{
    unsigned int numVerts = 0;
    unsigned int numIndices = 0;
    VertexFormat* verts = 0;
    unsigned int* indices = 0;

    LoadBasicOBJFromFile( objFilename, scale, &numVerts, &numIndices, &verts, &indices );

    Init( verts, numVerts, indices, numIndices, GL_TRIANGLES, GL_STATIC_DRAW );

    delete[] verts;
    delete[] indices;
}
