#ifndef EXE_FILEPATH
#error "You must define EXE_FILEPATH macro before include this file"
#endif

// creation

#define PARM_NODE_CLASS Extrude
#define PARM_NODE_NAME extrude
#include EXE_FILEPATH

#define PARM_NODE_CLASS Insert
#define PARM_NODE_NAME i
#include EXE_FILEPATH

#define PARM_NODE_CLASS PrimCube
#define PARM_NODE_NAME prim_cube
#include EXE_FILEPATH

#define PARM_NODE_CLASS PrimQuad
#define PARM_NODE_NAME prim_quad
#include EXE_FILEPATH

// subdivision

#define PARM_NODE_CLASS Comp
#define PARM_NODE_NAME comp
#include EXE_FILEPATH

#define PARM_NODE_CLASS Offset
#define PARM_NODE_NAME offset
#include EXE_FILEPATH

#define PARM_NODE_CLASS ShapeO
#define PARM_NODE_NAME shapeo
#include EXE_FILEPATH

#define PARM_NODE_CLASS Split
#define PARM_NODE_NAME split
#include EXE_FILEPATH

// transformations

#define PARM_NODE_CLASS Scale
#define PARM_NODE_NAME s
#include EXE_FILEPATH

#define PARM_NODE_CLASS TransScope
#define PARM_NODE_NAME t
#include EXE_FILEPATH

// flow control

#define PARM_NODE_CLASS NIL
#define PARM_NODE_NAME NIL
#include EXE_FILEPATH

#define PARM_NODE_CLASS Switch
#define PARM_NODE_NAME switch
#include EXE_FILEPATH

// attributes

#define PARM_NODE_CLASS Color
#define PARM_NODE_NAME color
#include EXE_FILEPATH
