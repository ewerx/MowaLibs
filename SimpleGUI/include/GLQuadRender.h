//
//  GLColorQuad.h
//  SubaruAnimPrototype_1
//
//  Created by Victor Martins on 10/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once


#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"


namespace cinder
{
    namespace gl
    {
        class TextureFont;        
        class GlslProg;        
    }
}


class GLQuadRender
{
    enum Attr
    {
        AttrPosition = 0,
        AttrTexCoord = 1,
        AttrColor = 2,
        AttrCount = 3
    };
 
    GLuint vaoId;
    GLuint vboId[3];
    float* vertices;
    float* texcoords;
    float* vertexColors;
    cinder::gl::GlslProg programColor;
    cinder::gl::GlslProg programTex;
    
    ci::Matrix44f worldViewProj;

    
public:
    GLQuadRender()
    {
        Init();
    }
    ~GLQuadRender()
    {
        Release();
    }
    
    void Init()
    {
        static const char *vertexSimpleTex = \
        "#version 150 core\n"
        "uniform mat4 worldViewProjMatrix;\n"
        "layout(location = 0) in vec3 in_Position;\n"
        "layout(location = 1) in vec2 in_TexCoord;\n"
        "layout(location = 2) in vec4 in_Color;\n"
        "out vec2 pass_TexCoord;\n"
        "out vec4 pass_Color;\n"
        "void main(void)\n"
        "{\n"
        "gl_Position = worldViewProjMatrix * vec4(in_Position.x, in_Position.y, 0.0, 1.0);\n"
        "pass_TexCoord = in_TexCoord;\n"
        "pass_Color = in_Color;\n"
        "}";
        static const char *fragmentSimpleTex = \
        "#version 150 core\n"
        "uniform sampler2D colorSampler;\n"
        "in vec2 pass_TexCoord;\n"
        "in vec4 pass_Color;\n"
        "out vec4 out_Color;\n"
        "void main(void)\n"
        "{\n"
        "vec4 texel = texture( colorSampler, pass_TexCoord );\n"
        "out_Color = texel.rgba * pass_Color;\n"
        "}";

        
        static const char *vertexSimpleColor = \
        "#version 150 core\n"
        "uniform mat4 worldViewProjMatrix;\n"
        "layout(location = 0) in vec3 in_Position;\n"
        "layout(location = 1) in vec2 in_TexCoord;\n"
        "layout(location = 2) in vec4 in_Color;\n"
        "out vec2 pass_TexCoord;\n"
        "out vec4 pass_Color;\n"
        "void main(void)\n"
        "{\n"
        "gl_Position = worldViewProjMatrix * vec4(in_Position.x, in_Position.y, 0.0, 1.0);\n"
        "pass_TexCoord = in_TexCoord;\n"
        "pass_Color = in_Color;\n"
        "}";
        static const char *fragmentSimpleColor = \
        "#version 150 core\n"
        "in vec2 pass_TexCoord;\n"
        "in vec4 pass_Color;\n"
        "out vec4 out_Color;\n"
        "void main(void)\n"
        "{\n"
        "out_Color = pass_Color;\n"
        "}";
     
        programColor = ci::gl::GlslProg( vertexSimpleColor, fragmentSimpleColor );
        programTex = ci::gl::GlslProg( vertexSimpleTex, fragmentSimpleTex );
        
        
        ci::Rectf r = ci::Rectf( 0, 0, 100, 100 );
        
        vertices = new float[3*6];	// Vertices for our square        
        vertices[ 0] = r.x1; vertices[ 1] = r.y1; vertices[2] = 0.0; // Bottom left corner
        vertices[ 3] = r.x2; vertices[ 4] = r.y1; vertices[5] = 0.0; // Top left corner
        vertices[ 6] = r.x2; vertices[ 7] = r.y2; vertices[8] = 0.0; // Top Right corner
        vertices[ 9] = r.x1; vertices[10] = r.y1; vertices[11] = 0.0; // Bottom right corner
        vertices[12] = r.x2; vertices[13] = r.y2; vertices[14] = 0.0; // Bottom left corner
        vertices[15] = r.x1; vertices[16] = r.y2; vertices[17] = 0.0; // Top Right corner

        texcoords = new float[2*6];
        texcoords[ 0] = 0; texcoords[ 1] = 0;
        texcoords[ 2] = 1; texcoords[ 3] = 0;
        texcoords[ 4] = 1; texcoords[ 5] = 1;
        texcoords[ 6] = 0; texcoords[ 7] = 0;
        texcoords[ 8] = 1; texcoords[ 9] = 1;
        texcoords[10] = 0; texcoords[11] = 1;

        vertexColors = new float[4*6];	// Vertices for our square        
        vertexColors[ 0] = 0; vertexColors[ 1] = 0; vertexColors[ 2] = 0.0;     vertexColors[3] = 1.0;
        vertexColors[ 4] = 0; vertexColors[ 5] = 0; vertexColors[ 6] = 0.0;     vertexColors[7] = 1.0;
        vertexColors[ 8] = 0; vertexColors[ 9] = 0; vertexColors[10] = 0.0;     vertexColors[11] = 1.0;
        vertexColors[12] = 0; vertexColors[13] = 0; vertexColors[14] = 0.0;     vertexColors[15] = 1.0;
        vertexColors[16] = 0; vertexColors[17] = 0; vertexColors[18] = 0.0;     vertexColors[19] = 1.0;
        vertexColors[20] = 0; vertexColors[21] = 0; vertexColors[22] = 0.0;     vertexColors[23] = 1.0;


        glGenVertexArrays( 1, &vaoId );
        glBindVertexArray( vaoId );
        
        glGenBuffers( AttrCount, vboId );
        //
        glBindBuffer( GL_ARRAY_BUFFER, vboId[AttrPosition] );
        glBufferData( GL_ARRAY_BUFFER, 3*6*sizeof(float), vertices, GL_STREAM_DRAW );
        glVertexAttribPointer( (GLuint)AttrPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 ); 
        //
        glBindBuffer( GL_ARRAY_BUFFER, vboId[AttrTexCoord] );
        glBufferData( GL_ARRAY_BUFFER, 2*6*sizeof(float), texcoords, GL_STATIC_DRAW );
        glVertexAttribPointer( (GLuint)AttrTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0 ); 
        //
        glBindBuffer( GL_ARRAY_BUFFER, vboId[AttrColor] );
        glBufferData( GL_ARRAY_BUFFER, 4*6*sizeof(float), vertexColors, GL_STREAM_DRAW );
        glVertexAttribPointer( (GLuint)AttrColor, 4, GL_FLOAT, GL_FALSE, 0, 0 ); 
        
        //
        glEnableVertexAttribArray( (GLuint)AttrPosition );
        glEnableVertexAttribArray( (GLuint)AttrTexCoord );
        glEnableVertexAttribArray( (GLuint)AttrColor );
        
        //
        glBindVertexArray( 0 );
    }
    
    
    void Release()
    {
        delete [] vertices;
        delete [] texcoords;
        delete [] vertexColors;
        glDeleteBuffers( 2, vboId );
        glDeleteVertexArrays( 1, &vaoId );
    }
    
    
    void SetMatrices( ci::Matrix44f wvp )
    {
        worldViewProj = wvp;
    }
    
    
    void Draw( ci::Matrix44f wvp, ci::Rectf r, ci::ColorA color, int texId = 0 )
    {        
        vertices[ 0] = r.x1; vertices[ 1] = r.y1; vertices[2] = 0.0; // Bottom left corner
        vertices[ 3] = r.x2; vertices[ 4] = r.y1; vertices[5] = 0.0; // Top left corner
        vertices[ 6] = r.x2; vertices[ 7] = r.y2; vertices[8] = 0.0; // Top Right corner        
        vertices[ 9] = r.x1; vertices[10] = r.y1; vertices[11] = 0.0; // Bottom right corner
        vertices[12] = r.x2; vertices[13] = r.y2; vertices[14] = 0.0; // Bottom left corner
        vertices[15] = r.x1; vertices[16] = r.y2; vertices[17] = 0.0; // Top Right corner
        
        for( int i=0; i<4*6; i+=4 )
        {
            vertexColors[i+0] = color.r;
            vertexColors[i+1] = color.g;
            vertexColors[i+2] = color.b;
            vertexColors[i+3] = color.a;
        }
        
        glBindBuffer( GL_ARRAY_BUFFER, vboId[(GLuint)AttrPosition] );
        glBufferSubData( GL_ARRAY_BUFFER, 0, 3*6*sizeof(float), vertices );
        glBindBuffer( GL_ARRAY_BUFFER, vboId[(GLuint)AttrColor] );
        glBufferSubData( GL_ARRAY_BUFFER, 0, 4*6*sizeof(float), vertexColors );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        
        if( texId > 0 )
        {
            glEnable( GL_TEXTURE_2D );
            glBindTexture( GL_TEXTURE_2D, texId );
            programTex.bind();
            glBindAttribLocation( programTex.getHandle(), (GLuint)AttrPosition, "in_Position" );
            glBindAttribLocation( programTex.getHandle(), (GLuint)AttrTexCoord, "in_TexCoord" );
            glBindAttribLocation( programTex.getHandle(), (GLuint)AttrColor, "in_Color" );
            programTex.uniform( "worldViewProjMatrix", wvp );
            programTex.uniform( "colorSampler", 0 );
        }
        else
        {
            programColor.bind();
            glBindAttribLocation( programColor.getHandle(), (GLuint)AttrPosition, "in_Position" );
            glBindAttribLocation( programColor.getHandle(), (GLuint)AttrTexCoord, "in_TexCoord" );
            glBindAttribLocation( programColor.getHandle(), (GLuint)AttrColor, "in_Color" );
            programColor.uniform( "worldViewProjMatrix", wvp );
        }
        
        glBindVertexArray( vaoId );
        glDrawArrays( GL_TRIANGLES, 0, 6*3 );
        glBindVertexArray( 0 );
        
        
        if( texId > 0 )
        {
            programTex.unbind();
            glBindTexture( GL_TEXTURE_2D, 0 );
        }
        else
        {
            programColor.unbind();
        }
    }
    
};
