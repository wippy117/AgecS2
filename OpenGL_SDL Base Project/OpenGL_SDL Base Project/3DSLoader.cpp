#include <stdio.h>
#include <stdlib.h>
#include "Commons.h"
#include "3DSLoader.h"
#include <sys\stat.h>

long filelength(int f)
{
	struct stat buf;

	fstat(f, &buf);

	return(buf.st_size);
}

char Load3DS(obj_type_ptr p_object, char *p_filename)
{
	int i;															
	FILE *inFile;													

	unsigned short chunkId, s_temp;									
	int chunkLength;												
	unsigned char name;												
	unsigned short size;											

	unsigned short faceFlag;										
	if ((inFile = fopen(p_filename, "rb")) == NULL)					
		return 0;

	while (ftell(inFile) < filelength(fileno(inFile)))				
	{
		fread(&chunkId, 2, 1, inFile);								
		fread(&chunkLength, 4, 1, inFile);							

		switch (chunkId)
		{
		case 0x4d4d:												
			break;													

		case 0x3d3d:												
			break;													

		case 0x4000:												
			i = 0;													
			do
			{
				fread(&name, 1, 1, inFile);
				p_object->name[i] = name;
				i++;
			} while (name != '\0' && i < 20);
			break;

		case 0x4100:												
			break;													

		case 0x4110:
			fread(&size, sizeof(unsigned short), 1, inFile);		
			p_object->vertices_qty = size;							

			for (i = 0; i < size; i++)
			{
				fread(&p_object->vertex[i].x, sizeof(float), 1, inFile);
				fread(&p_object->vertex[i].y, sizeof(float), 1, inFile);
				fread(&p_object->vertex[i].z, sizeof(float), 1, inFile);
			}
			break;

		case 0x4120:												
			fread(&size, sizeof(unsigned short), 1, inFile);		
			p_object->triangles_qty = size;

			for (i = 0; i < size; i++)
			{
				fread(&s_temp, sizeof(unsigned short), 1, inFile);
				p_object->triangles[i].a = s_temp;

				fread(&s_temp, sizeof(unsigned short), 1, inFile);
				p_object->triangles[i].b = s_temp;

				fread(&s_temp, sizeof(unsigned short), 1, inFile);
				p_object->triangles[i].c = s_temp;

				fread(&faceFlag, sizeof(unsigned short), 1, inFile);
			}
			break;

		case 0x4140:
			fread(&size, sizeof(unsigned short), 1, inFile);
			for (i = 0; i < size; i++)
			{
				fread(&p_object->mapcoord[i].u, sizeof(float), 1, inFile);
				fread(&p_object->mapcoord[i].v, sizeof(float), 1, inFile);
			}
			break;

		default:
			fseek(inFile, chunkLength - 6, SEEK_CUR);
		}
	}

	fclose(inFile);
	return(1);
}