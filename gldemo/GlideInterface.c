#include <stdio.h>
#include <stdlib.h>

//Function prototypes
int  InitGlide ();
void SolidColour (GrColor_t colour);
void Textured ();
void UseTexture (FxU32 startAddress, GrTexInfo info);
void LoadTexture (char *Filename, int Size, int *Start, GrTexInfo *Info);
void DrawPoly (float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4);

/*
  This procedure initializes the Glide library and 
  sets the correct modes.
*/
int InitGlide ()
{
  //For detecting the Voodoo hardware
  GrHwConfiguration hw;
  //For fogging
  GrFog_t FogTable [GR_FOG_TABLE_SIZE];

  //Initialize Glide library
  grGlideInit ();
  //If there is no 3Dfx card present, don't run
  if (!grSstQueryHardware(&hw)) {
#ifdef WIN32
	MessageBox (NULL, "ERROR: no Voodoo Graphics!", NULL, MB_OK | MB_ICONERROR);
#else
    fprintf(stderr,"ERROR: no Voodoo Graphics!\n"); fflush(stderr);
#endif
    return 0;
  }

  //Select the first Voodoo card found (there may be more than one)
  grSstSelect (0);

  //Set to use full screen 640x480, 60Hz, ARGB byte order, with the origin in the top left of the screen, double buffering, z buffering
  grSstWinOpen ((FxU32)0/*WindowHandle*/, GR_RESOLUTION_640x480, GR_REFRESH_60Hz, GR_COLORFORMAT_ARGB, GR_ORIGIN_UPPER_LEFT, 2, 1);

  //Render to backbuffer  
  grRenderBuffer(GR_BUFFER_BACKBUFFER);

  //Set up render state
  grTexCombine (GR_TMU0, GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, FXFALSE, FXFALSE);
  Textured ();

  //Enable z buffering
  grDepthBufferMode (GR_DEPTHBUFFER_ZBUFFER);
  grDepthBufferFunction (GR_CMP_GREATER);
  grDepthMask (FXTRUE);

  //Don't display pixels without alpha
  grAlphaTestFunction (GR_CMP_GREATER);
  grAlphaTestReferenceValue (1);

  //Enable bilinear filtering, no mipmapping
  grTexFilterMode (GR_TMU0, GR_TEXTUREFILTER_BILINEAR, GR_TEXTUREFILTER_BILINEAR);
  grTexMipMapMode (GR_TMU0, GR_MIPMAP_DISABLE, FXFALSE );

  //Enable fogging to black
  grFogColorValue (0xff000000);
  guFogGenerateExp (FogTable, .005f);
  grFogMode (GR_FOG_WITH_TABLE);
  grFogTable (FogTable);

  //Store start address
  MinAddr = grTexMinAddress (GR_TMU0);

  //Successfull
  return 1;
}

/*
  This procedure sets up rendering using a solid colour, 
  not used in this demo, but included for completeness.
*/
void SolidColour (GrColor_t colour)
{
  grColorCombine (GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE, GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_CONSTANT, FXFALSE);
  grAlphaCombine (GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE, GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_CONSTANT, FXFALSE);
  grConstantColorValue (colour);
}

/*
  This procedure sets up rendering with texture mapping.
  To choose the actual texture, see UseTexture below.
*/
void Textured ()
{
  grColorCombine (GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE, GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_TEXTURE, FXFALSE);
  grAlphaCombine (GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE, GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_TEXTURE, FXFALSE);
}

/*
  This sets the 3Dfx card to use a certain texture for all rendering
  operations that follow. So you need to call this one for every
  different texture as you render it.
  This requires the address in Voodoo memory and texture information
  block, as returned by LoadTexture below. 
*/
void UseTexture (FxU32 startAddress, GrTexInfo info)
{
  grTexSource (GR_TMU0, startAddress, GR_MIPMAPLEVELMASK_BOTH, &info); 
}

