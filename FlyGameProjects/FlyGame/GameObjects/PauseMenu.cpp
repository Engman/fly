#include "PauseMenu.h"

PauseMenu::PauseMenu()
{

}

PauseMenu::~PauseMenu()
{

}

bool PauseMenu::Initialize(FlyGame* entry, float windowHeight, float windowWidth)
{
	this->windowHeight = windowHeight;
	this->windowWidth = windowWidth;

	vector<IShader*> shaders;
	entry->GetCoreInstance()->Gfx_GetShader(shaders);

	BoundingSphere* sphere = new BoundingSphere();
	sphere->center = vec3(0.0f, 0.0f, -1.0f);
	sphere->radius = 2000.0f;
	
	entry->GetCoreInstance()->Geometry_Load(L"..\\Resources\\Models\\pauseMenu.fgm", &this->models);
	this->models[0]->setScale(vec3(this->windowWidth*0.5f, this->windowHeight*0.5f, 1.0f));
	this->models[0]->setPosition(vec3(0.0f, 0.0f, 0.0f));
	this->models[0]->setShader(shaders[FlyShader_gBufferDefault]);
	this->models[0]->setBoundingSphere(sphere);
	entry->GetCoreInstance()->Geometry_Load(L"..\\Resources\\Models\\pauseMenuHover1.fgm", &this->models);
	this->models[1]->setScale(vec3(this->windowWidth*0.5f, this->windowHeight*0.5f, 1.0f));
	this->models[1]->setPosition(vec3(0.0f, 0.0f, 0.0f));
	this->models[1]->setShader(shaders[FlyShader_gBufferDefault]);
	this->models[1]->setBoundingSphere(sphere);
	entry->GetCoreInstance()->Geometry_Load(L"..\\Resources\\Models\\pauseMenuHover2.fgm", &this->models);
	this->models[2]->setScale(vec3(this->windowWidth*0.5f, this->windowHeight*0.5f, 1.0f));
	this->models[2]->setPosition(vec3(0.0f, 0.0f, 0.0f));
	this->models[2]->setShader(shaders[FlyShader_gBufferDefault]);
	this->models[2]->setBoundingSphere(sphere);

	entry->GetCoreInstance()->Geometry_Load(L"..\\Resources\\Models\\mouse_cursor.fgm", &this->models);
	this->models[3]->setScale(vec3(3.0f, 3.0f, 1.0f));
	this->models[3]->setPosition(vec3(-this->windowWidth*0.165f, 0.0f, 0.0f));
	this->models[3]->setShader(shaders[FlyShader_gBufferDefault]);
	this->models[3]->setBoundingSphere(sphere);
	entry->GetCoreInstance()->Geometry_Load(L"..\\Resources\\Models\\mouse_cursor.fgm", &this->models);
	this->models[4]->setScale(vec3(3.0f, 3.0f, 1.0f));
	this->models[4]->setPosition(vec3(0.0f, 0.0f, 0.0f));
	this->models[4]->setShader(shaders[FlyShader_gBufferDefault]);
	this->models[4]->setBoundingSphere(sphere);
	entry->GetCoreInstance()->Geometry_Load(L"..\\Resources\\Models\\mouse_cursor.fgm", &this->models);
	this->models[5]->setScale(vec3(3.0f, 3.0f, 1.0f));
	this->models[5]->setPosition(vec3(this->windowWidth*0.165f, 0.0f, 0.0f));
	this->models[5]->setShader(shaders[FlyShader_gBufferDefault]);
	this->models[5]->setBoundingSphere(sphere);

	for(int i =0; i<6; i++)
	{
		models.at(i)->Update();
	}
	this->currentRender = this->models[0];

	return true;
}
void PauseMenu::Render(ViewFrustum f, bool one, bool two, bool three)
{
	this->currentRender->Render(f);

	if(one)
		this->models[3]->Render(f);
	if(two)
		this->models[4]->Render(f);
	if(three)
		this->models[5]->Render(f);
}

int PauseMenu::Update(int mouseX, int mouseY)
{
	if((448.0f/1920.0f)*this->windowWidth < mouseX && mouseX < (881.0f/1920.0f)*this->windowWidth && (948.0f/1080.0f)*this->windowHeight < mouseY && mouseY < this->windowHeight)
	{
		this->currentRender = this->models[1];

		if(Input::self()->IsMouseButtonPressed(0))
		{
			return 1;
		}
	}
	else if((999.0f/1920.0f)*this->windowWidth < mouseX && mouseX < (1646.0f/1920.0f)*this->windowWidth && (948.0f/1080.0f)*this->windowHeight < mouseY && mouseY < this->windowHeight)
	{
		this->currentRender = this->models[2];

		if(Input::self()->IsMouseButtonPressed(0))
		{
			return 2;
		}
	}
	else
		this->currentRender = this->models[0];


	return 0;
}

void PauseMenu::Release()
{
	
}