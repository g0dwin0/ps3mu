#include "frontend/window.hpp"

#include <spdlog/common.h>

#include <atomic>

#include "SDL3/SDL_dialog.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "common/stopwatch.hpp"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "imgui_memory_edit.h"
#include "sysinstance.hpp"

static MemoryEditor editor_instance;

void Frontend::handle_events() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_EVENT_QUIT: {
        SPDLOG_INFO("SDL QUIT CALLED");
        state.running = false;
        break;
      }
/*
      case SDL_EVENT_KEY_DOWN: {
        if (state.keyboard_state[settings.keybinds.control_map.at(RIGHT).current_key]) {
          agb->bus.keypad_input.KEYINPUT.RIGHT = 0;
        }
        if (state.keyboard_state[settings.keybinds.control_map.at(LEFT).current_key]) {
          agb->bus.keypad_input.KEYINPUT.LEFT = 0;
        }
        if (state.keyboard_state[settings.keybinds.control_map.at(UP).current_key]) {
          agb->bus.keypad_input.KEYINPUT.UP = 0;
        }
        if (state.keyboard_state[settings.keybinds.control_map.at(DOWN).current_key]) {
          agb->bus.keypad_input.KEYINPUT.DOWN = 0;
        }
        if (state.keyboard_state[settings.keybinds.control_map.at(A).current_key]) {
          agb->bus.keypad_input.KEYINPUT.A = 0;
        }
        if (state.keyboard_state[settings.keybinds.control_map.at(B).current_key]) {
          agb->bus.keypad_input.KEYINPUT.B = 0;
        }
        if (state.keyboard_state[settings.keybinds.control_map.at(L).current_key]) {
          agb->bus.keypad_input.KEYINPUT.L = 0;
        }
        if (state.keyboard_state[settings.keybinds.control_map.at(R).current_key]) {
          agb->bus.keypad_input.KEYINPUT.R = 0;
        }
        if (state.keyboard_state[settings.keybinds.control_map.at(SELECT).current_key]) {
          agb->bus.keypad_input.KEYINPUT.SELECT = 0;
        }
        if (state.keyboard_state[settings.keybinds.control_map.at(START).current_key]) {
          agb->bus.keypad_input.KEYINPUT.START = 0;
        }

        break;
      }

      case SDL_EVENT_KEY_UP: {
        if (!state.keyboard_state[settings.keybinds.control_map.at(RIGHT).current_key]) {
          agb->bus.keypad_input.KEYINPUT.RIGHT = 1;
        }
        if (!state.keyboard_state[settings.keybinds.control_map.at(LEFT).current_key]) {
          agb->bus.keypad_input.KEYINPUT.LEFT = 1;
        }
        if (!state.keyboard_state[settings.keybinds.control_map.at(UP).current_key]) {
          agb->bus.keypad_input.KEYINPUT.UP = 1;
        }
        if (!state.keyboard_state[settings.keybinds.control_map.at(DOWN).current_key]) {
          agb->bus.keypad_input.KEYINPUT.DOWN = 1;
        }
        if (!state.keyboard_state[settings.keybinds.control_map.at(A).current_key]) {
          agb->bus.keypad_input.KEYINPUT.A = 1;
        }
        if (!state.keyboard_state[settings.keybinds.control_map.at(B).current_key]) {
          agb->bus.keypad_input.KEYINPUT.B = 1;
        }
        if (!state.keyboard_state[settings.keybinds.control_map.at(L).current_key]) {
          agb->bus.keypad_input.KEYINPUT.L = 1;
        }
        if (!state.keyboard_state[settings.keybinds.control_map.at(R).current_key]) {
          agb->bus.keypad_input.KEYINPUT.R = 1;
        }
        if (!state.keyboard_state[settings.keybinds.control_map.at(SELECT).current_key]) {
          agb->bus.keypad_input.KEYINPUT.SELECT = 1;
        }
        if (!state.keyboard_state[settings.keybinds.control_map.at(START).current_key]) {
          agb->bus.keypad_input.KEYINPUT.START = 1;
        }

        break;
      }

      */
    }
    ImGui_ImplSDL3_ProcessEvent(&event);
  }
}

