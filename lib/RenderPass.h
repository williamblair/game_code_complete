#ifndef RENDERPASS_H_INCLUDED
#define RENDERPASS_H_INCLUDED

enum RenderPass
{
    RenderPass_0,                       // A constant to define the starting pass
    RenderPass_Static = RenderPass_0,   // environments and level geometry
    RenderPass_Actor,                   // objects and things that can move
    RenderPass_Sky,                     // the background 'behind' everything
    RenderPass_NotRendered,             // objects that don't render but exist
    RenderPass_Last                     // not used - a counter for "for" loops
};

#endif // RENDERPASS_H_INCLUDED
