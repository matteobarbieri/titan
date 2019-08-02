#include "Constants.h"
#include "Uid.hpp"

/*! The static method to access the singleton */
Uid & Uid::singleton()
{
    static Uid instance;

    return instance;
}

Uid::~Uid()
{
}

/*! Private constructor */
Uid::Uid()
{
    /*! Initialize pointers for consoles */
    last_id = 0;
    
}

unsigned long int Uid::next_id()
{
    last_id++;

    return last_id;
}
