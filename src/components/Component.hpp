#ifndef ROGUE_20177_COMPONENT
#define ROGUE_20177_COMPONENT 

class Entity;

class Component
{
    private:

       Entity * _owner;

    public:
       Component();
       ~Component();

       void setOwner(Entity * owner);
};

#endif /* ifndef ROGUE_20177_COMPONENT */
