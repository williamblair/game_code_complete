#ifndef GCC4_RENDERPASS_H_INCLUDED
#define GCC4_RENDERPASS_H_INCLUDED

enum RenderPass
{
    RenderPass_0,                       // A constant to define the starting pass
    RenderPass_Sky = RenderPass_0,      // the background 'behind' everything
    RenderPass_Static,   // environments and level geometry
    RenderPass_Actor,                   // objects and things that can move
    RenderPass_NotRendered,             // objects that don't render but exist
    RenderPass_Last                     // not used - a counter for "for" loops
};

#endif // GCC4_RENDERPASS_H_INCLUDED
