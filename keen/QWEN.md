# Coding Style Guidelines

## Naming Conventions

### Classes and Functions
- Use **CapitalizedCase** (PascalCase) for all class names and function names
- Examples:
  - Class: `VideoRenderer`, `GameManager`, `AudioDevice`
  - Function: `InitializeSystem`, `LoadTexture`, `ProcessInput`

### Variables
- Use **underscore_style** (snake_case) for all variable names
- Examples:
  - Variables: `game_state`, `player_position`, `texture_width`
  - Member variables: `current_frame`, `render_target`, `audio_buffer`

### Enums
- Use **UPPER_CASE** for all enum values
- Examples:
  - `GAME_STATE_PLAYING`, `RENDER_MODE_OPENGL`, `INPUT_TYPE_KEYBOARD`

## Avoidances
- Do not use **camelCase** in any part of the codebase
- Maintain consistency with the chosen styles throughout all new code

## Other Style Guidelines
- Use RAII principles with raw C-pointers that are zeroed at definition
- Initialize all pointer variables to nullptr at definition
- Implement proper destructors for automatic cleanup
- Ensure exception safety with proper cleanup in destructors
- Follow consistent naming and memory management patterns