void Frontend::show_memory_viewer() {
  ImGui::Begin("Memory Viewer", &state.memory_viewer_open, 0);
  // const char* regions[] = {"Region: [0x00000000 - 0x00003FFF] BIOS", "Region: [0x02000000 - 0x0203FFFF] EWRAM", "Region: [0x03000000 - 0x03007FFF] IWRAM",
  //                          // "Region: [0x04000000 - 0x040003FE] IO",
  //                          "Region: [0x05000000 - 0x050003FF] BG/OBJ Palette RAM", "Region: [0x06000000 - 0x06017FFF] VRAM", "Region: [0x07000000 - 0x070003FF] OAM",
  //                          "Region: [0x08000000 - 0x09FFFFFF] Game Pak ROM", "Region: [0x0E000000 - 0x0E00FFFF] Game Pak SRAM", "Region: BGMAP1"};

  // const std::vector<u8>* memory_partitions[] = {
  //     &agb->bus.BIOS,
  //     &agb->bus.EWRAM,
  //     &agb->bus.IWRAM,
  //     // &agb->bus.IO,
  //     &agb->ppu.PALETTE_RAM,
  //     &agb->ppu.VRAM,
  //     &agb->bus.OAM,
  //     &agb->pak.data,
  //     &agb->pak.SRAM,

  // };

  // static int SelectedItem = 0;

  // if (ImGui::Combo("Regions", &SelectedItem, regions, IM_ARRAYSIZE(regions))) {
  //   SPDLOG_DEBUG("switched to: {}", regions[SelectedItem]);
  // }
  // editor_instance.OptShowAscii = false;
  // // editor_instance.ReadOnly = true;

  // if (SelectedItem == 8) {
  //   // agb->ppu.tile_map_texture_buffer_1,
  //   editor_instance.DrawContents((void*)agb->ppu.tile_map_texture_buffer_arr[0].data(), sizeof(u32) * 512 * 512);
  // } else {
  //   editor_instance.DrawContents((void*)memory_partitions[SelectedItem]->data(), memory_partitions[SelectedItem]->size());
  // }

  ImGui::End();
}
// void Frontend::show_irq_status() {
//   ImGui::Begin("IRQ STATUS");
//   ImGui::Text("%s", fmt::format("IME: {}", agb->bus.interrupt_control.IME.enabled ? 1 : 0).c_str());
//   ImGui::Separator();
//   ImGui::Text("IE - IF");
//   ImGui::Text("%s", fmt::format("LCD VBLANK:        {} - {}", agb->bus.interrupt_control.IE.LCD_VBLANK ? 1 : 0, agb->bus.interrupt_control.IF.LCD_VBLANK ? 1 : 0).c_str());
//   ImGui::Text("%s", fmt::format("LCD HBLANK:        {} - {}", agb->bus.interrupt_control.IE.LCD_HBLANK ? 1 : 0, agb->bus.interrupt_control.IF.LCD_HBLANK ? 1 : 0).c_str());
//   ImGui::Text("%s", fmt::format("LCD VCOUNT_MATCH:  {} - {}", agb->bus.interrupt_control.IE.LCD_V_COUNTER_MATCH ? 1 : 0, agb->bus.interrupt_control.IF.LCD_V_COUNTER_MATCH ? 1 : 0).c_str());
//   ImGui::Text("%s", fmt::format("TIMER0 OVERFLOW:   {} - {}", agb->bus.interrupt_control.IE.TIMER0_OVERFLOW ? 1 : 0, agb->bus.interrupt_control.IF.TIMER0_OVERFLOW ? 1 : 0).c_str());
//   ImGui::Text("%s", fmt::format("TIMER1 OVERFLOW:   {} - {}", agb->bus.interrupt_control.IE.TIMER1_OVERFLOW ? 1 : 0, agb->bus.interrupt_control.IF.TIMER1_OVERFLOW ? 1 : 0).c_str());
//   ImGui::Text("%s", fmt::format("TIMER2 OVERFLOW:   {} - {}", agb->bus.interrupt_control.IE.TIMER2_OVERFLOW ? 1 : 0, agb->bus.interrupt_control.IF.TIMER2_OVERFLOW ? 1 : 0).c_str());
//   ImGui::Text("%s", fmt::format("TIMER3 OVERFLOW:   {} - {}", agb->bus.interrupt_control.IE.TIMER3_OVERFLOW ? 1 : 0, agb->bus.interrupt_control.IF.TIMER3_OVERFLOW ? 1 : 0).c_str());
//   ImGui::Text("%s", fmt::format("SERIAL COMM:       {} - {}", agb->bus.interrupt_control.IE.SERIAL_COM ? 1 : 0, agb->bus.interrupt_control.IF.SERIAL_COM ? 1 : 0).c_str());
//   ImGui::Text("%s", fmt::format("DMA0:              {} - {}", agb->bus.interrupt_control.IE.DMA0 ? 1 : 0, agb->bus.interrupt_control.IF.DMA0 ? 1 : 0).c_str());
//   ImGui::Text("%s", fmt::format("DMA1:              {} - {}", agb->bus.interrupt_control.IE.DMA1 ? 1 : 0, agb->bus.interrupt_control.IF.DMA1 ? 1 : 0).c_str());
//   ImGui::Text("%s", fmt::format("DMA2:              {} - {}", agb->bus.interrupt_control.IE.DMA2 ? 1 : 0, agb->bus.interrupt_control.IF.DMA2 ? 1 : 0).c_str());
//   ImGui::Text("%s", fmt::format("DMA3:              {} - {}", agb->bus.interrupt_control.IE.DMA3 ? 1 : 0, agb->bus.interrupt_control.IF.DMA3 ? 1 : 0).c_str());
//   ImGui::Text("%s", fmt::format("KEYPAD:            {} - {}", agb->bus.interrupt_control.IE.KEYPAD ? 1 : 0, agb->bus.interrupt_control.IF.KEYPAD ? 1 : 0).c_str());
//   ImGui::Text("%s", fmt::format("GAMEPAK_EXT:       {} - {}", agb->bus.interrupt_control.IE.GAMEPAK ? 1 : 0, agb->bus.interrupt_control.IF.GAMEPAK ? 1 : 0).c_str());
//   ImGui::End();
// }
// void Frontend::show_tiles() {
//   ImGui::Begin("Tile Window");

