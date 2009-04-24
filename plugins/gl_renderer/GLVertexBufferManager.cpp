#include "PCH.h"
#include "GLVertexBufferManager.h"


bool IsExtensionSupported( char* szTargetExtension )
{
	const unsigned char *pszExtensions = NULL;
	const unsigned char *pszStart;
	unsigned char *pszWhere, *pszTerminator;

	pszWhere = (unsigned char *) strchr( szTargetExtension, ' ' );
	if( pszWhere || *szTargetExtension == '\0' )
		return false;

	// 返回扩展字符串
	pszExtensions = glGetString( GL_EXTENSIONS );

	// 在扩展字符串中搜索
	pszStart = pszExtensions;
	for(;;)
	{
		pszWhere = (unsigned char *) strstr( (const char *) pszStart, szTargetExtension );
		if( !pszWhere )
			break;
		pszTerminator = pszWhere + strlen( szTargetExtension );
		if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' )
			if( *pszTerminator == ' ' || *pszTerminator == '\0' )
				//如果存在返回True
				return true;
		pszStart = pszTerminator;
	}
	return false;
}


HARE_IMPLEMENT_SINGLETON(GLVertexBufferManager)

GLVertexBufferManager::GLVertexBufferManager()
    :arrayCount(0)
    ,VBOVertex(0)
    ,VBOTexCoord(0)
    ,VBOColor(0)
{
	// NB: GL_STATIC_DRAW_ARB and GL_STREAM_DRAW_ARB may use video memory
	//     GL_DYNAMIC_DRAW_ARB may use AGP memory

	vertexArray   = new float[VERTEX_BUFFER_COUNT * 3];
    texCoordArray = new float[VERTEX_BUFFER_COUNT * 2];
	colorArray    = new uint8[VERTEX_BUFFER_COUNT * 4];

	//glGenBuffersARB(1, &VBOVertex);
	//glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBOVertex);
	//glBufferDataARB(GL_ARRAY_BUFFER_ARB, VERTEX_BUFFER_COUNT * 3 * sizeof(float), vertexArray, GL_STREAM_DRAW_ARB);

	//glGenBuffersARB(1, &VBOTexCoord);
	//glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBOTexCoord );
	//glBufferDataARB(GL_ARRAY_BUFFER_ARB, VERTEX_BUFFER_COUNT * 2 * sizeof(float), texCoordArray, GL_STREAM_DRAW_ARB);

	//glGenBuffersARB(1, &VBOColor);
	//glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBOColor);
	//glBufferDataARB(GL_ARRAY_BUFFER_ARB, VERTEX_BUFFER_COUNT * 4 * sizeof(uint8), colorArray, GL_STREAM_DRAW_ARB);

}

GLVertexBufferManager::~GLVertexBufferManager()
{
    //glDeleteBuffersARB(1, &VBOVertex);
    //glDeleteBuffersARB(1, &VBOTexCoord);
    //glDeleteBuffersARB(1, &VBOColor);
	delete [] vertexArray;
	delete [] texCoordArray;
	delete [] colorArray;
}

void GLVertexBufferManager::writeBuffer(Vertex* buffer, uint32 count)
{
	Color color;
	for (uint32 i = 0; i < count; ++i){
        vertexArray[arrayCount * 3 + 0] = buffer[i].x;
		vertexArray[arrayCount * 3 + 1] = buffer[i].y;
		vertexArray[arrayCount * 3 + 2] = buffer[i].z;
        texCoordArray[arrayCount * 2 + 0] = buffer[i].u;
        texCoordArray[arrayCount * 2 + 1] = buffer[i].v;
        color = buffer[i].diffuse;
        colorArray[arrayCount * 4 + 0] = (uint8)(255 * color.R);
        colorArray[arrayCount * 4 + 1] = (uint8)(255 * color.G);
        colorArray[arrayCount * 4 + 2] = (uint8)(255 * color.B);
        colorArray[arrayCount * 4 + 3] = (uint8)(255 * color.A);
		arrayCount++;
	}
}

float* GLVertexBufferManager::getVertexArray()
{
	return vertexArray;
}

float* GLVertexBufferManager::getTexCoordArray()
{
	return texCoordArray;
}

uint8* GLVertexBufferManager::getColorArray()
{
	return colorArray;
}

uint32 GLVertexBufferManager::getVertexVBO()
{
	return VBOVertex;
}

uint32 GLVertexBufferManager::getTexCoordVBO()
{
	return VBOTexCoord;
}

uint32 GLVertexBufferManager::getColorVBO()
{
	return VBOColor;
}

uint32 GLVertexBufferManager::getArrayCount()
{
	return arrayCount;
}
