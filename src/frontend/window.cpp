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

void Frontend::show_cpu_info() {
  ImGui::Begin("CPU INFO", &state.cpu_info_open, 0);
  const auto& cpu = sys_instance->cell_be;
  ImGui::Columns(2, "Registers");
  for (u8 i = 0; i < 32; i++) {
    if (i % 2 == 0 && i != 0) ImGui::NextColumn();
    ImGui::Text("%s", fmt::format("r{:d}:   {:#016x}", i, sys_instance->cell_be.ppe.gpr.at(i)).c_str());
  }

  ImGui::Columns(1);
  ImGui::Text("%s", fmt::format("LR: {:#016x}", cpu.ppe.lr).c_str());

  ImGui::Text("%s", fmt::format("CIA: {:#016x}", cpu.ppe.cia).c_str());
  ImGui::Text("%s", fmt::format("NIA: {:#016x}", cpu.ppe.nia).c_str());
  ImGui::Separator();

  ImGui::BeginDisabled(true);
  ImGui::Spacing();
  ImGui::EndDisabled();

  ImGui::Separator();

  ImGui::Separator();

  ImGui::InputInt("step amount", &state.step_amount, 0, 0, 0);
  if (ImGui::Button("STEP")) {
    sys_instance->cell_be.step();
  }

  if (ImGui::Button("STEP AMOUNT")) {
    for (int i = 0; i < state.step_amount; i++) {
      sys_instance->cell_be.step();
    }
  }

  ImGui::Separator();
  if (ImGui::Button("Enable Logging")) {
    spdlog::set_level(spdlog::level::trace);
  }
  if (ImGui::Button("Disable Logging")) {
    spdlog::set_level(spdlog::level::off);
  }

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
  this->state.gpu_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XBGR8888, SDL_TEXTUREACCESS_TARGET, 240, 160);
  if (this->state.gpu_texture == NULL) {
    fmt::println("Could not create texture: {}", SDL_GetError());
    assert(0);
  }
  
  SDL_SetTextureScaleMode(state.gpu_texture, SDL_SCALEMODE_NEAREST);

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