//   ImGui::Text("Tile Set");
//   ImGui::Image(state.tile_set_texture, ImVec2(256, 256));
//   ImGui::Separator();
//   ImGui::Text("Tilemap");
//   ImGui::Image(state.tile_map_texture, ImVec2(256 * 2, 256 * 2));
//   ImGui::End();
// }

// void Frontend::show_obj() {
//   ImGui::Begin("Sprite/OBJ Window");

//   ImGui::Text("Sprite Texture -- relative to BG");
//   ImGui::Image(state.obj_texture, ImVec2(256, 256));
//   ImGui::Separator();

//   ImGui::End();
// }

// void Frontend::show_timer_window() {
//   ImGui::Begin("Show Timer Window", &state.timer_window_open);

//   ImGui::SeparatorText("Timer 0");
//   // bool enabled_0     = agb->timers[0].ctrl.timer_start_stop;
//   // bool irq_enabled_0 = agb->timers[0].ctrl.timer_irq_enable;

//   ImGui::Text("Ticking Enabled: %d", agb->timers[0].ctrl.timer_start_stop);
//   ImGui::Text("IRQ Enabled: %d", agb->timers[0].ctrl.timer_irq_enable);
//   ImGui::Text("Counter: %d", agb->timers[0].counter);
//   ImGui::Text("Prescaler: %s", agb->timers[0].get_prescaler_string().c_str());

//   // =========================

//   ImGui::SeparatorText("Timer 1");

//   ImGui::Text("Ticking Enabled: %d", agb->timers[1].ctrl.timer_start_stop);
//   ImGui::Text("IRQ Enabled: %d", agb->timers[1].ctrl.timer_irq_enable);
//   ImGui::Text("Counter: %d", agb->timers[1].counter);
//   ImGui::Text("Prescaler: %s", agb->timers[1].get_prescaler_string().c_str());

//   // =========================
//   ImGui::SeparatorText("Timer 2");
//   bool enabled_2     = agb->timers[2].ctrl.timer_start_stop;
//   bool irq_enabled_2 = agb->timers[2].ctrl.timer_irq_enable;

//   ImGui::Checkbox("Ticking Enabled##l2", &enabled_2);
//   ImGui::Checkbox("IRQ Enabled##il2", &irq_enabled_2);
//   ImGui::Text("Counter: %d", agb->timers[2].counter);
//   ImGui::Text("Prescaler: %s", agb->timers[2].get_prescaler_string().c_str());

//   // =========================
//   ImGui::SeparatorText("Timer 3");
//   bool enabled_3     = agb->timers[3].ctrl.timer_start_stop;
//   bool irq_enabled_3 = agb->timers[3].ctrl.timer_irq_enable;

//   ImGui::Checkbox("Ticking Enabled##l3", &enabled_3);
//   ImGui::Checkbox("IRQ Enabled##il3", &irq_enabled_3);

//   ImGui::Text("Counter: %d", agb->timers[3].counter);
//   ImGui::Text("Prescaler: %s", agb->timers[3].get_prescaler_string().c_str());

//   ImGui::End();
// }
// void Frontend::show_viewport() {
//   ImGui::Begin("Viewport");
//   ImGui::Image(state.ppu_texture, {ImGui::GetWindowWidth() - 10, ImGui::GetWindowHeight() - 10});

//   ImGui::End();
// }
// void Frontend::show_audio_info() {
//   ImGui::Begin("Audio", &state.apu_window_open, 0);
//   ImGui::Text("Write position: %d", agb->apu.write_pos);
//   ImGui::Text("FIFO A size: %lu", agb->apu.FIFO_A.size());
//   ImGui::Text("FIFO B size: %lu", agb->apu.FIFO_B.size());

//   ImGui::End();
// }


void Frontend::show_cpu_info() {
  ImGui::Begin("CPU INFO", &state.cpu_info_open, 0);


  ImGui::End();
}

