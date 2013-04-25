#ifndef CORE_INCLUDE_H
#define CORE_INCLUDE_H

/** Conveniance define for the core */
#define FLYGAME_CORE
/** Core version defenition */
#define CORE_VERSION	1_0

/** 
*	Include files to use the core.
*/


#include "D3DShell.h"
#include "IShader.h"
#include "Entity.h"
#include "Mesh\Object.h"
#include "Mesh\MaterialHandler.h"
#include "Mesh\RawObjectData.h"
#include "Mesh\FullScreenQuad.h"
#include "Render\GBufferShader.h"
#include "Render\ColorShader.h"
#include "Render\LightShader.h"
#include "..\Util\FlyCamera.h"
#include "..\Util\Importer\ResourceImporter.h"



#endif

