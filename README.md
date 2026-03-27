Physics Integration (Box2D): 
    Integrated the Box2D engine to handle realistic collisions and rigid-body dynamics. I managed the conversion between physics world coordinates and SFML screen pixels.

C++ Memory Management: 
    Practiced manual memory management and object lifetimes within a C++ environment to ensure stability and performance.

Collaborative Development: 
    Worked with a partner using a shared codebase, which required clear communication, modular code, and basic version control.

Engine Extension: 
    Built gameplay features on top of a provided base engine, learning how to read, understand, and extend existing professional-style code.

Collision Callbacks: 
    Utilized Box2D’s contact listeners to trigger game events (like scoring or sound effects) when specific objects collided, keep colliding or stop colliding.

Composition over Inheritance: 
    Moved away from a rigid inheritance hierarchy to a Component-based architecture. This allows for "mixing and matching" behaviors (e.g., adding a HealthComponent or DamageComponent) to any object without creating complex sub-classes.

Decoupled Logic: 
    By using components, I ensured that the Render logic doesn't need to know how the Movement logic works, making the code much easier to maintain and debug.

Flexibility: 
    This system made it simple to create new entity types quickly just by attaching existing components, rather than writing new classes from scratch.