#include "SceneCollisionAvoidance.h"
#include "Obstacle.h"
using namespace std;

SceneCollisionAvoidance::SceneCollisionAvoidance()
{
	Agent *agent = new Agent;
	agent->setPosition(Vector2D(640, 360));
	agent->setTarget(Vector2D(640, 360));
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agents.push_back(agent);
	target = Vector2D(640, 360);

	Agent *zombie = new Agent;
	zombie->setPosition(Vector2D(600, 300));
	zombie->setMass(0.5);
	//zombie->setTarget(Vector2D(600, 300));
	zombie->loadSpriteTexture("../res/zombie2.png", 8);
	agents.push_back(zombie);

	//Obstacle *obstacle = new Obstacle;
	//obstacle->setPosition(Vector2D(600, 300));
	////zombie->setTarget(Vector2D(600, 300));
	//obstacle->loadSpriteTexture("../res/rock.png", 1);
	////obstacle.push_back(obstacle);
}

SceneCollisionAvoidance::~SceneCollisionAvoidance()
{
	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void SceneCollisionAvoidance::update(float dtime, SDL_Event *event)
{
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			target = Vector2D((float)(event->button.x), (float)(event->button.y));
			agents[0]->setTarget(target);
		}
		break;

	default:
		break;
	}
	

	Vector2D steering_force = agents[0]->Behavior()->Seek(agents[0], agents[0]->getTarget(), dtime);
	agents[0]->update(steering_force, dtime, event);

	Vector2D steering_force2 = agents[1]->Behavior()->Flee(agents[1], agents[0], dtime);
	agents[1]->update(steering_force2, dtime, event);
}

void SceneCollisionAvoidance::draw()
{
	Vector2D raycastVector = agents[0]->getPosition();
	raycastVector += agents[0]->getVelocity().Normalize() * 200;

	draw_circle(TheApp::Instance()->getRenderer(), (int)target.x, (int)target.y, 15, 255, 0, 0, 255);

	SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), agents[0]->getPosition().x, agents[0]->getPosition().y, raycastVector.x, raycastVector.y);
	draw_circle(TheApp::Instance()->getRenderer(), 300, 300, 40, 255, 0, 0, 255);

	agents[0]->draw();
	agents[1]->draw();
}

const char* SceneCollisionAvoidance::getTitle()
{
	return "SDL Steering Behaviors :: SceneCollisionAvoidance Demo";
}