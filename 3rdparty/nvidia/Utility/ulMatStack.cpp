/******************************************************************************
ulMatStack.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:



******************************************************************************/
#include <ul.h>

#if 0 /* TODO: Since the header is clearly missing (like alot of stuff) I'll have to define the class myself.  May or may not be worth it though... */

MatrixStack::MatrixStack()
{
  matrix_top = 0;
  matrix = matrix_stack;
  matrix->SetIdentity();

}


void MatrixStack::PushMatrix ()
{
  if (matrix_top < 255)
  {
    matrix_stack[matrix_top + 1] = matrix_stack[matrix_top];
    matrix_top++;
    matrix = &matrix_stack[matrix_top];
  }
}



void MatrixStack::InitMatrixStack ()
{
  matrix_top = 0;
  matrix = matrix_stack;
  matrix->SetIdentity();
}


void MatrixStack::LoadMatrix(Mat4 & mat)
{
  matrix = &mat;
}



void MatrixStack::PopMatrix ()
{
  if (matrix_top > 0)
    matrix_top--;

  matrix = &matrix_stack[matrix_top];
}


Mat4 &MatrixStack::GetMatrix ()
{
  return *matrix;
}





#endif