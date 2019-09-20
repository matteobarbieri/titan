#include "Player.hpp"
#include "Skill.hpp"

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

void Player::preload_skill_textures(SDL_Renderer * renderer)
{
    for (unsigned int i=0; i<skills.size(); i++)
    {
        skills[i]->load_texture(renderer);
    }
}
