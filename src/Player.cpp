#include "Player.hpp"
#include "skills/Skill.hpp"

Player::Player()
{
}

Player::~Player()
{
}

/*! The static method to access the singleton */
Player & Player::singleton()
{
    static Player instance;

    return instance;
}

int Player::char_to_skill_index(char c)
{
    int i = c - '1';
    if (i >= (int)skills.size() || i < 0)
    {
        return -1;
    }
    else
    {
        return i;
    }
}

void Player::preload_skill_textures(SDL_Renderer * renderer)
{
    for (unsigned int i=0; i<skills.size(); i++)
    {
        skills[i]->load_texture(renderer);
    }
}
