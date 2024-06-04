
#pragma once

// Core Event Loop
#include "engine/Event.h"
#include <engine/RTTI.h>
#include <memory>
#include <string>
#include <string_view>

#include <math/Crc32.h>
#include <vector>

namespace hpl {
enum class BroadcastEvent {
  OnPostBufferSwap,
  OnStart,
  OnDraw,
  OnPostRender,
  PreUpdate,
  Update,
  PostUpdate,
  OnQuit,
  OnExit,
  Reset,
  OnPauseUpdate,

  AppGotInputFocus,
  AppGotMouseFocus,
  AppGotVisibility,

  AppLostInputFocus,
  AppLostMouseFocus,
  AppLostVisibility,

  AppDeviceWasPlugged,
  AppDeviceWasRemoved,

  LastEnum
};

class UpdateEventLoop final {
public:
  // store event handlers separately for each event group
  using UpdateEvent = hpl::Event<float>;
  using ChangeGroupEvent = hpl::Event<>;

  struct EventGroupBroadcast {
    ChangeGroupEvent m_enterEventGroup;
    ChangeGroupEvent m_leaveEventGroup;
    std::array<UpdateEvent, static_cast<size_t>(BroadcastEvent::LastEnum)>
        m_events;
  };

  struct EventGroup {
    std::string m_name = "";
    math::Crc32 m_id = math::Crc32();
    std::unique_ptr<EventGroupBroadcast> m_broadcast =
        std::make_unique<EventGroupBroadcast>();
  };

  UpdateEventLoop();
  ~UpdateEventLoop();

  void ChangeEventGroup(std::string_view name);

  const std::string_view GetActiveEventGroup() const; 

  void CreateEventGroup(std::string_view name);
  void DestroyEventGroup(std::string_view name);

  // broadcast to the global event group and the active event group
  void Broadcast(BroadcastEvent event, float value = 0);
  // broadcast to all event groups
  void BroadcastToAll(BroadcastEvent event, float value = 0);

  void Subscribe(BroadcastEvent event, UpdateEvent::Handler &handler);
  void Subscribe(const std::string_view id, BroadcastEvent event,
                 UpdateEvent::Handler &handler);

private:
  std::vector<EventGroup> m_eventGroups;
  math::Crc32 m_activeEventGroup = math::Crc32();
  std::array<UpdateEvent, static_cast<size_t>(BroadcastEvent::LastEnum)>
      m_events;
};
} // namespace hpl
