#include <EnteeZ/EnteeZ.hpp>
#include <iostream>
#include <sstream>
#include <vector>

struct Position
{
	Position(float x, float y, float z) : x(x), y(y), z(z) {}
	float x;
	float y;
	float z;
};

template<typename T>
class MsgRecive
{
public:
	virtual void ReciveMessage(enteez::Entity* sender, T message) = 0;
private:

};

template<typename T>
void SendMessage(enteez::Entity* target, enteez::Entity* sender, T data)
{
	target->ForEach<MsgRecive<T>>([data](enteez::Entity* entity, MsgRecive<T>& recive)
	{
		recive.ReciveMessage(entity, data);
	});
}

struct MoveMessage
{
	float x;
	float y;
	float z;
};

struct Logic
{
	virtual void Update() = 0;
};

struct PlayerMovment : public Logic, public MsgRecive<MoveMessage>
{
	PlayerMovment(enteez::Entity* entity) : m_entity(entity) {}

	virtual void ReciveMessage(enteez::Entity* sender, MoveMessage message)
	{
		if (m_entity->HasComponent<Position>())
		{
			Position& position = m_entity->GetComponent<Position>();
			position.x += message.x;
			position.y += message.y;
			position.z += message.z;
		}
	}

	virtual void Update()
	{
		if (m_entity->HasComponent<Position>())
		{
			Position& position = m_entity->GetComponent<Position>();
			// Move the player forward
			position.z += 10;
		}
	}
private:
	enteez::Entity* m_entity;
};

int main(int argc, char **argv)
{
	// Memory Leak Checking
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

	{
		// Creation of entity manager
		enteez::EnteeZ ez;
		enteez::EntityManager& em = ez.GetEntityManager();

		// EG 1
		{
			std::cout << "Example 1" << std::endl;
			// Create a entity
			enteez::Entity* entity = em.CreateEntity();
			// Attach a structure to the entity and initilize its constructor with 3 floats
			entity->AddComponent<Position>(1.0f, 2.0f, 3.0f);
			// Check to see if we have the component
			bool hasComponent = entity->HasComponent<Position>();
			std::cout << "Has Component: " << (hasComponent ? "True" : "False") << std::endl;
			if (hasComponent)
			{
				// Get component and output its data
				Position& position = entity->GetComponent<Position>();
				std::cout << "Data: X:" << position.x << ", Y:" << position.y << ", Z:" << position.z << std::endl;
			}
			// Clean up the entity
			entity->Destroy();
			// or
			//em.DestroyEntity(entity);
		}

		std::cout << std::endl;

		{ // EG 2: Component looping
			std::cout << "Example 2" << std::endl;
			// Create a pointer array for 10 new entitys
			enteez::Entity** entities = new enteez::Entity*[10];
			// Will be used to store entity names
			std::stringstream ss;
			// Initilize them
			for (int i = 0; i < 10; i++)
			{
				// Create the entity
				entities[i] = em.CreateEntity();
				ss << "Entity " << i;
				// Add a name to the entity
				entities[i]->SetName(ss.str());
				// Clear the name buffer
				ss.str("");

				// Only add a component to every other entity
				if (i % 2 == 0)
				{
					entities[i]->AddComponent<Position>(1.0f, 2.0f, 3.0f);
				}
			}

			// There is a optional setting in the for each loop to allow any results to be cached.
			// Later on if a new entity is changed and it no longer or now matches the cache, the cache will automaticly update
			bool shouldCache = true;

			// Loop through all entitys and find one's that have x components. They must have all the components for it to be a match
			em.ForEach<Position>([](enteez::Entity* entity, Position& position) {
				std::cout << entity->GetName() << std::endl;
			}, shouldCache);

			// Clean up all entitys
			for (auto& it : em.GetEntitys())
			{
				it->Destroy();
			}
			delete entities;
		}

		std::cout << std::endl;

		{ // EG 3 : Looping through components that inherit another
			std::cout << "Example 3" << std::endl;

			// Since C++ dose not have any form of meta data that retains information about what inherts what else. We have to tell Enteez about this inheritance
			// Here we are saying that the Player Movment component will inherit Logic
			ez.RegisterBase<PlayerMovment, Logic>();
			// You can also define things your inherited objects inherit if needed as well as define multiple inheritances at the same time
			//ez.RegisterBase<PlayerMovment, Logic, .., ..>();

			// Create a entity
			enteez::Entity* entity = em.CreateEntity();

			// Add 2 components, one with the inheritance and one without
			entity->AddComponent<Position>(1.0f, 2.0f, 3.0f);
			entity->AddComponent<PlayerMovment>(entity);

			// Loop through the components for each component that inherits the requested object
			entity->ForEach<Logic>([](enteez::Entity* entity, Logic& logic) {
				logic.Update();
			});

			// There is no inbuilt safeguard for when a component dose not exist for effitency, so it is down to the user to make sure entitys exist before accessing
			if (entity->HasComponent<Position>())
			{
				Position& position = entity->GetComponent<Position>();
				std::cout << "Position: X:" << position.x << ", Y:" << position.y << ", Z:" << position.z << std::endl;
			}

			// Cleanup
			entity->Destroy();
		}

		std::cout << std::endl;

		{ // EG 4
			std::cout << "Example 4" << std::endl;

			// Register another base for player movment, this one is for reciving messages
			ez.RegisterBase<PlayerMovment, MsgRecive<MoveMessage>>();

			// Create a entity
			enteez::Entity* entity = em.CreateEntity();

			// Add the position and player movment components
			entity->AddComponent<Position>(1.0f,2.0f,3.0f);
			entity->AddComponent<PlayerMovment>(entity);

			// Create a new message to send to the entity
			MoveMessage msg{};
			msg.x = 3.0f;
			msg.y = 2.0f;
			msg.z = 1.0f;

			// Call the user defined function to send messages, with no sender set
			SendMessage(entity,nullptr, msg);


			// There is no inbuilt safeguard for when a component dose not exist for effitency, so it is down to the user to make sure entitys exist before accessing
			if (entity->HasComponent<Position>())
			{
				Position& position = entity->GetComponent<Position>();
				std::cout << "Position: X:" << position.x << ", Y:" << position.y << ", Z:" << position.z << std::endl;
			}
			// Cleanup
			entity->Destroy();
		}



		{


			enteez::Entity* entity = em.CreateEntity();
			enteez::ComponentWrapper<PlayerMovment>* a = entity->AddComponent<PlayerMovment>(entity);


			Logic* logic = nullptr;
			if (em.BaseClassInstance(*a, logic))
			{

			}


		}



	}
	
	// End of memory leak checking
	_CrtDumpMemoryLeaks();

    return 0;
}