/*
  This loads a 24bit RAW image and uploads it to the 3Dfx card.
  The size of the picture is in "Size", being 16, 32, 64, 128 or 256
  The procedure also saves the necessary information that allows
  the UseTexture procedure to select the correct texture.
*/
void LoadTexture (char *Filename, int Size, int *Start, GrTexInfo *Info)
{
  //For loading the texture data
  FILE          *RAWFile;
  unsigned char *Bitmap;
  FxU16         *Converted;
  int           o, Lod;

  //Allocate memory
  Bitmap = (char *)malloc(Size * Size * 3);
  if(!Bitmap) {
    fprintf(stderr,"No Bitmap!\n"); fflush(stderr);
  }
  Converted = (FxU16 *)malloc(Size * Size * 2);
  if(!Converted) {
    fprintf(stderr,"No Converted!\n"); fflush(stderr);
  }

  //Load data from file
  RAWFile = fopen (Filename, "rb");
  if(!RAWFile) {
    fprintf(stderr,"No RAWFile!\n"); fflush(stderr);
  }
  fread (Bitmap, 3, Size * Size, RAWFile);
  fclose (RAWFile);

  //Convert to 3Dfx format
  for (o = 0; o < Size * Size; o++)
    Converted [o] = ((Bitmap [o * 3    ] >> 3) << 11) |
                    ((Bitmap [o * 3 + 1] >> 2) <<  5) |
                     (Bitmap [o * 3 + 2] >> 3);

  //Set size
  switch (Size) {
    case  16 : Lod = GR_LOD_16; break;
    case  32 : Lod = GR_LOD_32; break;
    case  64 : Lod = GR_LOD_64; break;
    case 128 : Lod = GR_LOD_128; break;
    case 256 : Lod = GR_LOD_256; break;
  }

  //Get texture info
  Info->smallLod = Lod;
  Info->largeLod = Lod;
  Info->aspectRatio = GR_ASPECT_1x1;
  Info->format = GR_TEXFMT_RGB_565;
  Info->data = Converted;

  //Update memory pointer
  *Start = MinAddr;
  MinAddr += grTexCalcMemRequired (Lod, Lod, GR_ASPECT_1x1, GR_TEXFMT_RGB_565);
  
  //Upload bitmap data to hardware
  grTexDownloadMipMap (GR_TMU0, *Start, GR_MIPMAPLEVELMASK_BOTH, Info);
  
  //Release memory
  free (Bitmap);
  free (Converted);
}

/*
  Draws a foursided polygon through the 3D accelarator
  This takes 4 sets of x, y and z coordinates.
  x and y are both scan converted coordinates, 
    (0, 0) being the center of the screen
  z is the depth, as used for texturing
*/
void DrawPoly (float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4)
{
  //A polygon has 4 vertices
  GrVertex vtx [4];

  //Truncate decimal positions to avoid crashes
  x1 = (float)floor (x1); y1 = (float)floor (y1);
  x2 = (float)floor (x2); y2 = (float)floor (y2);
  x3 = (float)floor (x3); y3 = (float)floor (y3);
  x4 = (float)floor (x4); y4 = (float)floor (y4);

  //Convert coordinates to vertices

  //Top left vertex
  vtx[0].x = x1 + 320.0f;        //x coordinate. on screen
  vtx[0].y = y1 + 240.0f;        //y coordinate, on screen
  vtx[0].ooz = 65535.0f / z1;    //one-over-z coordinate
  vtx[0].oow =     1.0f / z1;    //one-over-w "homogenous" coordinate
  vtx[0].tmuvtx[0].sow =   0.0f; //texture coordinates, being 0, 0 here
  vtx[0].tmuvtx[0].tow =   0.0f; 

  //Top right vertex
  vtx[1].x = x2 + 320.0f;
  vtx[1].y = y2 + 240.0f;
  vtx[1].ooz = 65535.0f / z2;
  vtx[1].oow =     1.0f / z2;
  vtx[1].tmuvtx[0].sow = 255.0f / z2;
  vtx[1].tmuvtx[0].tow =   0.0f;

  //Bottom right vertex
  vtx[2].x = x3 + 320.0f;
  vtx[2].y = y3 + 240.0f;
  vtx[2].ooz = 65535.0f / z3;
  vtx[2].oow =     1.0f / z3;
  vtx[2].tmuvtx[0].sow = 255.0f / z3;
  vtx[2].tmuvtx[0].tow = 255.0f / z3;

  //Bottom left vertex
  vtx[3].x = x4 + 320.0f;
  vtx[3].y = y4 + 240.0f;
  vtx[3].ooz = 65535.0f / z4;
  vtx[3].oow =     1.0f / z4;
  vtx[3].tmuvtx[0].sow =   0.0f;
  vtx[3].tmuvtx[0].tow = 255.0f / z4;

  //Draw 2 triangles to get a four-sided polygon
  guDrawTriangleWithClip (&vtx [0], &vtx [1], &vtx [2]);
  guDrawTriangleWithClip (&vtx [0], &vtx [2], &vtx [3]);
}