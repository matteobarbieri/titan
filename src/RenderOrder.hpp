#ifndef R20177_RENDER_ORDER
#define R20177_RENDER_ORDER

/*! \enum RenderOrder
 *
 * The rendering order of entities on map
 */
enum RenderOrder {
    NONE = 0,
    STAIRS = 10,
    SFX = 11,
    CORPSE = 20,
    ITEM = 30,
    ACTOR = 40
};

#endif
