#include "SceneCollisionAvoidance.h"
#include "Obstacle.h"
using namespace std;
using namespace Vector2DUtils;

SceneCollisionAvoidance::SceneCollisionAvoidance()
{
	Agent *agent = new Agent;
	agent->setPosition(Vector2D(640, 360));
	agent->setTarget(Vector2D(640, 360));
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agents.push_back(agent);
	target = Vector2D(640, 360);


	Agent *obstacle = new Agent;
	obstacle->setPosition(Vector2D(300, 300));
	//obstacle->setTarget(Vector2D(640, 360));
	obstacle->loadSpriteTexture("../res/zombie1.png", 8);
	obstacles.push_back(obstacle);
	//target = Vector2D(640, 360);

	Agent *obstacle2 = new Agent;
	obstacle2->setPosition(Vector2D(600, 300));
	//obstacle->setTarget(Vector2D(640, 360));
	obstacle2->loadSpriteTexture("../res/zombie2.png", 8);
	obstacles.push_back(obstacle2);
	//target = Vector2D(640, 360);

	Agent *obstacle3 = new Agent;
	obstacle3->setPosition(Vector2D(900, 300));
	//obstacle->setTarget(Vector2D(640, 360));
	obstacle3->loadSpriteTexture("../res/rock.png", 1);
	obstacles.push_back(obstacle3);
	//target = Vector2D(640, 360);

}

SceneCollisionAvoidance::~SceneCollisionAvoidance()
{
	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
	for (int i = 0; i < (int)obstacles.size(); i++)
	{
		delete obstacles[i];
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
	

	/*for each (Agent* x in obstacles)
	{
		Vector2D steering_force = x->Behavior()->Wander(x, dtime);
		x->update(steering_force, dtime, event);
	}*/
	
	
	
	
	for each (Agent* x in obstacles)
	{
		if (IsInsideCone(x->getPosition(), agents[0]->getPosition(), agents[0]->getPosition() + (agents[0]->getVelocity().Normalize() * 200), 30)) {
			
			float currDist = Vector2D().Distance(agents[0]->getPosition(), x->getPosition());
			if (currDist < shortestDist) {
				nearestTarget = x;
				shortestDist = currDist;
				colDetec = true;
			}
			else if(currDist > shortestDist)
			{
				colDetec = false;
			}
		}	
	}
	
	if (!colDetec) {
		Vector2D steering_force = agents[0]->Behavior()->Seek(agents[0], agents[0]->getTarget(), dtime);
		agents[0]->update(steering_force, dtime, event);
	}
	else if (colDetec) {
		
		Vector2D steering_force = agents[0]->Behavior()->Flee(agents[0], nearestTarget->getPosition(), dtime);
		agents[0]->update(steering_force, dtime, event);
	}
	
	
}

void SceneCollisionAvoidance::draw()
{
	Vector2D raycastVector = agents[0]->getPosition();
	raycastVector += (agents[0]->getVelocity().Normalize() * 200);



	draw_circle(TheApp::Instance()->getRenderer(), (int)target.x, (int)target.y, 15, 255, 0, 0, 255);

	SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), agents[0]->getPosition().x, agents[0]->getPosition().y, raycastVector.x, raycastVector.y);
	

	agents[0]->draw();
	for each (Agent* x in obstacles)
	{
		x->draw();
		draw_circle(TheApp::Instance()->getRenderer(), x->getPosition().x, x->getPosition().y, 40, 255, 0, 0, 255);
	}
}

const char* SceneCollisionAvoidance::getTitle()
{
	return "SDL Steering Behaviors :: SceneCollisionAvoidance Demo";
}