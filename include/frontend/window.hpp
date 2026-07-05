#pragma once

#include <string>
#include <thread>
#include <unordered_map>

#include "SDL3/SDL.h"
#include "SDL3/SDL_video.h"
#include "imgui.h"
#include "sysinstance.hpp"
#include "spdlog/spdlog.h"
// TODO: add breakpoints in debugger

struct State {
  SDL_Texture* gpu_texture = nullptr;

  std::array<SDL_Texture*, 4> background_textures{};
  std::array<SDL_Texture*, 4> background_affine_textures{};

  const bool* keyboard_state = SDL_GetKeyboardState(nullptr);
  std::atomic<bool> running  = true;

  bool memory_viewer_open   = true;
  bool cpu_info_open        = true;
  
  bool halted                  = false;
  int step_amount              = 0;
  ImGuiIO* io                  = nullptr;
};

struct Keymap_State {
  SDL_Scancode current_key;
  bool being_remapped;
};


enum class KEY { CROSS, SQUARE, TRIANGLE, CIRCLE, DPAD_UP, DPAD_LEFT, DPAD_RIGHT, DPAD_DOWN, START, SELECT, L1, L2, R1, R2, NONE = 0xFF };


struct Settings {
  struct Keybinds {
    std::unordered_map<KEY, Keymap_State> control_map = {
        {    KEY::SQUARE, {SDL_SCANCODE_INTERNATIONAL1, false}},
        {     KEY::CROSS, {SDL_SCANCODE_INTERNATIONAL1, false}},
        {    KEY::CIRCLE, {SDL_SCANCODE_INTERNATIONAL1, false}},
        {  KEY::TRIANGLE, {SDL_SCANCODE_INTERNATIONAL1, false}},
        {    KEY::SELECT,      {SDL_SCANCODE_BACKSPACE, false}},
        {     KEY::START,         {SDL_SCANCODE_RETURN, false}},
        {KEY::DPAD_RIGHT,          {SDL_SCANCODE_RIGHT, false}},
        { KEY::DPAD_LEFT,           {SDL_SCANCODE_LEFT, false}},
        {   KEY::DPAD_UP,             {SDL_SCANCODE_UP, false}},
        { KEY::DPAD_DOWN,           {SDL_SCANCODE_DOWN, false}},
    };

  } keybinds;
};

struct Frontend {
  SDL_Window* window;
  SDL_Renderer* renderer;
  State state;
  SysInstance* sys_instance = nullptr;
  std::thread sdlThread;
  SDL_AudioStream* stream = nullptr;
  SDL_FRect rect{0, 0, 1280, 720};

  constexpr static const SDL_DialogFileFilter filters[] = {
      {"Executables", "self,elf,bin"},
  };

  void init_sdl();

  void handle_events();

  void render_frame();

  void show_menu_bar();
  void show_audio_info();

  
  void init_audio_device();
  void shutdown();
  void show_memory_viewer();
  void show_cpu_info();
  void show_controls_menu(bool*);

  explicit Frontend(SysInstance*);

  Settings settings;
};


const inline std::unordered_map<KEY, std::string> buttons = {
    {     KEY::CROSS,    "CROSS"},
    {    KEY::SQUARE,   "SQUARE"},
    {    KEY::CIRCLE,   "CIRCLE"},
    {  KEY::TRIANGLE, "TRIANGLE"},
    {     KEY::START,    "START"},
    {    KEY::SELECT,   "SELECT"},
    {KEY::DPAD_RIGHT,    "RIGHT"},
    { KEY::DPAD_LEFT,     "LEFT"},
    {   KEY::DPAD_UP,       "UP"},
    { KEY::DPAD_DOWN,     "DOWN"},
};