void Frontend::show_menu_bar() {
  if (ImGui::BeginMainMenuBar()) {
    ImGui::Separator();
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Load ROM")) {
        // SDL_ShowOpenFileDialog(
        //     [](void* userdata, const char* const* filelist, int filter) {
        //       if (filter == -1) {
        //         spdlog::info("no filters");
        //       }

        //       auto agb = static_cast<AGB*>(userdata);
        //       (void)(agb);
        //       fmt::println("{}", *filelist);

        //       // agb->pak.load_data(std::vector<u8> &)
        //     },
        //     this, window, filters, 1, "./", false);
      }
      if (ImGui::MenuItem("Reset")) {}
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("State")) {
      if (ImGui::MenuItem("Save state...")) {}
      if (ImGui::MenuItem("Load state...")) {}
      if (ImGui::MenuItem("Reset")) {}
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}
void Frontend::shutdown() {
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_Quit();
}

void Frontend::render_frame() {
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  if (state.cpu_info_open) {
    show_cpu_info();
  }

  if (state.memory_viewer_open) {
    show_memory_viewer();
  }

  ImGui::Render();
  SDL_SetRenderScale(renderer, state.io->DisplayFramebufferScale.x, state.io->DisplayFramebufferScale.y);

  SDL_SetRenderTarget(renderer, NULL);
  SDL_RenderClear(renderer);
  
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

  SDL_RenderPresent(renderer);
}

void Frontend::init_sdl() {
  spdlog::info("initializing SDL");

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD | SDL_INIT_AUDIO)) {
    spdlog::error("ERROR: failed initialize SDL");
    exit(-1);
  }

  auto window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

  this->window = SDL_CreateWindow("ps3mu", 640, 480, window_flags);
  if (this->window == nullptr) {
    spdlog::error("failed to create window");
    assert(0);
  }
  // SDL_SetRenderVSync(renderer, 1);

  this->renderer = SDL_CreateRenderer(window, nullptr);
  // init_audio_device();
  if (this->renderer == nullptr) {
    spdlog::error("failed to create renderer");
    assert(0);
  }

  spdlog::info("window ptr: {}", fmt::ptr(this->window));
  spdlog::info("render ptr: {}", fmt::ptr(this->renderer));

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;

  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  this->state.io = &io;

  ImGui::StyleColorsDark();

  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);
  this->state.ppu_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XBGR8888, SDL_TEXTUREACCESS_TARGET, 240, 160);
  if (this->state.ppu_texture == NULL) {
    fmt::println("Could not create texture: {}", SDL_GetError());
  }
  this->state.tile_set_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XBGR8888, SDL_TEXTUREACCESS_TARGET, 240, 160);
  if (this->state.tile_set_texture == NULL) {
    fmt::println("Could not create texture: {}", SDL_GetError());
  }
  this->state.tile_map_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XBGR8888, SDL_TEXTUREACCESS_TARGET, 512, 512);
  if (this->state.tile_map_texture == NULL) {
    fmt::println("Could not create texture: {}", SDL_GetError());
  }
  this->state.backdrop = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XBGR8888, SDL_TEXTUREACCESS_TARGET, 512, 512);
  if (this->state.backdrop == NULL) {
    fmt::println("Could not create texture: {}", SDL_GetError());
  }
  this->state.obj_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XBGR8888, SDL_TEXTUREACCESS_TARGET, 256, 256);

  SDL_SetTextureScaleMode(state.ppu_texture, SDL_SCALEMODE_NEAREST);

  for (size_t bg_id = 0; bg_id < 4; bg_id++) {
    this->state.background_textures[bg_id] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XBGR8888, SDL_TEXTUREACCESS_TARGET, 512, 512);
    if (state.background_textures[bg_id] == NULL) {
      fmt::println("Could not create texture: {}", SDL_GetError());
    }
  }
  for (size_t bg_id = 0; bg_id < 4; bg_id++) {
    this->state.background_affine_textures[bg_id] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XBGR8888, SDL_TEXTUREACCESS_TARGET, 1024, 1024);
    if (state.background_affine_textures[bg_id] == NULL) {
      fmt::println("Could not create texture: {}", SDL_GetError());
    }
  }
};
void Frontend::init_audio_device() {
  SDL_AudioSpec spec = {};
  spec.freq          = 48000;
  spec.format        = SDL_AUDIO_S8;
  spec.channels      = 2;
  // stream             = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, audio_callback, agb);
  stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);

  if (!stream) {
    fmt::println("Couldn't create audio stream: {}", SDL_GetError());
    exit(-1);
  }

  if (!SDL_ResumeAudioStreamDevice(stream)) {
    fmt::println("failed to unpause audio device: {}", SDL_GetError());
    assert(0);
  }

  spdlog::info("initialized audio device");
}

Frontend::Frontend(SysInstance* instance) : sys_instance(instance) { init_sdl(); }
