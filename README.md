# Enteez

Enteez is a lightwate Entity Component System that lets users create, manage and destroy entities whilst adding new functionality to them through the use of components.

Components are any valid C++ object, such as a struct or class, or even a C++ primitive, such as an int or float.

This library gives the ability for quick entity lookups based on what components they have and can cache the results for faster retrieval later on.

## Building [CMake]

To build the project, make sure you have CMake 3.5 or newer installed and run:

```
cmake -S . -B build
cmake --build build
```

On Windows this generates a Visual Studio solution inside the build/ directory. Three targets are built: the EnteeZ static library, the EnteeZ-demo example program and the EnteeZ-tests test suite.

## Running the tests

The EnteeZ-tests target is a small assert-based suite covering the core API (add/get/remove, ownership, query caching, base-class iteration) plus regression tests. Run it directly, or through CTest:

```
cd build
ctest -C Debug
```

## Example Usage

### Creating an instance of Enteez
```
// Creation of entity manager
enteez::EnteeZ ez;
enteez::EntityManager& em = ez.GetEntityManager();
```
Enteez is split up into two primary parts, EntityManager and EnteeZ. EntityManager is used for the day to day management of entities in the scene whereas EnteeZ is used for defining various configs for the library.

### Creating a Entity
Creating a entity is done by calling em.CreateEntity() and the required memory is allocated for you.
```
enteez::Entity* entity = em.CreateEntity();
```
You are able to give a entity a name so that you can identityfy them at a later date.
```
entity->SetName("Ryan");
```

### Adding a component
Before we can add a component, we need to create an object to be held, in this example, we will define a very basic position object.
```
struct Position
{
    Position(float x, float y, float z) : x(x), y(y), z(z) {}
    float x;
    float y;
    float z;
};
```
One thing to note about this object is that it has no default constructor and only has 1 constructor with 3 parameters, we are able to attach a new component to an object and call the new component's constructor at the same time with the following line.

```
// Attach a structure to the entity and initilize its constructor with 3 floats
entity->AddComponent<Position>(1.0f, 2.0f, 3.0f);
```

The entity owns components added this way and deletes them when the component is removed or the entity is destroyed. Alternatively, you can hand the entity a pointer to a component you own yourself; in that case the entity will never delete it and you remain responsible for its lifetime.

```
Position* position = new Position(1.0f, 2.0f, 3.0f);
entity->AddComponent<Position>(position);
// ... later, after removing it or destroying the entity ...
delete position;
```

Adding a component type the entity already has replaces the old component with the new one (the old one is freed if the entity owned it).

### Check to see if the entity has a component
```
// Check to see if we have the component
bool hasComponent = entity->HasComponent<Position>();
// Or check several at once, the entity must have all of them
bool hasBoth = entity->HasComponent<Position, Velocity>();
```

### Get a component from a entity
```
Position& position = entity->GetComponent<Position>();
```
For efficiency there is no safety check inside GetComponent; calling it for a component the entity does not have is undefined behaviour, so guard it with HasComponent first.

### Remove a component from a entity
```
// Remove the component from the entity
entity->RemoveComponent<Position>();
```

### Clean up the entity
We can clean up the entity in two ways. We can either ask the entity manager to clean up the entity for us or ask the entity itself to do it.

Entity Manager, Entity Cleanup
```
em.DestroyEntity(entity);
```
Entity cleaning up itself
```
entity->Destroy();
```

### Loop through entities

If we want to do a normal loop through all the entities, this can be done through.
```
for (auto& it : em.GetEntitys())
{
  //...
}
```
But with a project that could hold hundreds of entities, this could prove slow, the next best thing is to loop through all entities that have x components.

```
// There is an optional setting in the for each loop to allow any results to be cached.
// Later on if a new entity is changed and it no longer or now matches the cache, the cache will automatically update
bool shouldCache = true;

// Loop through all entities and find ones that have x components. They must have all the components for it to be a match
em.ForEach<Position>([](enteez::Entity* entity, Position& position) {
  std::cout << entity->GetName() << std::endl;
}, shouldCache);
```
Optionaly, we can define a flag to say that we wish to cache the results or even to check againsed multiple components.
```
em.ForEach<Position, Rotation>([](enteez::Entity* entity, Position& position, Rotation& rotation) {
   //...
});
```

It is safe to destroy entities from inside a ForEach lambda; destroyed entities are skipped for the rest of the loop.

If you just want the matching entities rather than a loop, use
```
std::vector<enteez::Entity*> movers = em.GetEntitysWith<Position, Rotation>();
```

### Component looping
In other ECS such as Unity's, they require you to have an Update function ether defined in your component or in the base class, that will be called once per logic update. With this system, you are able to define what components are called when doing logic loops, and you can have several different types of logic loops at the same time. This is ideal as we don't want to be looping through components that either has no update function body defined or we may just be updating too frequently.

Since C++ has no metadata to retain where a inheritance is, we need to define this information ourselves. First, we need to create an object that inherits another object.

```
struct Logic
{
    virtual void Update() = 0;
};

struct PlayerMovement : public Logic
{
    PlayerMovment(enteez::Entity* entity) : m_entity(entity) {}

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
```
Here we have a component that inherits a logic object. Now when we need to define the inheritance so later on, when we request to loop through all components that inherit logic, only the correct one's will be returned.

```
// Since C++ does not have any form of metadata that retains information about what inherits what else. We have to tell Enteez about this inheritance
// Here we are saying that the Player Movement component will inherit Logic
ez.RegisterBase<PlayerMovement, Logic>();
// You can also define things your inherited objects inherit if needed as well as define multiple inheritances at the same time
//ez.RegisterBase<PlayerMovment, Logic, .., ..>();
```

Finally, when it comes to looping through the components, we define it like so

```
// Loop through the components for each component that inherits the requested object
entity->ForEach<Logic>([](enteez::Entity* entity, Logic& logic) {
  logic.Update();
});
```

You can also test a single component wrapper against a registered base directly:

```
enteez::ComponentWrapper<PlayerMovement>* wrapper = entity->AddComponent<PlayerMovement>(entity);

Logic* logic = nullptr;
if (em.BaseClassInstance(*wrapper, logic))
{
    logic->Update();
}
```

## Notes and limits

* A maximum of 100 distinct component types can be used per EnteeZ instance (the size of the internal component bitset).
* Component type indices are assigned lazily on first use, so they are only stable within a single run; never serialise them.
* The library is not thread safe; create, query and destroy entities from a single thread.
* Components are stored per entity on the heap rather than in contiguous per-type arrays, so iteration is pointer chasing rather than cache-linear. This is fine for small and medium scenes but is the first thing to revisit for very large ones.

