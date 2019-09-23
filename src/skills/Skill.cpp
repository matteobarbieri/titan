#include "Skill.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

Skill::Skill(const char * name, const char * icon_path) :
    icon_path(icon_path), name(name) 
{
    icon_texture = NULL;
}

Skill::~Skill()
{
    // Destroy the texturs
    SDL_DestroyTexture(icon_texture);
}


void Skill::load_texture(SDL_Renderer * renderer)
{
    
    // First create a SDL_Surface from the png image
    SDL_Surface * icon_surface = IMG_Load(icon_path);

    // Then create a texture from the surface
    icon_texture = SDL_CreateTextureFromSurface(renderer, icon_surface);

    // Destroy surface as it is no longer needed
    SDL_FreeSurface(icon_surface);
}


SDL_Texture * Skill::get_icon_texture()
{
    return icon_texture;
}
