#include "Collision.h"
#include "Collider.h"
#include "Camera.h"

int squareSize = 400;
Vector2 gridSize = Vector2(20, 2);

vector<pair<SDL_Rect, vector<Collider*>>> Collision::grid;

bool Collision::drawGrid = false;

SDL_Rect tempRec;

void Collision::Init()
{
	int cols = static_cast<int>(gridSize.x);
	int rows = static_cast<int>(gridSize.y);

	for (int x = 0; x < cols; x++)
	{
		for (int y = 0; y < rows; y++)
		{
			SDL_Rect rect;

			rect.x = x * squareSize;
			rect.y = y * squareSize;
			rect.w = squareSize;
			rect.h = squareSize;

			grid.push_back(pair<SDL_Rect, vector<Collider*>>(rect, vector<Collider*>()));
		}
	}
}

void Collision::DebugDraw()
{
	if (Collision::drawGrid)
	{
		for (auto& g : grid)
		{
			tempRec.x = g.first.x - EngineCore::camera->offset.x;
			tempRec.y = g.first.y - EngineCore::camera->offset.y;
			tempRec.w = squareSize;
			tempRec.h = squareSize;

			SDL_SetRenderDrawColor(EngineCore::Renderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(EngineCore::Renderer, &tempRec);
			SDL_SetRenderDrawColor(EngineCore::Renderer, 99, 173, 255, 255);
		}
	}
}

void Collision::UpdateGrid(Collider* collider)
{
	//add collider to correct grid
	for (auto& g : grid)
	{
		tempRec.x = g.first.x - EngineCore::camera->offset.x;
		tempRec.y = g.first.y - EngineCore::camera->offset.y;
		tempRec.w = squareSize;
		tempRec.h = squareSize;

		bool colliderInsideGrid = AABB(tempRec, collider->collider);
		bool gridContainsCollider = std::find(g.second.begin(), g.second.end(), collider) != g.second.end();

		if (colliderInsideGrid && !gridContainsCollider)
		{
			g.second.push_back(collider);
		}
		else if (!colliderInsideGrid && gridContainsCollider || (gridContainsCollider && !collider->entity->IsActive()))
		{
			g.second.erase(remove(g.second.begin(), g.second.end(), collider), g.second.end());
		}
	}
}

bool Collision::CheckCollision(Collider* collider, Hit& hit)
{
	bool collided = false;
	bool pushedX = false;
	bool pushedY = false;

	hit = Hit(Vector2::Zero(), nullptr);

	for (auto& g : Collision::grid)
	{
		tempRec.x = g.first.x - EngineCore::camera->offset.x;
		tempRec.y = g.first.y - EngineCore::camera->offset.y;
		tempRec.w = squareSize;
		tempRec.h = squareSize;

		if (AABB(collider->collider, tempRec))
		{
			for (auto& c : g.second)
			{
				if (c != collider && c->entity->IsActive())
				{
					Vector2 otherPosition = c->Centre();
					Vector2 otherHalfSize = Vector2((float)c->collider.w / 2, (float)c->collider.h / 2);

					Vector2 thisPosition = collider->Centre();
					Vector2 thisHalfSize = Vector2((float)collider->collider.w / 2, (float)collider->collider.h / 2);

					float deltaX = otherPosition.x - thisPosition.x;
					float deltaY = otherPosition.y - thisPosition.y;
					float intersectX = abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
					float intersectY = abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

					Transform* thisTransform = &collider->entity->GetComponent<Transform>();
					Rigidbody* thisRigidbody = &collider->entity->GetComponent<Rigidbody>();

					Vector3 pos = thisTransform->GetPosition();
					float push = 0.0;//thisRigidbody->mass;

					if (intersectX < 0.0 && intersectY < 0.0f)
					{
						hit.normal = (otherPosition - thisPosition).Normalised();
						hit.collider = c;

						if (c->Trigger)
						{
							collider->entity->OnTrigger(&hit);
							c->entity->OnTrigger(&hit);
						}
						else
						{
							if (intersectX > intersectY)
							{
								if (!pushedX && abs(intersectX) > 1.0f)
								{
									pushedX = true;
									if (deltaX > 0.0f)
									{
										thisTransform->SetPosition(Vector3(pos.x + intersectX, pos.y, pos.z));
									}
									else
									{
										thisTransform->SetPosition(Vector3(pos.x - intersectX, pos.y, pos.z));
									}
								}
							}
							else
							{
								if (!pushedY && abs(intersectY) > 1.0f)
								{
									intersectY /= 2;
									pushedY = true;
									if (deltaY > 0.0f)
									{
										thisTransform->SetPosition(Vector3(pos.x, pos.y + intersectY, pos.z));
									}
									else
									{
										thisTransform->SetPosition(Vector3(pos.x, pos.y - intersectY, pos.z));
									}
								}
							}

							collider->entity->OnCollision(&hit);
							c->entity->OnCollision(&hit);

							collided = true;
						}
					}
				}
			}
		}
	}

	return collided;
}

bool Collision::AABB(const SDL_Rect& rectA, const SDL_Rect& rectB)
{
	SDL_bool b = SDL_HasIntersection(&rectA, &rectB);
	return b;
}

bool Collision::AABB(Collider& colA, Collider& colB)
{
	return AABB(colA.GetCollider(), colB.GetCollider());
}
