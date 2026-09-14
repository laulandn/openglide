/*
  This rotates point x,y,z around angles xa,ya,za
*/
void Rotate3D (float *x, float *y, float *z, float xa, float ya, float za)
{
  float lx = *x, ly = *y, lz = *z;
  float xt, yt, zt;

  //Rotate around X-axis
  zt = (float)(lz * cos (xa) - ly * sin (xa));
  yt = (float)(lz * sin (xa) + ly * cos (xa));
  lz = zt; ly = yt;

  //Rotate around Z-axis
  xt = (float)(lx * cos (za) - ly * sin (za));
  yt = (float)(lx * sin (za) + ly * cos (za));
  lx = xt;

  //Rotate around Y-axis
  xt = (float)(lx * cos (ya) - lz * sin (ya));
  zt = (float)(lx * sin (ya) + lz * cos (ya));

  *x = xt;
  *y = yt;
  *z = zt;
}

//This animates a rotating cube every time it is called
void RotateCube ()
{
  //8 vertices
  float x [8], y [8], z [8];
  int i;

  //Set coordinates to that of a cube
  y [0] = y [1] = y [2] = y [3] = -50; //Top
  y [4] = y [5] = y [6] = y [7] =  50; //Bottom
  x [0] = x [3] = x [4] = x [7] = -50; //Left
  x [1] = x [2] = x [5] = x [6] =  50; //Right
  z [0] = z [1] = z [4] = z [5] = -50; //Back
  z [2] = z [3] = z [6] = z [7] =  50; //Front

  //Rotate over the angles
  for (i = 0; i < 8; i++)
    Rotate3D (&x [i], &y [i], &z [i], xAngle, yAngle, zAngle);

  //Perform the perspective transformation
  for (i = 0; i < 8; i++) {
    z [i] += 200;
    x [i] *= 400; x [i] /= z [i];
    y [i] *= 400; y [i] /= z [i];
  }

  //Draw the cube, the sides use the brick texture
  UseTexture (BrickStart, BrickInfo);
    //Left
    DrawPoly (x [0], y [0], z [0], x [3], y [3], z [3],
              x [7], y [7], z [7], x [4], y [4], z [4]);
    //Right
    DrawPoly (x [2], y [2], z [2], x [1], y [1], z [1],
              x [5], y [5], z [5], x [6], y [6], z [6]);
    //Back
    DrawPoly (x [1], y [1], z [1], x [0], y [0], z [0],
              x [4], y [4], z [4], x [5], y [5], z [5]);
    //Front
    DrawPoly (x [3], y [3], z [3], x [2], y [2], z [2],
              x [6], y [6], z [6], x [7], y [7], z [7]);
  //The top and bottom using another texture
  UseTexture (WoodStart, WoodInfo);
    //Top
    DrawPoly (x [0], y [0], z [0], x [1], y [1], z [1],
              x [2], y [2], z [2], x [3], y [3], z [3]);
    //Bottom
    DrawPoly (x [4], y [4], z [4], x [5], y [5], z [5],
              x [6], y [6], z [6], x [7], y [7], z [7]);

  //Set new angles
  xAngle += 0.01f; yAngle += 0.02f; zAngle += 0.03f;
}